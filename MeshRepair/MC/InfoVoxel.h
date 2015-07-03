#ifndef INFOVOXEL_H_
#define INFOVOXEL_H_

#include <list>

#define VOXEL_EXTERIOR 0
#define VOXEL_SUPERFICIE 1 
#define VOXEL_INTERIOR 2
#define VOXEL_INTERSECADO 3
#define VOXEL_BIINTERSECADO 4
#define VOXEL_SOBREINTERSECADO 5

#define NO_CORTA -1
#define VISITADO -2 



typedef struct{
    int x;
    int y;
    int z;
}aristaXYZ;



class InfoVoxel
{
public:
    //constructora
    InfoVoxel();
    
    //destructora
	~InfoVoxel();

    //consultoras
    unsigned char getState();
    bool initMarchingSticksQ();
    bool initPlaneSticksQ();
    std::list<unsigned int>::iterator getBeginTriangles();
    std::list<unsigned int>::iterator getEndTriangles();
    int getMarchingSticks(int aresta);
    int getPlaneSticks(int aresta);

    //modificadoras
    void setState(unsigned char s);
    void pushTriangle(unsigned int t);
    void initTriangles();
    void clearTriangles();
    void initMarchingSticks();
    void initPlaneSticks();
    void setMarchingSticks(int aresta, int pos);
    void setPlaneSticks(int aresta, int pos);

private:

    unsigned char state;
    std::list<unsigned int> *triangles;  
    aristaXYZ *marching;
    aristaXYZ *plane;




};
#endif /*INFOVOXEL_H_*/
