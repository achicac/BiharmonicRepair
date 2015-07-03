#ifndef GRID3DLODOD_H
#define GRID3DLODOD_H

#include "grid3D.h"

template <class T>
class Grid3DLOD
{

public:

    Grid3DLOD(Grid3D<T>* g, unsigned int lod = 0) {
        grid = g;
        LOD  = lod;
    }

    T get(int x, int y, int z) const {
        return grid->get(x << LOD, y << LOD, z << LOD);
    }

    void set(int x, int y, int z, T val) {
        grid->set(x << LOD, y << LOD, z << LOD, val);
    }

    unsigned int getWidth() const {
        return grid->getWidth() >> LOD;
    }

    unsigned int getHeight() const {
        return grid->getHeight() >> LOD;
    }

    unsigned int getDepth() const {
        return grid->getDepth() >> LOD;
    }

    Grid3D<T>* getGrid() const {
        Grid3D<T>* g = new Grid3D<T>(this->getWidth(), this->getHeight(), this->getDepth());
        for (unsigned int i = 0; i < this->getWidth(); i++) {
            for (unsigned int j = 0; j < this->getHeight(); j++) {
                for (unsigned int k = 0; k < this->getDepth(); k++) {
                    g->set(i, j, k, this->get(i, j, k));
                }
            }
        }
        return g;
    }

private:

    Grid3D<T>* grid;
    unsigned int LOD;

};


#endif // GRID3DLODOD_H
