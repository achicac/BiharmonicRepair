#ifndef __MARCHING_CUBES_H__
#define __MARCHING_CUBES_H__

#include <vector>
#include <list>

#include "MC/Voxel048.h"
#include "MC/Label.h"
#include "MC/InfoVoxel.h"
#include "grid3D.h"


struct RangValors {
    float max, min;
    RangValors () {}
    RangValors (float a, float b) {min = a; max = b;}
};
typedef bool funcioEsSel (class Captacio * cap, const IJK &, void *param);
typedef void FuncioOmpleF4V(Captacio &aCaptacio, const IJK &id, std::vector<unsigned int> &cares, void *param);
 
/**

MarchingCubes

*/


class MarchingCubes {
  public:
   MarchingCubes ();
  ///Constructor a partir d'una grid
   MarchingCubes (const Grid3D<float> *grid, const glm::vec3& voxelSize, const glm::vec3& origin);
  
/** @name Operaciones 
 */

    //@{
     /** Inicializaciones para el algoritmo de MC
    @param th Rango de valores
    @param mth método de interpolación: 0 punto medio   1: interpolación lineal
    @param t_norm: 0 normales en los vértices se calcularán como la media entre las normales de las caras que confluyen en ese vértice
                   1: se utilizará el gradiente en el vértice como normal
    @param resolucio:
    @param color:
    @param tapas: Indica si se va a tapar el modelo
     */
    void Initialize (const RangValors &th, int mth, int t_norm, int t_amb, const IJK &resolucio, bool color = true, bool tapas = true);


    /** Run para hacer el recorrido
    @param vertexs parametro de salida con el conjunto de los vertices
    @param normals parametro de salida con el conjunto de las normales
    @param cares parametro de salida con el conjunto de caras
    @param funcion "omple" para asignar las caras con sus respectivos voxels (en una Mat3D)
    @param parametros necesarios para reailzar la funcion "omple" - Mat3D.
    */
    void Run (std::vector<double> &vertexs, std::vector<double> &normals, std::vector<unsigned int> &cares, FuncioOmpleF4V *omple = 0, void *param = 0);

    /** Recorrido a partir de un conjunto de IJK
      @param llista parametro de entrada con el conjunto de IJK a recorrer
      @param vertexs parametro de salida con el conjunto de los vertices
      @param normals parametro de salida con el conjunto de las normales
      @param cares parametro de salida con el conjunto de caras
      @param f funcion para evaluar si el IJK esta en la lista
      @param param puntero a los parametros necesarios para la funcion f
    */
     void Run (std::vector<double> &vertexs, std::vector<double> &normals, std::vector<unsigned int> &cares, InfoVoxel *voxels_iso);

     void RecorregutLlista(const std::list<IJK> &llista, std::vector<double> &vertexs, std::vector<double> &normals, std::vector<unsigned int> &cares);

  //@}




private:

   void f_rec_Captacio(const Grid3D<float>* captacio, const IJK &id, void *params);


 /**
   @name Operaciones
 */
//@{
  ///ConnectaVertex
   void ConnectaVertexs (const std::vector<int>& tvertexs,int primervertex,int nombrevertexs,int orientacio);

  ///CreaCares
   void CreaCares (int num_patro,int orient, const std::vector<int>& tvertexs, std::vector<unsigned int> *cares);

  ///BuscaPuntInterseccio
   int  BuscaPuntInterseccio (const IJK &IJKvoxel,int num_aresta, std::vector<double> *vertexs, std::vector<double> *normals);
  ///Crea Index
  int CreaIndex (Label Linf,Label Lsup);
   /*vector<int>*/ void CalculaPuntsInterseccio (const std::vector<int>& ArestesInterp, const IJK& IJKvoxel,std::vector<double> *vertexs, std::vector<double> *normals);
  ///ClassificaVoxels
   void ClassificaVoxels (std::vector<double> *vertexs, std::vector<double> *normals, std::vector<unsigned int> *cares, void *param = 0);
  ///OmpleVeins 
  void OmpleVeins (int PosicioLlesca, const IJK &posMon, float ValorProp, bool Estat);
  ///OmpleLlesca
   void OmpleLlesca (float ValorProp, bool Estat, const IJK &posMon, std::vector<double> *vertexs, std::vector<double> *normals, std::vector<unsigned int> *cares, void *param);
  ///CalculaGradient
   void CalculaGradient (Voxel048 &vox, int num_aresta);
  ///SetAresta 
  void SetAresta (Voxel048 &vox, int num_aresta);
  /** A partir de las llesques fija los valores de las labels que usaran SetAresta y CalculaGradient
      @param pvox Voxel048 de las labels a fijar
  */
   void SetLabelsRecorregut(const Voxel048 &vox);
  /**
     Funcion que obtiene el valor y el estado de un ijk
     @param ijk parametro de entrada
     @param val parametro de salida indica el valor del ijk
     @param estat parametro de salida indica si esta seleccionado el ijk
  
  */
   void ObtenirValorEstat(const IJK &ijk, float& val, bool& estat);
  /**
     Funcion que fija las labels que usa SetAresta y CalculaGradient a partir de un Voxel048
  */
  void SetLabelsVoxel(const Voxel048 &vox);
  /** 
      Funcion que fija labels a partir de un ijk para la funcion CreaIndex
  */
  void SetLabelsPatro(const IJK &ijk);

