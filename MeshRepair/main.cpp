#include "trianglemesh.h"
#include "holerepairer.h"
#include <iostream>
#include <string>
#include <sstream>


void printUsage()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "  MeshRepair <inFile.ply> <outFile.ply> [--border] [--numVoxels <N>]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --border" << "\t" << "Treat longest edge loop as mesh border, not as a hole" << std::endl;
    std::cout << "  --numVoxels <N>" << "\t" << "Use a grid of N^3 voxels (default: 128)" << std::endl;
}



bool parseInput(int argc, char *argv[],
                std::string& inFileName,
                std::string& outFileName,
                bool& borders,
                int& numVoxels)
{
    if (argc < 3) {
        return false;
    }

    inFileName = argv[1];
    outFileName = argv[2];
    borders = false;
    numVoxels = 128;

    int i = 3;
    while (i < argc) {
        if (std::string(argv[i]) == "--border") {
            borders = true;
            i++;
        }
        else if (std::string(argv[i]) == "--numVoxels" && i+1 < argc) {
            std::stringstream ss(argv[i+1]);
			if (!(ss >> numVoxels)) {
                return false;
            }
            i += 2;
        }
        else {
            return false;
        }
    }

    return true;
}



int main(int argc, char *argv[])
{
    std::string inFileName;
    std::string outFileName;
    bool hasBorder;
    int numVoxels;

    if (!parseInput(argc, argv, inFileName, outFileName, hasBorder, numVoxels)) {
        std::cout << "Invalid or insufficient parameters. See usage below:" << std::endl << std::endl;
        printUsage();
        return 0;
    }

    TriangleMesh* meshOriginal = new TriangleMesh();
    bool loadedOK = meshOriginal->loadPLY(inFileName);
    if (loadedOK) {
        meshOriginal->setHasBorder(hasBorder);

        HoleRepairer* holeRepairer = new HoleRepairer(numVoxels);
        holeRepairer->setInputMesh(meshOriginal);
        holeRepairer->execute();

        TriangleMesh* meshRepaired = holeRepairer->getResultMesh();
        meshRepaired->savePLY(outFileName);

        delete holeRepairer;
        delete meshOriginal;
        delete meshRepaired;
    }
    else {
        std::cout << "Could not load input mesh: " << std::string(inFileName) << std::endl;
    }
}
