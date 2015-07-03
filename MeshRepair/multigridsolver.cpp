#include "multigridsolver.h"
#include "grid3Dlod.h"
#include "gridinterpolatortrilinear.h"
#include "bilaplacian.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <cmath>
#include <iostream>
#include <ctime>

float SMOOTHING_STRENGTH         = 1.0f;
float CONSTRAINT_STRENGTH_WEAK   = 5.0f;
float CONSTRAINT_STRENGTH_STRONG = 5.0f;
#define CONSTRAINT_THRESHOLD        8.0
#define COARSE_LEVEL_SIZE           16
#define MAX_ITERATIONS              100

using namespace Eigen;


void MultigridSolver::solve(Grid3D<float>* inGrid, Grid3D<bool>* inMask, Grid3D<float>* inHoleDist)
{
    // compute number of LOD levels required
    int minsize = std::min(inGrid->getWidth(), std::min(inGrid->getHeight(), inGrid->getDepth()));
    int levels  = minsize > COARSE_LEVEL_SIZE ? int(std::log(double(minsize)/double(COARSE_LEVEL_SIZE))/std::log(2.0) + 0.5) : 0;

    // get lowest resolution level
    Grid3D<float>* auxGrid  = Grid3DLOD<float>(inGrid, levels).getGrid();
    Grid3DLOD<bool> lodMask(inMask, levels);

    std::cout << "Solving system for resolution " << auxGrid->getWidth() << "x";
    std::cout << auxGrid->getHeight() << "x" << auxGrid->getDepth() << "..." << std::endl;

    // low-res first step using cholesky
    applyStep(CHOLESKY, auxGrid, lodMask, Grid3DLOD<float>(inHoleDist, levels));

    // now that we have an approximation, use this as initial guess for iterative solvers
    for (int lod = levels-1; lod >= 0; lod--) {

        // upscale
        Grid3D<float>* upGrid = upscaleSolution(auxGrid, 2);
        delete auxGrid;
        auxGrid = upGrid;

        std::cout << "Solving system for resolution " << auxGrid->getWidth() << "x";
        std::cout << auxGrid->getHeight() << "x" << auxGrid->getDepth() << "..." << std::endl;

        // replace not-masked values with ground truth
        lodMask = Grid3DLOD<bool>(inMask, lod);
        Grid3DLOD<float> lodValue(inGrid, lod);
        for (unsigned int i = 0; i < auxGrid->getWidth(); i++) {
            for (unsigned int j = 0; j < auxGrid->getHeight(); j++) {
                for (unsigned int k = 0; k < auxGrid->getDepth(); k++) {
                    if (!lodMask.get(i, j, k))
                        auxGrid->set(i, j, k, lodValue.get(i, j, k));
                }
            }
        }

        // find voxels to update
        Grid3D<bool>* unknowns = getVoxelsToUpdate(auxGrid, 1);

        // smooth
        applyStepAdaptive(CONJGRADIENT, auxGrid, lodMask, Grid3DLOD<float>(inHoleDist, lod), *unknowns);

        delete unknowns;
    }

    // copy solution
    for (unsigned int i = 0; i < auxGrid->getWidth(); i++) {
        for (unsigned int j = 0; j < auxGrid->getHeight(); j++) {
            for (unsigned int k = 0; k < auxGrid->getDepth(); k++) {
                inGrid->set(i, j, k, auxGrid->get(i, j, k));
            }
        }
    }
}


bool isBoundary(int x, int y, int z, int sizeX, int sizeY, int sizeZ) {
    return x < 2 || y < 2 || z < 2 || x >= sizeX - 2 || y >= sizeY - 2 || z >= sizeZ - 2;
}