  /**
     RevisioNormal
     Comprobar que la normal calculada a partir de CalculaGradient esta correctamente orientada
     Este test se realiza con el producto escalar de la normal calculada por CalculaGradient y la normal geometrica asociada al triangulo mediante el calculo del producto vectorial.
  
  */
   void RevisioNormal(std::vector<double> *vertexs, std::vector<double> *normals, std::vector<unsigned int> *cares, const IJK &ijk, unsigned int cara_inici);
   
  /**
     Agefir Veins
  */
   void AfegirVeins(const std::vector<int> &arestes, std::list<IJK> &llista, IJK &voxel_actual);
    friend void f_rec_Captacio(Captacio &captacio, const IJK &id, void *params);
    /*vector<int>*/ void BuscaPatro(int index_voxel,int &num_patro,int &orientacio);
  /**
     ini_patrons
  */ 
  void ini_patrons ();
  //float CalcularSeparacioLlesques(int ll0,int ll1);
 
  //@}

 /** @name Miembros
*/
  //@{

  const Grid3D<float>* mGrid;

  ///Indica si queremos tapar el modelo
   bool m_bCloseSurface;
  ///Indica si estamos realizando el recorrido
   bool recorregut;
   float mValorSelec;

   funcioEsSel * m_EsSelLlistaf;
   void * m_EsSelLlistap;

   int mc_llescaInfPlena,mc_llescaSupPlena;
   int mc_llescaPostPlena,mc_llescaAntPlena;
   RangValors threshold;
   ///Marching Cubes Discreto
   int discret;  
   ///paso del recorrido por el mundo
   IJK noupas; 
   IJK mresolucio;
   IJK mResolucioMon;
   ///calculo normales geometricas
   int normal; 
   ///metodo de interpolacion
   int metode; 
   ///resolver ambigüedades (tapar agujeros)
   int ambig; 
   /// calculo del color a nivel de vertice
   bool calcul_color; 
    
   //  int taresta; //tamany de l'aresta
   // int tpropietat; //tipus de propietat
   glm::vec3 dim_aresta_voxel;
   ///chapuza para la  interpolaciom lineal
   int retoca_interpolacio; 
   ///origen de las llesques
   glm::vec3 origen,origenzp1, absOrigen;
   
  
   int voxelspatro3, voxelspatro6, voxelspatro7;

   int vp0, vp1, vp2, vp3, vp4, vp5, vp6, vp7, vp8, vp9, vp10, vp11, vp12, vp13, vp14,  patro;
   
 
  ///Llesques usadas durante el recorrido
   std::vector<Label> mc_llescaInf,mc_llescaSup,mc_llescaActual;
   std::vector<Label> mc_llescaAnt,mc_llescaPost;
   //vector<pVoxel048> PuntsIntIsoVox;
   std::vector< std::vector< Voxel048 > > PuntsIntIsoVox;
   int KActual;
  ///Labels usadas para SetAresta y CalculaGradient
   Label inf_posicio, inf_posiciomi, inf_posiciopi, inf_posiciomj, inf_posiciopj;
   Label sup_posicio, sup_posiciomi, sup_posiciopi, sup_posiciomj, sup_posiciopj;
   Label ant_posicio, post_posicio, inf_posicio_aresta, sup_posicio_aresta;
  //@}

   //hector
	InfoVoxel* mVoxelsIso;


   unsigned int mTamX, mTamY, mTamZ;

   std::vector <int> ta;
   std::vector <int> tvertexscaraiso;
};

#endif
