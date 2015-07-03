#include "holerepairer.h"
#include "geomtests.h"
#include "multigridsolver.h"
#include "MC/MarchingCubes.h"
#include <queue>


HoleRepairer::HoleRepairer(int voxSamples) :
        m_distanceGrid(voxSamples),
        m_triangleMask(voxSamples),
        m_holeDistance(voxSamples)
{
    NUM_VOXELS  = voxSamples - 1;
    VOX_SAMPLES = voxSamples;
    m_mesh = NULL;
    m_outMesh = NULL;
}

HoleRepairer::~HoleRepairer()
{
}

void HoleRepairer::execute()
{
    m_boundingBox = m_mesh->getBoundingBox();
    glm::vec3 bbc = m_boundingBox.getCenter();
    m_boundingBox.limits[0] = 1.01f*(m_boundingBox.limits[0] - bbc) + bbc;
    m_boundingBox.limits[1] = 1.01f*(m_boundingBox.limits[1] - bbc) + bbc;

    // detect holes
    int numHoles = m_mesh->detectHoles();
    std::cout << "    found " << numHoles << " holes" << std::endl;

    m_mesh->computeConnectivity();

    if (m_mesh->hasBorder()) {
        // expand border triangles so they lie outside the bounding box
        expandBorderTriangles();
    }

    prepareGrid();
    computeSignedDistanceField();

    if (m_mesh->hasBorder()) {
        recoverBorderTriangles();
    }

    MultigridSolver solver;
    solver.solve(&m_distanceGrid, &m_triangleMask, &m_holeDistance);

    m_minDistanceNegative = -1e32f;
    m_maxDistanceNegative = m_maxDistancePositive = 0;
    m_minDistancePositive = 1e32f;
    for (int i = 0; i < VOX_SAMPLES; i++) {
        for (int j = 0; j < VOX_SAMPLES; j++) {
            for (int k = 0; k < VOX_SAMPLES; k++) {
                float dist = m_distanceGrid.get(i, j, k);
                dist = std::max(-m_boundingBox.getCircumRadius()*4, dist);
                dist = std::min( m_boundingBox.getCircumRadius()*4, dist);
                if (dist < 0 && dist > m_minDistanceNegative) m_minDistanceNegative = dist;
                if (dist < 0 && dist < m_maxDistanceNegative) m_maxDistanceNegative = dist;
                if (dist > 0 && dist < m_minDistancePositive) m_minDistancePositive = dist;
                if (dist > 0 && dist > m_maxDistancePositive) m_maxDistancePositive = dist;
            }
        }
    }

    std::cout << "Extracting isosurface with MC..." << std::endl;
    extractIsosurface();
}


void HoleRepairer::prepareGrid()
{
    // make sure BBox is cubic by expanding it
    glm::vec3 bbsize = m_boundingBox.getDimensions();
    float maxside = std::max(std::max(bbsize.x, bbsize.y), bbsize.z);
    for (int i = 0; i < 3; i++) {
        float incr = maxside - bbsize[i];
        m_boundingBox.limits[0][i] -= incr/2;
        m_boundingBox.limits[1][i] += incr/2;
    }
    m_voxelSize = glm::vec3(maxside/float(NUM_VOXELS));

    // init field and mask
    m_distanceGrid.fill(1e32f);
    m_holeDistance.fill(1e32f);
    m_triangleMask.fill(false);
}


void HoleRepairer::expandBorderTriangles()
{
    std::vector<int> border;
    m_mesh->getBorder(border);
    m_originalBorder = std::vector<glm::vec3>(border.size());

    int numTriangles = m_mesh->getNumTriangles();
    int numVertices  = m_mesh->getNumVertices();
    std::vector<glm::vec3>  vtxNormal(numVertices);
    std::vector<float> vtxDenom(numVertices);
    for (int i = 0; i < numTriangles; i++) {
        int v0, v1, v2;
        m_mesh->getTriangle(i, v0, v1, v2);
        glm::vec3 n = m_mesh->getFaceNormal(i);
        float a = m_mesh->getFaceArea(i);
        vtxNormal[v0] += a*n;
        vtxNormal[v1] += a*n;
        vtxNormal[v2] += a*n;
        vtxDenom[v0] += a;
        vtxDenom[v1] += a;
        vtxDenom[v2] += a;
    }
    for (int i = 0; i < numVertices; i++) {
        if (glm::length(vtxNormal[i]) > 1e-6f) {
            vtxNormal[i] = glm::normalize(vtxNormal[i]/vtxDenom[i]);        
		}
    }

    glm::vec3 centroid = m_boundingBox.getCenter();
    float rad = m_boundingBox.getCircumRadius();

    for (unsigned int i = 0; i < border.size(); i++) {
        glm::vec3 vtx = m_mesh->getVertex(border[i]);
        m_originalBorder[i] = vtx;

        float dist = glm::length(vtx - centroid);
        glm::vec3 c = glm::normalize(vtx - centroid);
        glm::vec3 dir = c;
        vtx += (dist*rad/dist - 0.9f*dist)*dir;

        m_mesh->setVertex(border[i], vtx);
    }
}

