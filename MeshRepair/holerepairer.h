#ifndef HOLEREPAIRER_H
#define HOLEREPAIRER_H

#include "bbox.h"
#include "grid3D.h"
#include "trianglemesh.h"
#include <set>
#include <utility>


class HoleRepairer
{
public:

    HoleRepairer(int voxelSamples);
    ~HoleRepairer();

    void execute();
    void setInputMesh(TriangleMesh* mesh);
    TriangleMesh* getResultMesh() const;

private:

    void prepareGrid();
    void expandBorderTriangles();
    void recoverBorderTriangles();

    void computeUDFmesh(Grid3D<float>* distanceGrid);
    void computeUDFhole(Grid3D<float>* distanceGrid);
    void computeSignedDistanceField();
    void extractIsosurface();

private:

    int NUM_VOXELS;
    int VOX_SAMPLES;

    TriangleMesh*  m_mesh;
    TriangleMesh*  m_outMesh;

    std::vector<vec3f> m_originalBorder;
    BBox               m_boundingBox;
    glm::vec3          m_voxelSize;

    Grid3D<float>  m_distanceGrid;
    Grid3D<bool>   m_triangleMask;
    Grid3D<float>  m_holeDistance;

    std::set<std::pair<int, int> > m_surfaceSticks;
    std::set<int>                  m_positivePoints;

    float  m_maxDistanceNegative;
    float  m_minDistanceNegative;
    float  m_maxDistancePositive;
    float  m_minDistancePositive;
};

inline void HoleRepairer::setInputMesh(TriangleMesh *mesh)
{
    m_mesh = mesh;
}

inline TriangleMesh* HoleRepairer::getResultMesh() const
{
    return m_outMesh;
}

#endif // HOLEREPAIRER_H
