#include "MC/Voxel048.h"
#include "MC/Label.h"
#include <iostream>


float Voxel048::GetPuntAresta(int num_aresta)
{
  float PuntAresta;

  switch(num_aresta){
      case 0:
             PuntAresta=PuntAresta0;
         break;
      case 4:
             PuntAresta=PuntAresta4;
         break;
      case 8:
             PuntAresta=PuntAresta8;
         break;
      default:
            PuntAresta=-1;
         break;
  }
  return (PuntAresta);
}

int Voxel048::GetPosicio_iv(int num_aresta)
{
  int Posicio_iv;

  switch(num_aresta){
  case 0:
         Posicio_iv=Posicio0_iv;
         break;          
  case 4:
         Posicio_iv=Posicio4_iv;
         break; 
  case 8:
         Posicio_iv=Posicio8_iv;
         break;
  default:
         Posicio_iv=-1;
         break;
  }
  return (Posicio_iv);
}




void Voxel048::SetPosicio_iv(int num_aresta,int postaulaVertexs)
{ 
  switch(num_aresta){
  case 0:
         Posicio0_iv=postaulaVertexs;
	 break;          
  case 4:
         Posicio4_iv=postaulaVertexs;
	 break; 
  case 8:
         Posicio8_iv=postaulaVertexs;
	 break;          
  }
}


glm::vec3 Voxel048::RetornaPunt(int num_aresta, const glm::vec3 &origen, const glm::vec3 &dim_aresta_voxel)
{
  glm::vec3 P(0,0,0);
 // LGPt3d Po (0,0,0);
  switch(num_aresta)
    {
    case 0:
           P[0]=IJKVoxel048.i * dim_aresta_voxel.x+PuntAresta0;
	   P[1]=IJKVoxel048.j * dim_aresta_voxel.y;
	   //P.coord[2]=origen.z();//IJKVoxel048.k * dim_aresta_voxel.z; 
	   //Cal sumar-li l'origen del MonVoxels
	   P += origen;
	   /*Po.coord[0]=IJKVoxel048.i * dim_aresta_voxel.x+PuntAresta0;
	   Po.coord[1]=IJKVoxel048.j * dim_aresta_voxel.y;
	   Po.coord[2]=IJKVoxel048.k * dim_aresta_voxel.z;
	   Po += origenm;*/
	   
	   break;          
    case 4:
           P[0]=IJKVoxel048.i*dim_aresta_voxel.x;
	   P[1]=IJKVoxel048.j*dim_aresta_voxel.y+PuntAresta4;
	   //P.coord[2]=IJKVoxel048.k*dim_aresta_voxel.z; 
	   //Cal sumar-li l'origen del MonVoxels
	   P += origen;
	   /*
	   Po.coord[0]=IJKVoxel048.i*dim_aresta_voxel.x;
	   Po.coord[1]=IJKVoxel048.j*dim_aresta_voxel.y+PuntAresta4;
	   Po.coord[2]=IJKVoxel048.k*dim_aresta_voxel.z;
	   Po += origenm;*/
	   
	   break;           
    case 8:
           P[0]=IJKVoxel048.i*dim_aresta_voxel.x;
	   P[1]=IJKVoxel048.j*dim_aresta_voxel.y;
	   P[2]=PuntAresta8; 
	   //Cal sumar-li l'origen del MonVoxels
	   P += origen;
	  /* Po.coord[0]=IJKVoxel048.i*dim_aresta_voxel.x;
	   Po.coord[1]=IJKVoxel048.j*dim_aresta_voxel.y;
	   Po.coord[2]=IJKVoxel048.k*dim_aresta_voxel.z+PuntAresta8;
	   Po += origenm;*/
	  /* cerr<<"P : "<<P<<endl;
	   cerr<<"Po: "<<Po<<endl;*/
	   break;           
    default: 
           std::cout << "RetornaPunt aresta desconeguda";
	   exit(-1);   
    }
  /*
  if(!(P==Po))
    {
      cerr<<"P = "<<P<<" Po = "<<Po<<" origen "<<origen<<" num_aresta "<<num_aresta<<endl;
    }
*/
  return(P);
}

glm::vec3 Voxel048::RetornaGradient(int num_aresta)
{
//    cerr<<"Retorna gradient"<<endl;
    glm::vec3 v;
    
    switch (num_aresta)
    {
    case 0: v = Grad0; break;
    case 4: v = Grad4; break;
    case 8: v = Grad8; break;
    default: 
    std::cout << "RetornaPunt aresta desconeguda";
	exit(-1);   
    }
    return v;
}

IJK Voxel048::GetIJKvoxel()
{
    return IJKVoxel048;
}

void Voxel048::SetIJKvoxel(IJK ijk)
{
	IJKVoxel048 = ijk;
    PuntAresta0=-1.0;PuntAresta4=-1.0;PuntAresta8=-1.0;
    Posicio0_iv=-1;Posicio4_iv=-1;Posicio8_iv=-1; 
}

int Voxel048::Valid()
{
    return (IJKVoxel048.i!=-1);
}