void HoleRepairer::recoverBorderTriangles()
{
    std::vector<int> border;
    m_mesh->getBorder(border);
    for (unsigned int i = 0; i < border.size(); i++) {
        m_mesh->setVertex(border[i], m_originalBorder[i]);
    }
}


float minDist(const Grid3D<float>* grid, float dist, int i, int j, int k, int di, int dj, int dk, int W, int H, int D, const glm::vec3& S)
{
    if (i + di < 0)  return dist;
    if (j + dj < 0)  return dist;
    if (k + dk < 0)  return dist;
    if (i + di >= W) return dist;
    if (j + dj >= H) return dist;
    if (k + dk >= D) return dist;
    float ndist = grid->get(i+di, j+dj, k+dk);
    float ndisp = glm::length(glm::vec3(S.x*di, S.y*dj, S.z*dk));
    return std::min(dist, ndist + ndisp);
}


void propagateGridValues(Grid3D<float>* grid, const glm::vec3& voxelSize) {
    
    int WIDTH  = grid->getWidth();
    int HEIGHT = grid->getHeight();
    int DEPTH  = grid->getDepth();
    
    // first pass
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            for (int k = 0; k < DEPTH; k++) {
                float dist = grid->get(i, j, k);

                dist = minDist(grid, dist, i, j, k, -1, -1, -1, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k, -1, -1,  0, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k, -1, -1,  1, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k, -1,  0, -1, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k, -1,  0,  0, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k, -1,  0,  1, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k, -1,  1, -1, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k, -1,  1,  0, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k, -1,  1,  1, WIDTH, HEIGHT, DEPTH, voxelSize);

                dist = minDist(grid, dist, i, j, k,  0, -1, -1, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k,  0, -1,  0, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k,  0, -1,  1, WIDTH, HEIGHT, DEPTH, voxelSize);

                dist = minDist(grid, dist, i, j, k,  0,  0, -1, WIDTH, HEIGHT, DEPTH, voxelSize);

                grid->set(i, j, k, dist);
            }
        }
    }
    
    // second pass
    for (int i = WIDTH - 1; i >= 0; i--) {
        for (int j = HEIGHT - 1; j >= 0; j--) {
            for (int k = DEPTH - 1; k >= 0; k--) {
                float dist = grid->get(i, j, k);

                dist = minDist(grid, dist, i, j, k,  1,  1,  1, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k,  1,  1,  0, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k,  1,  1, -1, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k,  1,  0,  1, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k,  1,  0,  0, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k,  1,  0, -1, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k,  1, -1,  1, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k,  1, -1,  0, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k,  1, -1, -1, WIDTH, HEIGHT, DEPTH, voxelSize);

                dist = minDist(grid, dist, i, j, k,  0,  1,  1, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k,  0,  1,  0, WIDTH, HEIGHT, DEPTH, voxelSize);
                dist = minDist(grid, dist, i, j, k,  0,  1, -1, WIDTH, HEIGHT, DEPTH, voxelSize);

                dist = minDist(grid, dist, i, j, k,  0,  0,  1, WIDTH, HEIGHT, DEPTH, voxelSize);

                grid->set(i, j, k, dist);
            }
        }
    }
}


