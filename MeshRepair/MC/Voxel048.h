#ifndef __Voxel048_h__
#define __Voxel048_h__


#include "MC/IJK.h"
#include "glm/glm.hpp"


class Voxel048{

  IJK IJKVoxel048;
  float PuntAresta0,PuntAresta4,PuntAresta8;
  int Posicio0_iv,Posicio4_iv,Posicio8_iv;
  glm::vec3 Grad0, Grad4, Grad8;
    friend class MarchingCubes;
  
public:
  Voxel048(){

    IJKVoxel048.i=-1; IJKVoxel048.j=-1; IJKVoxel048.k=-1;
  }
  Voxel048(const IJK &IJKVoxel048i){
    IJKVoxel048=IJKVoxel048i;
    PuntAresta0=-1.0;PuntAresta4=-1.0;PuntAresta8=-1.0;
    Posicio0_iv=-1;Posicio4_iv=-1;Posicio8_iv=-1; 
  }
    Voxel048(const Voxel048 &vi )
	{
	    IJKVoxel048=vi.IJKVoxel048;
	    PuntAresta0=vi.PuntAresta0;
	PuntAresta4=vi.PuntAresta4;
	PuntAresta8=vi.PuntAresta8;
	Posicio0_iv=vi.Posicio0_iv;
	Posicio4_iv=vi.Posicio4_iv;
	Posicio8_iv=vi.Posicio8_iv;
    }
    Voxel048& operator =(const Voxel048 &vi ){
	IJKVoxel048=vi.IJKVoxel048;
	PuntAresta0=vi.PuntAresta0;
	PuntAresta4=vi.PuntAresta4;
    PuntAresta8=vi.PuntAresta8;
    Posicio0_iv=vi.Posicio0_iv;
    Posicio4_iv=vi.Posicio4_iv;
    Posicio8_iv=vi.Posicio8_iv;
    return *this;
  }
  void SetPosicio_iv(int num_aresta,int postaulaVertexs);
  float GetPuntAresta(int num_aresta);
  int GetPosicio_iv(int num_aresta);
  IJK GetIJKvoxel();
  void SetIJKvoxel(IJK ijk);
  glm::vec3 RetornaPunt(int num_aresta, const glm::vec3 &origen, const glm::vec3 &);
  glm::vec3 RetornaGradient(int num_aresta);
  int Valid();
};

class pVoxel048 {
public:
  Voxel048 *v;

  pVoxel048() { v=NULL; }
  ~pVoxel048() { if (v!=NULL) delete v; v=NULL; }
  bool Valid() { return v!=NULL; }
};


#endif


