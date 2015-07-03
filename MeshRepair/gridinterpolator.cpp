#include "gridinterpolator.h"


GridInterpolator::GridInterpolator(const Grid3D<float>* g) {
    m_base = g;
    Nx = m_base->getWidth();
    Ny = m_base->getHeight();
    Nz = m_base->getDepth();
}


Grid3D<float>* GridInterpolator::getScaledGrid(int sx, int sy, int sz)
{
    Grid3D<float>* res = new Grid3D<float>(sx, sy, sz);
    for (int i = 0; i < sx; i++) {
        float x = float(i)/float(sx);
        for (int j = 0; j < sy; j++) {
            float y = float(j)/float(sy);
            for (int k = 0; k < sz; k++) {
                float z = float(k)/float(sz);
                res->set(i, j, k, this->evaluate(x, y, z));
            }
        }
    }

    return res;
}

