#ifndef GRIDINTERPOLATORTRILINEAR_H
#define GRIDINTERPOLATORTRILINEAR_H

#include "gridinterpolator.h"


class GridInterpolatorTrilinear : public GridInterpolator
{

public:

    GridInterpolatorTrilinear(const Grid3D<float>* g);

    virtual float evaluate(float x, float y, float z) const;
};

#endif // GRIDINTERPOLATORTRILINEAR_H
