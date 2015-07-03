#ifndef GRID3D_H
#define GRID3D_H


template <class T>
class Grid3D
{

public:

    Grid3D() {
        Nx = Ny = Nz = 0;
        grid = 0;
    }

    Grid3D(unsigned int n) {
        Nx = n;
        Ny = n;
        Nz = n;
        grid = new T[Nx*Ny*Nz];
    }

    Grid3D(unsigned int n, const T& val) {
        Nx = n;
        Ny = n;
        Nz = n;
        grid = new T[Nx*Ny*Nz];
        fill(val);
    }

    Grid3D(unsigned int nx, unsigned int ny, unsigned int nz) {
        Nx = nx;
        Ny = ny;
        Nz = nz;
        grid = new T[Nx*Ny*Nz];
    }

    Grid3D(unsigned int nx, unsigned int ny, unsigned int nz, const T& val) {
        Nx = nx;
        Ny = ny;
        Nz = nz;
        grid = new T[Nx*Ny*Nz];
        fill(val);
    }

    ~Grid3D() {
        if (grid) delete[] grid;
    }

    T& get(int x, int y, int z) {
        return grid[x*Ny*Nz + y*Nz + z];
    }

    const T& get(int x, int y, int z) const {
        return grid[x*Ny*Nz + y*Nz + z];
    }

    void set(int x, int y, int z, const T& val) {
        grid[x*Ny*Nz + y*Nz + z] = val;
    }

    unsigned int getWidth() const {
        return Nx;
    }

    unsigned int getHeight() const {
        return Ny;
    }

    unsigned int getDepth() const {
        return Nz;
    }

    void fill(const T& val) {
        for (unsigned int x = 0; x < Nx; x++) {
            for (unsigned int y = 0; y < Ny; y++) {
                for (unsigned int z = 0; z < Nz; z++) {
                    grid[x*Ny*Nz + y*Nz + z] = val;
                }
            }
        }
    }


private:

    T* grid;
    unsigned int Nx, Ny, Nz;

};


#endif
