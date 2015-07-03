# Mesh Repair --- README

This repository holds the source code (in C++) for the repair isosurface construction introduced in [1][]. We 
also include some example meshes (with defects) to test it.

## INSTALL
To compile and link the source code you will need [cmake][], [glm][] and [eigen][]. The <code>CMakeLists</code> is very straightforward, and requires that you set environment variables pointing to the directories containing the headers of [glm][] and [eigen][]. For example

``` sh
export EIGEN3_INCLUDE_DIR=/usr/include/eigen3
export GLM_INCLUDE_DIR=/usr/include
```

Then in the usual way, do, if MeshRepair is the directory into which you've cloned this repository,

``` sh
cd MeshRepair
mkdir build
cd build
cmake ..
make
```

to build the binary.

You can then run it as

```
./MeshRepair <inFile.ply> <outFile.ply> [--border] [--numVoxels <N>]
```

where --border indicates the mesh is not a complete closed object (aside from the holes), and --numVoxels indicates
the size of the voxelization used, which should be chosen so that the voxels are of the same order of magnitude as the edges of the triangles in the mesh. if no voxelization size is selected, the value defaults to 128.

## Example files

we provide four sample holes from the *Portalada* [2][], as test data, in files <code>test1.ply</code>, <code>test2.ply</code>, <code>test3.ply</code> and <code>test4.ply</code>. Recommended parameters for our test examples are
``` sh
./MeshRepair test1.ply test1-out.ply --border --numVoxels 64 
./MeshRepair test2.ply test2-out.ply --border
./MeshRepair test3.ply test3-out.ply --border
./MeshRepair test4.ply test4-out.ply --border
```


References:

[1][]: O. Argudo, P. Brunet, A. Chica and À. Vinacua: "Biharmonic fields and mesh completion", *Graphical Models* (2015), http://dx.doi.org/10.1016/j.gmod.2015.06.010

[2][]: M.Callieri, A. Chica, M. Dellepiane, I. Besora, M. Corsini, J. Moyés, G. Ranzuglia, R. Scopigno and P. Brunet: "Multiscale acquisitionand presentation of very large artifacts: the case of portalada", *ACM J. Comut. Cult. Herit.* **3**(4) (2011) 14:1--14:20

[1]: http://dx.doi.org/10.1016/j.gmod.2015.06.010 "Biharmonic fields and mesh completion"
[cmake]: http://www.cmake.org "Cmake home page"
[glm]: http://glm.g-truc.net "glm home page"
[eigen]: http://eigen.tuxfamily.org "eigen home page"
[2]: http://dx.doi.org/10.1145/1957825.1957827 "Acquisition Portalada"