void HoleRepairer::computeUDFmesh(Grid3D<float>* distanceGrid)
{
    int numTriangles = m_mesh->getNumTriangles();
    glm::vec3 bbmin  = m_boundingBox.limits[0];

    // compute distances from triangles
    for (int tri = 0; tri < numTriangles; tri++) {

        int itv0, itv1, itv2;
        m_mesh->getTriangle(tri, itv0, itv1, itv2);
        const glm::vec3& v0 = m_mesh->getVertex(itv0);
        const glm::vec3& v1 = m_mesh->getVertex(itv1);
        const glm::vec3& v2 = m_mesh->getVertex(itv2);

        glm::vec3 tn = glm::cross(v1 - v0, v2 - v0);
        if (glm::length(tn) < 1e-15) continue;

        glm::vec3 c0 = (v0 - bbmin)/m_voxelSize;
        glm::vec3 c1 = (v1 - bbmin)/m_voxelSize;
        glm::vec3 c2 = (v2 - bbmin)/m_voxelSize;

        int imin = glm::clamp(int(std::min(std::min(c0.x, c1.x), c2.x)) - 1, 0, VOX_SAMPLES-1);
        int jmin = glm::clamp(int(std::min(std::min(c0.y, c1.y), c2.y)) - 1, 0, VOX_SAMPLES-1);
        int kmin = glm::clamp(int(std::min(std::min(c0.z, c1.z), c2.z)) - 1, 0, VOX_SAMPLES-1);
        int imax = glm::clamp(int(std::max(std::max(c0.x, c1.x), c2.x)) + 1, 0, VOX_SAMPLES-1);
        int jmax = glm::clamp(int(std::max(std::max(c0.y, c1.y), c2.y)) + 1, 0, VOX_SAMPLES-1);
        int kmax = glm::clamp(int(std::max(std::max(c0.z, c1.z), c2.z)) + 1, 0, VOX_SAMPLES-1);

        glm::vec3 voxmin(0);
        for (int i = imin; i <= imax; i++) {
            voxmin.x = (i == VOX_SAMPLES-1 ? i-1 : i)*m_voxelSize.x + bbmin.x;
            for (int j = jmin; j <= jmax; j++) {
                voxmin.y = (j == VOX_SAMPLES-1 ? j-1 : j)*m_voxelSize.y + bbmin.y;
                for (int k = kmin; k <= kmax; k++) {
                    voxmin.z = (k == VOX_SAMPLES-1 ? k-1 : k)*m_voxelSize.z + bbmin.z;
                    int voxid = i*VOX_SAMPLES*VOX_SAMPLES + j*VOX_SAMPLES + k;
                    glm::vec3 svoxel = glm::vec3(i, j, k)*m_voxelSize + bbmin;
                    float dist = pointToTriangleUnsignedDist(svoxel, v0, v1, v2);
                    float fval = distanceGrid->get(i, j, k);
                    if (fval > dist + 1e-3) {
                        distanceGrid->set(i, j, k, dist);
                    }

                    // extend triangle to deal with precission issues
                    float eps = 0.5;
                    glm::vec3 bc = (v0 + v1 + v2)/3.0f;
                    glm::vec3 ev0 = v0 + eps*glm::length(m_voxelSize)*glm::normalize(v0 - bc);
                    glm::vec3 ev1 = v1 + eps*glm::length(m_voxelSize)*glm::normalize(v1 - bc);
                    glm::vec3 ev2 = v2 + eps*glm::length(m_voxelSize)*glm::normalize(v2 - bc);

                    // sticks
                    if (i < VOX_SAMPLES - 1 && segmentTriangleIntersection(svoxel, svoxel + glm::vec3(1, 0, 0)*m_voxelSize, ev0, ev1, ev2)) {
                        m_surfaceSticks.insert(std::make_pair(voxid, voxid + VOX_SAMPLES*VOX_SAMPLES));
                        m_surfaceSticks.insert(std::make_pair(voxid + VOX_SAMPLES*VOX_SAMPLES, voxid));
                        if (pointInPositiveHalfSpace(svoxel, ev0, ev1, ev2)) {
                            m_positivePoints.insert(voxid);
                        }
                        else {
                            m_positivePoints.insert(voxid + VOX_SAMPLES*VOX_SAMPLES);
                        }
                    }
                    if (j < VOX_SAMPLES - 1 && segmentTriangleIntersection(svoxel, svoxel + glm::vec3(0, 1, 0)*m_voxelSize, ev0, ev1, ev2)) {
                        m_surfaceSticks.insert(std::make_pair(voxid, voxid + VOX_SAMPLES));
                        m_surfaceSticks.insert(std::make_pair(voxid + VOX_SAMPLES, voxid));
                        if (pointInPositiveHalfSpace(svoxel, ev0, ev1, ev2)) {
                            m_positivePoints.insert(voxid);
                        }
                        else {
                            m_positivePoints.insert(voxid + VOX_SAMPLES);
                        }
                    }
                    if (k < VOX_SAMPLES - 1 && segmentTriangleIntersection(svoxel, svoxel + glm::vec3(0, 0, 1)*m_voxelSize, ev0, ev1, ev2)) {
                        m_surfaceSticks.insert(std::make_pair(voxid, voxid + 1));
                        m_surfaceSticks.insert(std::make_pair(voxid + 1, voxid));
                        if (pointInPositiveHalfSpace(svoxel, ev0, ev1, ev2)) {
                            m_positivePoints.insert(voxid);
                        }
                        else {
                            m_positivePoints.insert(voxid + 1);
                        }
                    }
                }
            }
        }
    }

    propagateGridValues(distanceGrid, m_voxelSize);
}


