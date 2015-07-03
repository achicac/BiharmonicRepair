#include "MC/InfoVoxel.h"
#include <iostream>

using namespace std;

//constructora
InfoVoxel::InfoVoxel()
{
    state = VOXEL_EXTERIOR;
    triangles = 0;
    marching = 0;
    plane = 0;
}
    
//destructora
InfoVoxel::~InfoVoxel()
{
    if(triangles != 0) delete triangles;
    if(marching != 0) delete marching;
    if(plane != 0) delete plane;
}

//consultora
unsigned char InfoVoxel::getState(){return state;}
bool InfoVoxel::initMarchingSticksQ(){return (marching!=0);}
bool InfoVoxel::initPlaneSticksQ(){return (plane!=0);}
list<unsigned int>::iterator InfoVoxel::getBeginTriangles(){return triangles->begin();}
list<unsigned int>::iterator InfoVoxel::getEndTriangles(){return triangles->end();}
int InfoVoxel::getMarchingSticks(int aresta)
{
    switch(aresta)
    {
        case 0:
            return marching->x;
        case 4:
            return marching->y;
        case 8:
            return marching->z;
        default:
            std::cerr << "Error: ARESTA NO EXISTENT" << std::endl;
			return -1;
        break;
    }
}
int InfoVoxel::getPlaneSticks(int aresta)
{
    switch(aresta)
    {
        case 0:
            return plane->x;
        case 4:
            return plane->y;
        case 8:
            return plane->z;
        default:
            std::cerr << "Error: ARESTA NO EXISTENT" << std::endl;
			return -1;
        break;
    }
}


//modificadoras
void InfoVoxel::setState(unsigned char s){state = s;}
void InfoVoxel::pushTriangle(unsigned int t){triangles->push_back(t);}
void InfoVoxel::initTriangles()
{
    //quitar algun dia la comporobacion esta
    if(triangles != 0) std::cerr << "ERROR: me llega un puntero a triangulo que no es nulo" << std::endl;
    triangles = new list<unsigned int>();
}
void InfoVoxel::clearTriangles(){triangles->clear();}
void InfoVoxel::initMarchingSticks()
{
    marching = new aristaXYZ();
    marching-> x = NO_CORTA;
    marching-> y = NO_CORTA;
    marching-> z = NO_CORTA;

}
void InfoVoxel::initPlaneSticks()
{
    plane = new aristaXYZ();
    plane-> x = NO_CORTA;
    plane-> y = NO_CORTA;
    plane-> z = NO_CORTA;

}
void InfoVoxel::setMarchingSticks(int aresta, int pos)
{
    switch(aresta)
    {
        case 0:
            marching->x = pos;
        break;
        case 4:
            marching->y = pos;
        break;
        case 8:
            marching->z = pos;
        break;
        default:
            std::cerr << "Error: ARESTA NO EXISTENT" << std::endl;
        break;
    }
}
void InfoVoxel::setPlaneSticks(int aresta, int pos)
{
    switch(aresta)
    {
        case 0:
            plane->x = pos;
        break;
        case 4:
            plane->y = pos;
        break;
        case 8:
            plane->z = pos;
        break;
        default:
            std::cerr << "Error: ARESTA NO EXISTENT" << std::endl;
        break;
    }
}