Grid3D<bool>* MultigridSolver::getVoxelsToUpdate(const Grid3D<float>* field, int bandSize)
{
    int Nx = field->getWidth();
    int Ny = field->getHeight();
    int Nz = field->getDepth();

    // init
    Grid3D<bool>* surfaceVoxels = new Grid3D<bool>(Nx, Ny, Nz);
    for (int i = 0; i < Nx; i++) {
        for (int j = 0; j < Ny; j++) {
            for (int k = 0; k < Nz; k++) {
                surfaceVoxels->set(i, j, k, false);
            }
        }
    }

    // get surface voxels
    for (int i = 0; i < Nx - 1; i++) {
        for (int j = 0; j < Ny - 1; j++) {
            for (int k = 0; k < Nz - 1; k++) {
                float first = field->get(i, j, k);
                if (    field->get(i, j, k+1)    * first <= 0
                    ||  field->get(i, j+1, k)    * first <= 0
                    ||  field->get(i, j+1, k+1)  * first <= 0
                    ||  field->get(i+1, j, k)    * first <= 0
                    ||  field->get(i+1, j, k+1)  * first <= 0
                    ||  field->get(i+1, j+1, k)  * first <= 0
                    ||  field->get(i+1, j+1, k+1)* first <= 0)
                {
                    surfaceVoxels->set(i, j, k, true);
                    surfaceVoxels->set(i, j, k+1, true);
                    surfaceVoxels->set(i, j+1, k, true);
                    surfaceVoxels->set(i, j+1, k+1, true);
                    surfaceVoxels->set(i+1, j, k, true);
                    surfaceVoxels->set(i+1, j, k+1, true);
                    surfaceVoxels->set(i+1, j+1, k, true);
                    surfaceVoxels->set(i+1, j+1, k+1, true);
                }
            }
        }
    }

    // expand the surface voxels one unit
    Grid3D<bool>* updateVoxels = new Grid3D<bool>(Nx, Ny, Nz);
    for (int i = 0; i < Nx; i++) {
        for (int j = 0; j < Ny; j++) {
            for (int k = 0; k < Nz; k++) {
                bool toUpdate = false;
                for (int di = -bandSize; di <= bandSize; di++) {
                    if (i + di < 0 || i + di >= Nx) continue;
                    for (int dj = -bandSize; dj <= bandSize; dj++) {
                        if (j + dj < 0 || j + dj >= Ny) continue;
                        for (int dk = -bandSize; dk <= bandSize; dk++) {
                            if (k + dk < 0 || k + dk >= Nz) continue;
                            toUpdate = toUpdate || surfaceVoxels->get(i + di, j + dj, k + dk);
                        }
                    }
                }
                updateVoxels->set(i, j, k, toUpdate);
            }
        }
    }

    delete surfaceVoxels;

    return updateVoxels;
}



void MultigridSolver::applyStep(StepType stype, Grid3D<float> *distanceField, const Grid3DLOD<bool> &maskField, const Grid3DLOD<float> &holeDist)
{
    int SizeX = distanceField->getWidth();
    int SizeY = distanceField->getHeight();
    int SizeZ = distanceField->getDepth();
    Grid3D<bool> dummy(SizeX, SizeY, SizeZ);
    for (int i = 0; i < SizeX; i++) {
        for (int j = 0; j < SizeY; j++) {
            for (int k = 0; k < SizeZ; k++) {
                dummy.set(i, j, k, true);
            }
        }
    }
    applyStepAdaptive(stype, distanceField, maskField, holeDist, dummy);
}