void HoleRepairer::computeUDFhole(Grid3D<float>* distanceGrid)
{
    int numHoles = m_mesh->getNumHoles();
    glm::vec3 bbmin = m_boundingBox.limits[0];

    for (int h = 0; h < numHoles; h++) {
        const std::vector<int>& hole = m_mesh->getHole(h);
        for (unsigned int i = 0; i < hole.size(); i++) {

            int iv1 = hole[i];
            int iv2 = hole[(i+1)%hole.size()];
            const glm::vec3& v0 = m_mesh->getVertex(iv1);
            const glm::vec3& v1 = m_mesh->getVertex(iv2);

            glm::vec3 c0 = (v0 - bbmin)/m_voxelSize;
            glm::vec3 c1 = (v1 - bbmin)/m_voxelSize;

            int pad = 2;
            int imin = glm::clamp(int(std::min(c0.x, c1.x)) - pad, 0, VOX_SAMPLES-1);
            int jmin = glm::clamp(int(std::min(c0.y, c1.y)) - pad, 0, VOX_SAMPLES-1);
            int kmin = glm::clamp(int(std::min(c0.z, c1.z)) - pad, 0, VOX_SAMPLES-1);
            int imax = glm::clamp(int(std::max(c0.x, c1.x)) + pad, 0, VOX_SAMPLES-1);
            int jmax = glm::clamp(int(std::max(c0.y, c1.y)) + pad, 0, VOX_SAMPLES-1);
            int kmax = glm::clamp(int(std::max(c0.z, c1.z)) + pad, 0, VOX_SAMPLES-1);

            glm::vec3 voxmin(0);
            for (int i = imin; i <= imax; i++) {
                voxmin.x = (i == VOX_SAMPLES-1 ? i-1 : i)*m_voxelSize.x + bbmin.x;
                for (int j = jmin; j <= jmax; j++) {
                    voxmin.y = (j == VOX_SAMPLES-1 ? j-1 : j)*m_voxelSize.y + bbmin.y;
                    for (int k = kmin; k <= kmax; k++) {
                        voxmin.z = (k == VOX_SAMPLES-1 ? k-1 : k)*m_voxelSize.z + bbmin.z;
                        glm::vec3 svoxel = glm::vec3(i, j, k)*m_voxelSize + bbmin;
                        float dist_edge = pointToSegmentUnsignedDist(svoxel, v0, v1);
                        float dist_vtx  = glm::length(svoxel - v0);
                        float dist = std::min(dist_vtx, dist_edge);
                        float fval = distanceGrid->get(i, j, k);
                        if (fval > dist + 1e-3) {
                            distanceGrid->set(i, j, k, dist);
                        }
                    }
                }
            }
        }
    }

    propagateGridValues(distanceGrid, m_voxelSize);
}


void floodRegion(Grid3D<int>* grid, const Grid3D<bool>* mask, const std::set<std::pair<int, int> >& surfaceSticks, int i0, int j0, int k0, int rid)
{
    int WIDTH  = grid->getWidth();
    int HEIGHT = grid->getHeight();
    int DEPTH  = grid->getDepth();

    grid->set(i0, j0, k0, rid);
    std::queue<int> Q;
    Q.push(i0*HEIGHT*DEPTH + j0*DEPTH + k0);

    while (!Q.empty()) {

        int voxid = Q.front();
        Q.pop();

        int i =  voxid/(HEIGHT*DEPTH);
        int j = (voxid%(HEIGHT*DEPTH))/DEPTH;
        int k = (voxid%(HEIGHT*DEPTH))%DEPTH;

        const int sticks[6][3] = { {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1} };
        for (int ns = 0; ns < 6; ns++) {
            int di = sticks[ns][0];
            int dj = sticks[ns][1];
            int dk = sticks[ns][2];
            if (i + di < 0 || i + di >= WIDTH) continue;
            if (j + dj < 0 || j + dj >= HEIGHT) continue;
            if (k + dk < 0 || k + dk >= DEPTH) continue;

            int destid = (i+di)*HEIGHT*DEPTH + (j+dj)*DEPTH + k+dk;
            if (!mask->get(i+di, j+dj, k+dk) && grid->get(i+di, j+dj, k+dk) == 0
                && surfaceSticks.find(std::make_pair(voxid, destid)) == surfaceSticks.end())
            {
                grid->set(i+di, j+dj, k+dk, rid);
                Q.push(destid);
            }
        }
    }
}

