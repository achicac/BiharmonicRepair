#ifndef MULTIGRIDSOLVER_H
#define MULTIGRIDSOLVER_H


#include "grid3Dlod.h"

class MultigridSolver
{

public:

    enum StepType { CHOLESKY, CONJGRADIENT };

    void solve(Grid3D<float>* distgrid, Grid3D<bool>* mask, Grid3D<float>* holedist);

private:

    void           computeBoundaryDistances( Grid3D<float>* distanceField, const Grid3DLOD<bool>& maskField);
    void           applyStep(StepType stype, Grid3D<float>* distanceField, const Grid3DLOD<bool>& maskField, const Grid3DLOD<float>& holeDist);
    void           applyStepAdaptive(StepType stype, Grid3D<float>* distanceField, const Grid3DLOD<bool>& maskField, const Grid3DLOD<float>& holeDist, const Grid3D<bool>& voxelsUnknown);
    Grid3D<float>* upscaleSolution(Grid3D<float>* lowResGrid, int factor);
    Grid3D<bool>*  getVoxelsToUpdate(const Grid3D<float>* field, int bandSize = 1);

    double         getUnknownsAdaptiveRatio(const Grid3D<bool>& voxelsUnknown) const;

};



#endif // MULTIGRIDSOLVER_H
