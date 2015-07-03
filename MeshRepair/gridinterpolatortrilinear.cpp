#include "gridinterpolatortrilinear.h"
#include <algorithm>


GridInterpolatorTrilinear::GridInterpolatorTrilinear(const Grid3D<float> *g) : GridInterpolator(g)
{
    Nx = m_base->getWidth();
    Ny = m_base->getHeight();
    Nz = m_base->getDepth();
}

float GridInterpolatorTrilinear::evaluate(float x, float y, float z) const
{
    float px = x*Nx;
    float py = y*Ny;
    float pz = z*Nz;

    int x0 = std::min(std::max(int(px), 0), Nx - 1);
    int x1 = std::min(x0 + 1, Nx - 1);
    float xfrac = px - x0;
    int y0 = std::min(std::max(int(py), 0), Ny - 1);
    int y1 = std::min(y0 + 1, Ny - 1);
    float yfrac = py - y0;
    int z0 = std::min(std::max(int(pz), 0), Nz - 1);
    int z1 = std::min(z0 + 1, Nz - 1);
    float zfrac = pz - z0;

    float v00 = (1 - xfrac)*m_base->get(x0, y0, z0) + xfrac*m_base->get(x1, y0, z0);
    float v01 = (1 - xfrac)*m_base->get(x0, y0, z1) + xfrac*m_base->get(x1, y0, z1);
    float v10 = (1 - xfrac)*m_base->get(x0, y1, z0) + xfrac*m_base->get(x1, y1, z0);
    float v11 = (1 - xfrac)*m_base->get(x0, y1, z1) + xfrac*m_base->get(x1, y1, z1);
    float v0  = (1 - yfrac)*v00 + yfrac*v10;
    float v1  = (1 - yfrac)*v01 + yfrac*v11;
    return (1 - zfrac)*v0 + zfrac*v1;
}