void findSign(const Grid3D<int>* grid, const std::vector<bool>& setSign, const std::vector<bool>& posRegion,
              const std::set<std::pair<int, int> >& surfaceSticks, int i0, int j0, int k0, bool& positive)
{
    int WIDTH  = grid->getWidth();
    int HEIGHT = grid->getHeight();
    int DEPTH  = grid->getDepth();

    Grid3D<bool> visited(WIDTH, HEIGHT, DEPTH, false);

    int rid = grid->get(i0, j0, k0);
    std::queue<int> Q;
    visited.set(i0, j0, k0, true);
    Q.push(i0*HEIGHT*DEPTH + j0*DEPTH + k0);

    while (!Q.empty()) {
        int voxid = Q.front();
        Q.pop();
        int i =  voxid/(HEIGHT*DEPTH);
        int j = (voxid%(HEIGHT*DEPTH))/DEPTH;
        int k = (voxid%(HEIGHT*DEPTH))%DEPTH;

        const int sticks[6][3] = { {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1} };
        for (int ns = 0; ns < 6; ns++) {
            int di = sticks[ns][0];
            int dj = sticks[ns][1];
            int dk = sticks[ns][2];
            if (i + di < 0 || i + di >= WIDTH)  continue;
            if (j + dj < 0 || j + dj >= HEIGHT) continue;
            if (k + dk < 0 || k + dk >= DEPTH)  continue;

            int destid = (i+di)*HEIGHT*DEPTH + (j+dj)*DEPTH + k+dk;
            if (surfaceSticks.find(std::make_pair(voxid, destid)) != surfaceSticks.end()) continue;

            int drid = grid->get(i+di, j+dj, k+dk);
            if (drid > 0 && drid != rid && setSign[drid]) {
                positive = posRegion[drid];
                return;
            }
            else if (!visited.get(i+di, j+dj, k+dk)) {
                visited.set(i+di, j+dj, k+dk, true);
                Q.push(destid);
            }
        }
    }

    positive = false;
}


