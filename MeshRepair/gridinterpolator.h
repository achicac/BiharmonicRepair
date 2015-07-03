#ifndef GRIDINTERPOLATOR_H
#define GRIDINTERPOLATOR_H

#include "grid3D.h"


class GridInterpolator
{
public:

    GridInterpolator(const Grid3D<float>* g);

    virtual float           evaluate(float x, float y, float z) const = 0; // domain [0,1]^3
    virtual Grid3D<float>*  getScaledGrid(int sx, int sy, int sz);

protected:

    const Grid3D<float>*  m_base;
    int Nx;
    int Ny;
    int Nz;

};

#endif // GRIDINTERPOLATOR_H