void MultigridSolver::applyStepAdaptive(StepType stype, Grid3D<float>* distanceField, const Grid3DLOD<bool>& maskField,
                                        const Grid3DLOD<float>& holeDist, const Grid3D<bool>& voxelsUnknown)
{
    // first of all, smooth borders
    computeBoundaryDistances(distanceField, maskField);

    // initializations
    int SizeX = distanceField->getWidth();
    int SizeY = distanceField->getHeight();
    int SizeZ = distanceField->getDepth();

    Grid3D<int> distances2Unknowns(SizeX, SizeY, SizeZ);
    for (int i = 0; i < SizeX; i++) {
        for (int j = 0; j < SizeY; j++) {
            for (int k = 0; k < SizeZ; k++) {
                distances2Unknowns.set(i, j, k, -1);
            }
        }
    }

    // compute number of equations and unknowns
    int nEquations = 0;
    int nUnknowns  = 0;
    for (int i = 2; i < SizeX - 2; i++) {
        for (int j = 2; j < SizeY - 2; j++) {
            for (int k = 2; k < SizeZ - 2; k++) {
                // there's an equation for each unknown grid position (discarding positions closer than kernelRadius to border)
                if (voxelsUnknown.get(i, j, k)) {
                    nEquations++;
                    for (int di = -2; di <= 2; di++) {
                        for (int dj = -2; dj <= 2; dj++) {
                            for (int dk = -2; dk <= 2; dk++) {
                                if (   voxelsUnknown.get(i+di, j+dj, k+dk)
                                    && fabs(Convolution[di+2][dj+2][dk+2]) > 1e-3
                                    && distances2Unknowns.get(i+di, j+dj, k+dk) == -1)
                                {
                                    // add new unknown
                                    distances2Unknowns.set(i+di, j+dj, k+dk, nUnknowns);
                                    nUnknowns++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // there's also a constraint equation for each unknown close to the surface
    for (int i = 0; i < SizeX; i++) {
        for (int j = 0; j < SizeY; j++) {
            for (int k = 0; k < SizeZ; k++) {
                if (    distances2Unknowns.get(i, j, k) != -1
                    && (isBoundary(i, j, k, SizeX, SizeY, SizeZ) || !maskField.get(i, j, k)) )
                {
                    nEquations++;
                }
            }
        }
    }


    // sparse entries of A and independent terms vector
    std::vector<Triplet<double> > triplets;
    VectorXd b;
    b.resize(nEquations);

    int equationId = 0;

    int numVoxelsWeakConstraint = 0;
    int numVoxelsStrgConstraint = 0;

    for (int i = 0; i < SizeX; i++) {
        for (int j = 0; j < SizeY; j++) {
            for (int k = 0; k < SizeZ; k++) {

                // smoothing equation
                if (!isBoundary(i, j, k, SizeX, SizeY, SizeZ) && voxelsUnknown.get(i, j, k))
                {
                    double bTerm = 0;
                    for (int di = -2; di <= 2; di++) {
                        for (int dj = -2; dj <= 2; dj++) {
                            for (int dk = -2; dk <= 2; dk++) {
                                if (fabs(Convolution[di+2][dj+2][dk+2]) > 1e-3) {
                                    if (distances2Unknowns.get(i+di, j+dj, k+dk) != -1) {
                                        triplets.push_back(Triplet<double>(equationId,
                                                                           distances2Unknowns.get(i+di, j+dj, k+dk),
                                                                           SMOOTHING_STRENGTH*Convolution[di+2][dj+2][dk+2]));
                                    }
                                    else {
                                        bTerm -= SMOOTHING_STRENGTH * Convolution[di+2][dj+2][dk+2] * distanceField->get(i+di, j+dj, k+dk);
                                    }
                                }
                            }
                        }
                    }
                    b(equationId) = bTerm;
                    equationId++;
                }

                // constraint equation
                if (    distances2Unknowns.get(i, j, k) != -1
                    && (isBoundary(i, j, k, SizeX, SizeY, SizeZ) || !maskField.get(i, j, k)))
                {
                    float Cstrength = holeDist.get(i, j, k) > CONSTRAINT_THRESHOLD ? CONSTRAINT_STRENGTH_WEAK : CONSTRAINT_STRENGTH_STRONG;
                    if (holeDist.get(i, j, k) > CONSTRAINT_THRESHOLD)
                        numVoxelsWeakConstraint++;
                    else
                        numVoxelsStrgConstraint++;

                    triplets.push_back(Triplet<double>(equationId,
                                                       distances2Unknowns.get(i, j, k),
                                                       Cstrength));

                    b(equationId) = Cstrength * distanceField->get(i, j, k);
                    equationId++;
                }
            }
        }
    }


    SparseMatrix<double> A  (nEquations, nUnknowns );
    SparseMatrix<double> At (nUnknowns,  nEquations);
    SparseMatrix<double> M  (nUnknowns,  nUnknowns);
    VectorXd             Atb(nUnknowns );
    VectorXd             x  (nUnknowns );


    // Solve
    A.setFromTriplets(triplets.begin(), triplets.end());
    At  = A.transpose();
    M   = At * A;
    Atb = At * b;

    switch (stype)
    {
        case CHOLESKY: {
            SimplicialCholesky<SparseMatrix<double> > chol(M);
            if(chol.info() == NumericalIssue)
                std::cerr << "Numerical issue while computing Cholesky decomposition" << std::endl;
            x = chol.solve(Atb);
        } break;

        case CONJGRADIENT: {
            for (int i = 0; i < SizeX; i++) {
                for (int j = 0; j < SizeY; j++) {
                    for (int k = 0; k < SizeZ; k++) {
                        if (distances2Unknowns.get(i, j, k) != -1) {
                            x(distances2Unknowns.get(i, j, k)) = distanceField->get(i, j, k);
                        }
                    }
                }
            }

            ConjugateGradient<SparseMatrix<double> > cgSolver;
            cgSolver.setMaxIterations(MAX_ITERATIONS);
            cgSolver.compute(M);
            x = cgSolver.solveWithGuess(Atb, x);
        } break;

        default:
            break;
    }

    // Store result
    for (int i = 0; i < SizeX; i++) {
        for (int j = 0; j < SizeY; j++) {
            for (int k = 0; k < SizeZ; k++) {
                if (distances2Unknowns.get(i, j, k) != -1) {
                    distanceField->set(i, j, k, float(x(distances2Unknowns.get(i, j, k))));
                }
            }
        }
    }
}


Grid3D<float>* MultigridSolver::upscaleSolution(Grid3D<float>* lowResGrid, int factor)
{
    int SizeX = lowResGrid->getWidth();
    int SizeY = lowResGrid->getHeight();
    int SizeZ = lowResGrid->getDepth();

    GridInterpolatorTrilinear interpolator(lowResGrid);
    Grid3D<float>* hiResGrid = interpolator.getScaledGrid(factor*SizeX, factor*SizeY, factor*SizeZ);

    return hiResGrid;
}



void MultigridSolver::computeBoundaryDistances(Grid3D<float> *distanceField, const Grid3DLOD<bool> &maskField)
{
    int SizeX = distanceField->getWidth();
    int SizeY = distanceField->getHeight();
    int SizeZ = distanceField->getDepth();

    Grid3D<int> grid2unknown(SizeX, SizeY, SizeZ);
    for (int i = 0; i < SizeX; i++) {
        for (int j = 0; j < SizeY; j++) {
            for (int k = 0; k < SizeZ; k++) {
                grid2unknown.set(i, j, k, -1);
            }
        }
    }

    // compute number of equations and unknowns
    int nEquations = 0;
    int nUnknowns  = 0;
    for (int i = 0; i < SizeX; i++) {
        for (int j = 0; j < SizeY; j++) {
            for (int k = 0; k < SizeZ; k++) {
                if (isBoundary(i, j, k, SizeX, SizeY, SizeZ) && maskField.get(i, j, k)) {
                    grid2unknown.set(i, j, k, nUnknowns);
                    nUnknowns++;
                }
            }
        }
    }
    for (int i = 0; i < SizeX; i++) {
        for (int j = 0; j < SizeY; j++) {
            for (int k = 0; k < SizeZ; k++) {
                if (isBoundary(i, j, k, SizeX, SizeY, SizeZ)) {
                    // X laplacian (1D)
                    if (i > 0 && i < SizeX-1
                        && isBoundary(i-1, j, k, SizeX, SizeY, SizeZ)
                        && isBoundary(i+1, j, k, SizeX, SizeY, SizeZ)
                        && (maskField.get(i, j, k) || maskField.get(i-1, j, k) || maskField.get(i+1, j, k)))
                    {
                        nEquations++;
                    }
                    // Y laplacian (1D)
                    if (j > 0 && j < SizeY-1
                        && isBoundary(i, j-1, k, SizeX, SizeY, SizeZ)
                        && isBoundary(i, j+1, k, SizeX, SizeY, SizeZ)
                        && (maskField.get(i, j, k) || maskField.get(i, j-1, k) || maskField.get(i, j-1, k)))
                    {
                        nEquations++;
                    }
                    // Z laplacian (1D)
                    if (k > 0 && k < SizeZ-1
                        && isBoundary(i, j, k-1, SizeX, SizeY, SizeZ)
                        && isBoundary(i, j, k+1, SizeX, SizeY, SizeZ)
                        && (maskField.get(i, j, k) || maskField.get(i, j, k-1) || maskField.get(i, j, k+1)))
                    {
                        nEquations++;
                    }
                }
            }
        }
    }

    // sparse entries of A and independent terms vector
    std::vector<Triplet<double> > triplets;
    VectorXd b;
    b.resize(nEquations);

    int equationId = 0;
    for (int i = 0; i < SizeX; i++) {
        for (int j = 0; j < SizeY; j++) {
            for (int k = 0; k < SizeZ; k++) {
                if (isBoundary(i, j, k, SizeX, SizeY, SizeZ)) {
                    // X laplacian (1D)
                    if (i > 0 && i < SizeX-1
                        && isBoundary(i-1, j, k, SizeX, SizeY, SizeZ)
                        && isBoundary(i+1, j, k, SizeX, SizeY, SizeZ)
                        && (maskField.get(i, j, k) || maskField.get(i-1, j, k) || maskField.get(i+1, j, k)))
                    {
                        b(equationId) = 0;

                        if (maskField.get(i-1, j, k)) {
                            triplets.push_back(Triplet<double>(equationId, grid2unknown.get(i-1, j, k), 1));
                        }
                        else {
                            b(equationId) -= distanceField->get(i-1, j, k);
                        }
                        if (maskField.get(i, j, k)) {
                            triplets.push_back(Triplet<double>(equationId, grid2unknown.get(i, j, k), -2));
                        }
                        else {
                            b(equationId) += 2*distanceField->get(i, j, k);
                        }
                        if (maskField.get(i+1, j, k)) {
                            triplets.push_back(Triplet<double>(equationId, grid2unknown.get(i+1, j, k), 1));
                        }
                        else {
                            b(equationId) -= distanceField->get(i+1, j, k);
                        }

                        equationId++;
                    }
                    // Y laplacian (1D)
                    if (j > 0 && j < SizeY-1
                        && isBoundary(i, j-1, k, SizeX, SizeY, SizeZ)
                        && isBoundary(i, j+1, k, SizeX, SizeY, SizeZ)
                        && (maskField.get(i, j, k) || maskField.get(i, j-1, k) || maskField.get(i, j-1, k)))
                    {
                        b(equationId) = 0;

                        if (maskField.get(i, j-1, k)) {
                            triplets.push_back(Triplet<double>(equationId, grid2unknown.get(i, j-1, k), 1));
                        }
                        else {
                            b(equationId) -= distanceField->get(i, j-1, k);
                        }
                        if (maskField.get(i, j, k)) {
                            triplets.push_back(Triplet<double>(equationId, grid2unknown.get(i, j, k), -2));
                        }
                        else {
                            b(equationId) += 2*distanceField->get(i, j, k);
                        }
                        if (maskField.get(i, j+1, k)) {
                            triplets.push_back(Triplet<double>(equationId, grid2unknown.get(i, j+1, k), 1));
                        }
                        else {
                            b(equationId) -= distanceField->get(i, j+1, k);
                        }

                        equationId++;
                    }
                    // Z laplacian (1D)
                    if (k > 0 && k < SizeZ-1
                        && isBoundary(i, j, k-1, SizeX, SizeY, SizeZ)
                        && isBoundary(i, j, k+1, SizeX, SizeY, SizeZ)
                        && (maskField.get(i, j, k) || maskField.get(i, j, k-1) || maskField.get(i, j, k+1)))
                    {
                        b(equationId) = 0;

                        if (maskField.get(i, j, k-1)) {
                            triplets.push_back(Triplet<double>(equationId, grid2unknown.get(i, j, k-1), 1));
                        }
                        else {
                            b(equationId) -= distanceField->get(i, j, k-1);
                        }
                        if (maskField.get(i, j, k)) {
                            triplets.push_back(Triplet<double>(equationId, grid2unknown.get(i, j, k), -2));
                        }
                        else {
                            b(equationId) += 2*distanceField->get(i, j, k);
                        }
                        if (maskField.get(i, j, k+1)) {
                            triplets.push_back(Triplet<double>(equationId, grid2unknown.get(i, j, k+1), 1));
                        }
                        else {
                            b(equationId) -= distanceField->get(i, j, k+1);
                        }

                        equationId++;
                    }
                }
            }
        }
    }

    SparseMatrix<double> A  (nEquations, nUnknowns );
    SparseMatrix<double> At (nUnknowns,  nEquations);
    SparseMatrix<double> M  (nUnknowns,  nUnknowns);
    VectorXd             Atb(nUnknowns );
    VectorXd             x  (nUnknowns );


    // Solve
    A.setFromTriplets(triplets.begin(), triplets.end());
    At  = A.transpose();
    M   = At * A;
    Atb = At * b;

    SimplicialCholesky<SparseMatrix<double> > chol(M);
    if(chol.info() == NumericalIssue)
        std::cerr << "Numerical issue while computing Cholesky decomposition" << std::endl;
    x = chol.solve(Atb);

    // Store result
    for (int i = 0; i < SizeX; i++) {
        for (int j = 0; j < SizeY; j++) {
            for (int k = 0; k < SizeZ; k++) {
                if (grid2unknown.get(i, j, k) != -1) {
                    distanceField->set(i, j, k, float(x(grid2unknown.get(i, j, k))));
                }
            }
        }
    }

}