void HoleRepairer::computeSignedDistanceField()
{
    // distance field from the mesh
    std::cout << "Computing mesh distance field..." << std::endl;
    computeUDFmesh(&m_distanceGrid);

    // distance field from the hole
    std::cout << "Computing holes distance field..." << std::endl;
    computeUDFhole(&m_holeDistance);

    // compute the mask and final distance field
    std::cout << "Computing mask..." << std::endl;
    for (int i = 0; i < VOX_SAMPLES; i++) {
        for (int j = 0; j < VOX_SAMPLES; j++) {
            for (int k = 0; k < VOX_SAMPLES; k++) {
                float mdist = m_distanceGrid.get(i, j, k);
                float hdist = m_holeDistance.get(i, j, k);
                if (hdist < mdist + 0.5*glm::length(m_voxelSize)) {
                    m_triangleMask.set(i, j, k, true);
                    m_distanceGrid.set(i, j, k, mdist);
                }
                else {
                    m_triangleMask.set(i, j, k, false);
                    m_distanceGrid.set(i, j, k, mdist);
                }
            }
        }
    }

    std::cout << "Generating signed distance field..." << std::endl;

    // compute regions in the mask to propagate the sign
    Grid3D<int> regionGrid(VOX_SAMPLES, 0);
    int numRegions = 0;
    bool foundRegion = true;
    while (foundRegion) {
        foundRegion = false;
        for (int i = 0; i < VOX_SAMPLES; i++) {
            for (int j = 0; j < VOX_SAMPLES; j++) {
                for (int k = 0; k < VOX_SAMPLES; k++) {
                    if (!m_triangleMask.get(i, j, k) && regionGrid.get(i, j, k) == 0) {
                        numRegions++;
                        foundRegion = true;
                        floodRegion(&regionGrid, &m_triangleMask, m_surfaceSticks, i, j, k, numRegions);
                    }
                }
            }
        }
    }

    // votes based on sticks
    std::vector<int> positiveVotes(numRegions+1, 0);
    std::vector<int> negativeVotes(numRegions+1, 0);
    for (std::set<std::pair<int, int> >::const_iterator it = m_surfaceSticks.begin(); it != m_surfaceSticks.end(); it++) {
        int v1id = it->first;
        int v2id = it->second;
        int i1 =  v1id/(VOX_SAMPLES*VOX_SAMPLES);
        int j1 = (v1id%(VOX_SAMPLES*VOX_SAMPLES))/VOX_SAMPLES;
        int k1 = (v1id%(VOX_SAMPLES*VOX_SAMPLES))%VOX_SAMPLES;
        int i2 =  v2id/(VOX_SAMPLES*VOX_SAMPLES);
        int j2 = (v2id%(VOX_SAMPLES*VOX_SAMPLES))/VOX_SAMPLES;
        int k2 = (v2id%(VOX_SAMPLES*VOX_SAMPLES))%VOX_SAMPLES;
        int r1 = regionGrid.get(i1, j1, k1);
        int r2 = regionGrid.get(i2, j2, k2);

        // if both endpoints are marked as positive, this is not a safe sign sample
        if (m_positivePoints.find(v1id) != m_positivePoints.end() && m_positivePoints.find(v2id) == m_positivePoints.end()) {
            positiveVotes[r1]++;
            negativeVotes[r2]++;
        }
        else if (m_positivePoints.find(v2id) != m_positivePoints.end() && m_positivePoints.find(v1id) == m_positivePoints.end()) {
            positiveVotes[r2]++;
            negativeVotes[r1]++;
        }
    }

    // define sign to regions according to votes
    std::vector<bool> setSign(numRegions+1, false);
    std::vector<bool> posRegion(numRegions+1, false);
    for (int i = 1; i <= numRegions; i++) {
        if (positiveVotes[i] != negativeVotes[i]) {
            setSign[i] = true;
            posRegion[i] = positiveVotes[i] > negativeVotes[i];
        }
    }

    // assign sign
    for (int i = 0; i < VOX_SAMPLES; i++) {
        for (int j = 0; j < VOX_SAMPLES; j++) {
            for (int k = 0; k < VOX_SAMPLES; k++) {
                int rid = regionGrid.get(i, j, k);
                if (rid > 0) {
                    float dist = m_distanceGrid.get(i, j, k);

                    if (!setSign[rid]) {
                        bool positive;
                        findSign(&regionGrid, setSign, posRegion, m_surfaceSticks, i, j, k, positive);
                        posRegion[rid] = positive;
                        setSign[rid] = true;
                    }

                    if (!posRegion[rid]) {
                        m_distanceGrid.set(i, j, k, -dist);
                    }
                }
            }
        }
    }
}


void HoleRepairer::extractIsosurface()
{
    std::vector<double> mcvertexs;
    std::vector<double> mcnormals;
    std::vector<unsigned int> mcfaces;

    MarchingCubes mc(&m_distanceGrid, m_voxelSize, m_boundingBox.limits[0]);
    mc.Initialize(RangValors(2*m_maxDistanceNegative, 0), 1, 0, 1, IJK(VOX_SAMPLES, VOX_SAMPLES, VOX_SAMPLES), false, false);
    mc.Run(mcvertexs, mcnormals, mcfaces);

    std::vector<glm::vec3> vertices;
    std::vector<int>  vtable;
    for (unsigned int i = 0; i < mcvertexs.size(); i+= 3) {
        vertices.push_back(glm::vec3(mcvertexs[i], mcvertexs[i+1], mcvertexs[i+2]));
    }
    for (unsigned int i = 0; i < mcfaces.size(); i++) {
        vtable.push_back(mcfaces[i]);
    }
    m_outMesh = new TriangleMesh();
    m_outMesh->setVertices(vertices, vtable);
    m_outMesh->computeNormalsPerFace();
    m_outMesh->computeBoundingBox();

    std::cout << "    verts: " << mcvertexs.size()/3 << std::endl;
    std::cout << "    faces: " << mcfaces.size()/3 << std::endl;
}
