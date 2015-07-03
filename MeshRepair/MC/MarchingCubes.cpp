#include "MC/MarchingCubes.h"



// Establim uns valors maxim i minim per evitar que els punts d'interseccio
// en un voxel coincideixin amb un vertex d´aquest
#define MIN_RATIO 0.1
#define MAX_RATIO 0.9

using namespace std;

float InterpolacioLinialPunt(float p0, float p1, const RangValors &threshold);
void f_rec_Captacio(Captacio &captacio, const IJK &id, void *params);




MarchingCubes::MarchingCubes ()
{
    mGrid = 0;
	ini_patrons ();
}

MarchingCubes::MarchingCubes (const Grid3D<float> *g, const glm::vec3& voxelSize, const glm::vec3& origin)
{
    mGrid = g;
    mResolucioMon = IJK(g->getWidth(), g->getHeight(), g->getDepth());
    dim_aresta_voxel = voxelSize;
    origen = origin;
    absOrigen = origin;
	noupas = IJK(1,1,1);
    ini_patrons();
}


void MarchingCubes::Initialize (const RangValors &th, int mth, int t_norm, int t_amb, const IJK &resolucio, bool color, bool tapas)
{
	threshold = th;  
	m_bCloseSurface = tapas;
	metode = mth;
	normal = t_norm;
	ambig = t_amb;
	mresolucio = resolucio;
	discret = 0;
	calcul_color = color;

	// recalculem el pas i el tamany de l'aresta // inicialment pas = (1,1,1) i dimensió = dimensió món
    glm::vec3 dim = dim_aresta_voxel;
	noupas.i = noupas.j = noupas.k = 1;

	if (mresolucio.i <  mResolucioMon.i)
	{
		dim.x = ((float)mResolucioMon.i/(float)mresolucio.i)*dim_aresta_voxel.x;
		noupas.i = (int) ceil(dim.x/dim_aresta_voxel.x);
		if ((mresolucio.i%noupas.i) != 0) mresolucio.i++;
	}
	if (mresolucio.j < mResolucioMon.j)
	{
		dim.y = ((float)mResolucioMon.j/(float)mresolucio.j)*dim_aresta_voxel.y;
		noupas.j = (int) ceil(dim.y/dim_aresta_voxel.y);
		if ((mresolucio.j%noupas.j) != 0) mresolucio.j++;
	}

	if (mresolucio.k < mResolucioMon.k)
	{
		dim.z = ((float)mResolucioMon.k/(float)mresolucio.k)*dim_aresta_voxel.z;
		noupas.k = (int) ceil(dim.z/dim_aresta_voxel.z);
		if ((mresolucio.k%noupas.k) != 0) mresolucio.k++;
	}

    if(mGrid != 0) {
        float vmax = -1.0f;
        for (unsigned int i = 0; i < mGrid->getWidth(); i++) {
            for (unsigned int j = 0; j < mGrid->getHeight(); j++) {
                for (unsigned int k = 0; k < mGrid->getDepth(); k++) {
                    vmax = std::max(vmax, mGrid->get(i, j, k));
                }
            }
        }
        mValorSelec = vmax + 1;
    }

}

void MarchingCubes::Run (vector<double> &vertexs, vector<double> &normals, vector<unsigned int> &cares, FuncioOmpleF4V *omple, void *param)
{    
	ini_patrons ();

	mc_llescaInfPlena=0;
	mc_llescaSupPlena=0;
	mc_llescaAntPlena=0;
	mc_llescaPostPlena=0;

	mc_llescaInf.clear();
	mc_llescaSup.clear();
	mc_llescaActual.clear();
	mc_llescaAnt.clear();
	mc_llescaPost.clear();
	//PuntsIntIsoVox.clear();

	retoca_interpolacio = 0; //chapuza per interpolacio linial
	recorregut=true;

	int posicions = mresolucio.i * mresolucio.j;
	mc_llescaActual = vector<Label> (posicions);
	PuntsIntIsoVox.push_back(vector<Voxel048>(posicions));
	PuntsIntIsoVox.push_back(vector<Voxel048>(posicions));
	PuntsIntIsoVox.push_back(vector<Voxel048>(posicions));

	IJK primer(0, 0, 0);
	IJK ultim (mResolucioMon.i-1, mResolucioMon.j-1, mResolucioMon.k-1);

    mVoxelsIso = 0;
    {
		void *parametres[8];
		parametres[0] = this;
		parametres[1] = &vertexs;
		parametres[2] = &normals;
		parametres[3] = &cares;
        parametres[4] = &omple;
		parametres[5] = param;

        for (int k = primer.k; k <= ultim.k; k++) {
            for (int i = primer.i; i <= ultim.i; i++) {
                for (int j = primer.j; j <= ultim.j; j++) {
                    this->f_rec_Captacio(mGrid, IJK(i, j, k), parametres);
                }
            }
        }

	}

#ifdef BENCHMARKING_MC
	cerr << "patro 0 " << vp0 << "\n patro 1 " << vp1 << "\n patro 2 " << vp2 << "\n patro 3 " << vp3 << "\n patro 4 " << vp4 
		<< "\n patro 5 " << vp5 << "\n patro 6 " << vp6 << "\n patro 7 " << vp7 << "\n patro 8 " << vp8 << "\n patro 9 " 
		<< vp9 << "\n patro 10 " << vp10 << "\n patro 11 " << vp11 << "\n patro 12 " << vp12 << "\n patro 13 " << vp13 
		<< "\n patro 14 " << vp14 << "\n";
	cerr << "Discret 3:" << voxelspatro3 << "\nDiscret 6:" << voxelspatro6 << "\nDiscret 7:" << voxelspatro7 << "\n";
	cerr << "Quants voxels frontera " << vp0 + vp1 + vp3 + vp4 + vp5 + vp6 + vp7 + vp8 + vp9 + vp10 + vp11+ vp12 + vp13 + vp14 +
		voxelspatro3 + voxelspatro6 + voxelspatro7 << endl;
#endif
}




//---------------------------------------------------------------------
// Funcio de recorregut parcial per la captacio
//---------------------------------------------------------------------
void MarchingCubes::f_rec_Captacio(const Grid3D<float>* captacio, const IJK &id, void *params)
{

	void **parametres = (void**)params;
	parametres[6] = (void*)&captacio;
	parametres[7] = (void*)&id;

	MarchingCubes *MC = (MarchingCubes *)parametres[0];
	vector<double> *vertexs = (vector<double> *)parametres[1];
	vector<double> *normals = (vector<double> *)parametres[2];
	vector<unsigned int> *cares = (vector<unsigned int> *)parametres[3];

    float ValorProp = captacio->get(id.i, id.j, id.k);
    bool estat = (ValorProp >= MC->threshold.min) && (ValorProp <= MC->threshold.max);

	if(MC->m_bCloseSurface)
	{
		if ((id.i == 0) || (id.i >= MC->mResolucioMon.i - MC->noupas.i) ||
			(id.j == 0) || (id.j >= MC->mResolucioMon.j - MC->noupas.j) ||
			(id.k == 0) || (id.k >= MC->mResolucioMon.k - MC->noupas.k))
		{
			if(estat)
			{
				if(ValorProp - MC->threshold.min > MC->threshold.max - ValorProp)
				{
					ValorProp = MC->threshold.max + 1;
				}
				else
				{
					ValorProp = MC->threshold.min - 1;
				}
				estat = false;
			}
		}
	}

	MC->OmpleLlesca(ValorProp, estat, id, vertexs, normals, cares, params);
}


void MarchingCubes::ini_patrons ()
{
	// control del nombre de patrons
	vp0=0;
	vp1=0;
	vp2=0;
	vp3=0;
	vp4=0;
	vp5=0;
	vp6=0;
	vp7=0;
	vp8=0;
	vp9=0;
	vp10=0;
	vp11=0;
	vp12=0;
	vp13=0;
	vp14=0;

	voxelspatro3=0;
	voxelspatro6=0;
	voxelspatro7=0;


}

//-----------------------------------------------------------------------------
void MarchingCubes::CreaCares(int num_patro,int orient, const vector<int>& tvertexs, vector<unsigned int> *cares ){
	//-----------------------------------------------------------------------------
	switch(num_patro){
  case 0:
	  vp0++;
	  break;
  case 1:
	  if (orient==1) 
	  {
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]);
	  }
	  else 
	  {
		  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[0]);
	  }
	  vp1++;
	  break;
  case 2:
	  if (orient==1) 
	  {
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]);
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[2]); cares->push_back(tvertexs[3]);
	  }
	  else 
	  {
		  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[0]);
		  cares->push_back(tvertexs[3]); cares->push_back(tvertexs[2]); cares->push_back(tvertexs[0]);
	  }
	  vp2++;
	  break;
  case 3:
	  vp3++;
	  if (orient==-1 && ambig) // Resoldre ambiguitats
	  {
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[3]); cares->push_back(tvertexs[5]);
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[5]); cares->push_back(tvertexs[1]);
		  cares->push_back(tvertexs[3]); cares->push_back(tvertexs[0]); cares->push_back(tvertexs[2]);
		  cares->push_back(tvertexs[3]); cares->push_back(tvertexs[2]); cares->push_back(tvertexs[4]);
		  voxelspatro3++;
	  }
	  else
	  {
		  if(orient == 1)
		  {
			  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]);
			  cares->push_back(tvertexs[3]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[5]);
		  }
		  else
		  {
			  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[0]);
			  cares->push_back(tvertexs[5]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[3]);
		  }
	  } 
	  break;
  case 4: 
	  vp4++;
	  if(orient == 1)
	  {
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]);
		  cares->push_back(tvertexs[3]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[5]);
	  }
	  else
	  {
		  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[0]);
		  cares->push_back(tvertexs[5]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[3]);
	  }
	  break;
  case 5:  
	  if (orient==1)
	  {
		  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[3]); cares->push_back(tvertexs[4]);
		  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[0]);
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]);
	  }
	  else 
	  {
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[3]);
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[3]); cares->push_back(tvertexs[2]);
		  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[0]);
	  }
	  vp5++;  
	  break;

  case 12:
	  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[3]); cares->push_back(tvertexs[4]);
	  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[0]);
	  if (orient ==1)
	  {
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]);
		  cares->push_back(tvertexs[5]); cares->push_back(tvertexs[6]); cares->push_back(tvertexs[7]);
	  }
	  else
	  {  
		  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[0]);
		  cares->push_back(tvertexs[7]); cares->push_back(tvertexs[6]); cares->push_back(tvertexs[5]);
	  }
	  vp12++; 
	  break;
  case 6: 
	  vp6++; 
	  if (orient==-1)
	  {
		  if (ambig) {
			  cares->push_back(tvertexs[3]); cares->push_back(tvertexs[0]); cares->push_back(tvertexs[4]);
			  cares->push_back(tvertexs[4]); cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]);
			  cares->push_back(tvertexs[4]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[5]);
			  cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]); cares->push_back(tvertexs[5]);
			  cares->push_back(tvertexs[5]); cares->push_back(tvertexs[2]); cares->push_back(tvertexs[6]);
		  }
		  else {
			  if (orient)
			  {
				  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[0]);
			  }
			  else
			  {
				  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]);

			  }
			  { // Tot triangles
				  cares->push_back(tvertexs[6]); cares->push_back(tvertexs[5]); cares->push_back(tvertexs[4]);
				  cares->push_back(tvertexs[6]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[3]);
			  }
		  }
		  voxelspatro6++;
	  }
	  else
	  {

		  if (orient == 1)
		  {
			  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]);

		  }
		  else
		  { 
			  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[0]);
		  }
		  cares->push_back(tvertexs[3]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[5]);
		  cares->push_back(tvertexs[3]); cares->push_back(tvertexs[5]); cares->push_back(tvertexs[6]);
	  }
	  break;
  case 7: 
	  vp7++; 
	  if (orient==-1 && ambig)
	  {
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[3]); cares->push_back(tvertexs[6]);
		  // Tot triangles
		  cares->push_back(tvertexs[8]); cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]);     
		  cares->push_back(tvertexs[8]); cares->push_back(tvertexs[7]); cares->push_back(tvertexs[2]);  
		  cares->push_back(tvertexs[7]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[2]);  
		  cares->push_back(tvertexs[7]); cares->push_back(tvertexs[5]); cares->push_back(tvertexs[4]);  

		  voxelspatro7++;
	  }
	  else
	  { 
		  if (orient == 1)
		  {
			  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]);
			  cares->push_back(tvertexs[3]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[5]);
			  cares->push_back(tvertexs[6]); cares->push_back(tvertexs[7]); cares->push_back(tvertexs[8]);
		  }
		  else
		  {
			  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[0]);
			  cares->push_back(tvertexs[5]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[3]);
			  cares->push_back(tvertexs[8]); cares->push_back(tvertexs[7]); cares->push_back(tvertexs[6]);
		  }
	  }
	  break;
  case 8: 
	  vp8++; 
	  if (orient == 1)
	  {
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]);
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[2]); cares->push_back(tvertexs[3]);
	  }
	  else
	  {
		  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[0]);
		  cares->push_back(tvertexs[3]); cares->push_back(tvertexs[2]); cares->push_back(tvertexs[0]);
	  } 
	  break;
  case 9: 
	  vp9++; 
	  if (orient==1) {
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[5]);
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[4]);
		  cares->push_back(tvertexs[1]); cares->push_back(tvertexs[3]); cares->push_back(tvertexs[4]);
		  cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]); cares->push_back(tvertexs[3]);
	  }
	  else {
		  cares->push_back(tvertexs[5]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[0]);
		  cares->push_back(tvertexs[4]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[0]);
		  cares->push_back(tvertexs[4]); cares->push_back(tvertexs[3]); cares->push_back(tvertexs[1]);
		  cares->push_back(tvertexs[3]); cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]);
	  }
	  break;
  case 10: 
	  vp10++; 
	  if (orient == 1)
	  {
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]);
		  cares->push_back(tvertexs[4]); cares->push_back(tvertexs[5]); cares->push_back(tvertexs[6]);
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[2]); cares->push_back(tvertexs[3]);
		  cares->push_back(tvertexs[4]); cares->push_back(tvertexs[6]); cares->push_back(tvertexs[7]);
	  }
	  else {
		  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[0]);
		  cares->push_back(tvertexs[6]); cares->push_back(tvertexs[5]); cares->push_back(tvertexs[4]);
		  cares->push_back(tvertexs[3]); cares->push_back(tvertexs[2]); cares->push_back(tvertexs[0]);
		  cares->push_back(tvertexs[7]); cares->push_back(tvertexs[6]); cares->push_back(tvertexs[4]);   
	  }
	  break;
  case 11:  
  case 13:
  case 14:
	  if (orient == 1)
	  {
		  cares->push_back(tvertexs[0]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[2]);
		  cares->push_back(tvertexs[3]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[5]);
		  cares->push_back(tvertexs[6]); cares->push_back(tvertexs[7]); cares->push_back(tvertexs[8]);
		  cares->push_back(tvertexs[9]); cares->push_back(tvertexs[10]); cares->push_back(tvertexs[11]);
	  }
	  else
	  { 
		  cares->push_back(tvertexs[2]); cares->push_back(tvertexs[1]); cares->push_back(tvertexs[0]);
		  cares->push_back(tvertexs[5]); cares->push_back(tvertexs[4]); cares->push_back(tvertexs[3]);
		  cares->push_back(tvertexs[8]); cares->push_back(tvertexs[7]); cares->push_back(tvertexs[6]);
		  cares->push_back(tvertexs[11]); cares->push_back(tvertexs[10]); cares->push_back(tvertexs[9]);

	  }
	  if (num_patro==11) vp11++;
	  else if (num_patro==13) vp13++;
	  else vp14++;  
	  break;
  default:
	  cerr << "\n --- CreaCares --Patro no existent ";
	  break;
	}
}

//-----------------------------------------------------------------------------
int MarchingCubes::BuscaPuntInterseccio(const IJK &IJKvoxel,int num_aresta, 
										vector<double> *vertexs, vector<double> *normals)
										//-----------------------------------------------------------------------------
{
	int posiciopuntinterseccio;
	int posicio_IsoVox;

	posicio_IsoVox = (IJKvoxel.i/noupas.i) + ((mresolucio.i*IJKvoxel.j)/noupas.j);
	int k=(KActual - IJKvoxel.k)-1;
	Voxel048 *p048 = &(PuntsIntIsoVox[k][posicio_IsoVox]);
    if (!p048->Valid())
		p048->SetIJKvoxel(IJKvoxel); //--> modificat per EVa nova versio compilador 


	if (p048->GetPuntAresta(num_aresta)==-1)
	{
		if (recorregut) SetLabelsRecorregut(PuntsIntIsoVox[k][posicio_IsoVox]);
		else SetLabelsVoxel(PuntsIntIsoVox[k][posicio_IsoVox]);
		SetAresta(PuntsIntIsoVox[k][posicio_IsoVox], num_aresta);
		posiciopuntinterseccio = vertexs->size()/3;
        glm::vec3 punt;
		if(retoca_interpolacio==0)
		{
			punt = p048->RetornaPunt(num_aresta, origen, dim_aresta_voxel);
		}
		else
		{
			punt = p048->RetornaPunt(num_aresta, origenzp1, dim_aresta_voxel);
		}

		//hector
        if(mVoxelsIso != 0)
		{
			if(!mVoxelsIso[IJKvoxel.i*mTamY*mTamZ + IJKvoxel.j*mTamZ + IJKvoxel.k].initMarchingSticksQ())
			{
				mVoxelsIso[IJKvoxel.i*mTamY*mTamZ + IJKvoxel.j*mTamZ + IJKvoxel.k].initMarchingSticks();
			}
			mVoxelsIso[IJKvoxel.i*mTamY*mTamZ + IJKvoxel.j*mTamZ + IJKvoxel.k].setMarchingSticks(num_aresta,vertexs->size()/3);
		}

        vertexs->push_back(punt.x);
        vertexs->push_back(punt.y);
        vertexs->push_back(punt.z);

        glm::vec3 norm = p048->RetornaGradient(num_aresta);
        normals->push_back(norm.x);
        normals->push_back(norm.y);
        normals->push_back(norm.z);

		p048->SetPosicio_iv(num_aresta, posiciopuntinterseccio);



	}
	else { 
		posiciopuntinterseccio = p048->GetPosicio_iv(num_aresta);
	}
    p048=0;
	return posiciopuntinterseccio;
}


//---------------------------------------------------------------------
int MarchingCubes::CreaIndex(Label Linf,Label Lsup)
//--------------------------------------------------------------------------
{
	int factor=1,index=0; 

	// etiqueta guarda la diferencia entre propietat i threshold
	// el valor associat al vertex depen de si el valor de la diferencia es 
	// negatiu o positiu
	int i;
	for (i=0;i<4;i++)
	{
		int vertex = (Linf.GetState(i) ? 1:0);
		index = index + vertex*factor;
		factor = factor*2;
	}
	for(i=0;i<4;i++)
	{
		int vertex = (Lsup.GetState(i) ? 1:0);
		index = index + vertex*factor;
		factor = factor*2;
	}

	return index; 
}

//-------------------------------------------------------------------------------------
void MarchingCubes::CalculaPuntsInterseccio(const vector<int>& ArestesInterp, const IJK& IJKvoxel,
											vector<double> *vertexs, vector<double> *normals)
											//-------------------------------------------------------------------------------------
{


	if ((IJKvoxel.i/noupas.i == mresolucio.i - 1) || 
		(IJKvoxel.j/noupas.j == mresolucio.j - 1))
		return;

	int posiciopuntinterseccio,num_aresta;
	IJK IJKvoxeli;
	int i;
	for(i=0; i<(int)ArestesInterp.size();i++)
	{
		switch(ArestesInterp[i])
		{
		case 0:
		case 4:
		case 8:
			IJKvoxeli=IJKvoxel;
			num_aresta=ArestesInterp[i];
			retoca_interpolacio = 0;
			break;
		case 1:
			IJKvoxeli.i=IJKvoxel.i;
			IJKvoxeli.j=IJKvoxel.j+noupas.j;
			IJKvoxeli.k=IJKvoxel.k;
			retoca_interpolacio = 0;
			num_aresta=0;
			break;
		case 2:
			IJKvoxeli.i=IJKvoxel.i;
			IJKvoxeli.j=IJKvoxel.j;
			IJKvoxeli.k=IJKvoxel.k+noupas.k;
			retoca_interpolacio = 1;
			num_aresta=0;
			break;
		case 3:
			IJKvoxeli.i=IJKvoxel.i;
			IJKvoxeli.j=IJKvoxel.j+noupas.j;
			IJKvoxeli.k=IJKvoxel.k+noupas.k;
			retoca_interpolacio = 1;
			num_aresta=0;
			break;
		case 5:
			IJKvoxeli.i=IJKvoxel.i+noupas.i;
			IJKvoxeli.j=IJKvoxel.j;
			IJKvoxeli.k=IJKvoxel.k;
			retoca_interpolacio = 0;
			num_aresta=4;
			break;
		case 6:
			IJKvoxeli.i=IJKvoxel.i;
			IJKvoxeli.j=IJKvoxel.j;
			IJKvoxeli.k=IJKvoxel.k+noupas.k;
			retoca_interpolacio = 1;
			num_aresta=4;
			break;
		case 7:
			IJKvoxeli.i=IJKvoxel.i+noupas.i;
			IJKvoxeli.j=IJKvoxel.j;
			IJKvoxeli.k=IJKvoxel.k+noupas.k;
			retoca_interpolacio = 1;
			num_aresta=4;
			break;
		case 9:
			IJKvoxeli.i=IJKvoxel.i+noupas.i;
			IJKvoxeli.j=IJKvoxel.j;
			IJKvoxeli.k=IJKvoxel.k;
			retoca_interpolacio = 0;
			num_aresta=8;
			break;
		case 10:
			IJKvoxeli.i=IJKvoxel.i;
			IJKvoxeli.j=IJKvoxel.j+noupas.j;
			IJKvoxeli.k=IJKvoxel.k;
			retoca_interpolacio = 0;
			num_aresta=8;
			break;
		case 11:
			IJKvoxeli.i=IJKvoxel.i+noupas.i;
			IJKvoxeli.j=IJKvoxel.j+noupas.j;
			IJKvoxeli.k=IJKvoxel.k;
			retoca_interpolacio = 0;
			num_aresta=8;
			break;
		default:
			num_aresta=-1;
			cerr << " \n ARESTA NO EXISTENT \n";
			break;
		}
		posiciopuntinterseccio = BuscaPuntInterseccio(IJKvoxeli,num_aresta, vertexs, normals);
		tvertexscaraiso.push_back (posiciopuntinterseccio);
	}
}


//--------------------------------------------------------------
void MarchingCubes::ClassificaVoxels (vector<double> *vertexs, vector<double> *normals, vector<unsigned int> *cares, void *)
//---------------------------------------------------------------
{

	int index_voxel=0,num_patro,orientacio;
	int i;
    float /*mDimVx, mDimVy,*/ mDimVz;

    //mDimVx = dim_aresta_voxel.x;
    //mDimVy = dim_aresta_voxel.y;
	mDimVz = dim_aresta_voxel.z;

	IJK ijk = mc_llescaInf[0].GetIJKLabel();
    //IJK ijk1 = mc_llescaSup[0].GetIJKLabel();

	//dim_aresta_voxel.z = CalcularSeparacioLlesques(ijk.k,ijk1.k);
	//origen = mCaptacio->GetpLlesca(ijk.k)->GetPosicioLlesca();
	//origenzp1 = mCaptacio->GetpLlesca(ijk.k+1)->GetPosicioLlesca();

    origen = absOrigen + glm::vec3(0, 0, mDimVz * ijk.k);
    origenzp1 = absOrigen + glm::vec3(0, 0, mDimVz * (ijk.k + 1));

    /*dim_aresta_voxel.z = origenzp1.z - origen.z;

	if(dim_aresta_voxel.z<0)
	{
		dim_aresta_voxel.z = dim_aresta_voxel.z * -1;
	}
    mDimVz =dim_aresta_voxel.z;*/

	int faces = 0;
	for (i=0; i<(int)mc_llescaSup.size();i++)
	{	     
		index_voxel=CreaIndex(mc_llescaInf[i],mc_llescaSup[i]);
		BuscaPatro(index_voxel,num_patro,orientacio);

		if(num_patro!=0)
		{
			IJK IJKlabel = mc_llescaInf[i].GetIJKLabel();

			CalculaPuntsInterseccio(ta,IJKlabel,vertexs,normals);
			if (tvertexscaraiso.size () > 0)
		 {
			 unsigned int p_i = cares->size() / 3;
			 // Aquí abans
			 int sizePrev = cares->size()/3;			
			 // Crea cares
			 CreaCares(num_patro,orientacio,tvertexscaraiso,cares);
			 // Despres d'assignar cares
			 int sizePost = cares->size()/3;
			 vector<unsigned int> newFaces;
			 newFaces.clear();
			 for(int i = sizePrev;i<sizePost;i++)
				 newFaces.push_back(i);
			 //void **parametres = (void**)params;
			 if(newFaces.size() != 0)
			 {
				 //**** PROVES_INIT
				 faces += newFaces.size();
				 //cout << "NUM CARA1: " << newFaces.at(0) << endl;
				 //**** PROVES_FI
                 /*FuncioOmpleF4V *omple = (FuncioOmpleF4V*)parametres[4];
                 if (omple != 0)
				 {
					 // Cridem al metode que omple la Mat3D amb les cares per voxel
					 Captacio &cap = (Captacio&)parametres[6];
                     omple(cap,IJKlabel,newFaces,parametres[5]);
                 }*/
			 }

			 unsigned int p_f = cares->size() / 3;
             if(mVoxelsIso!=0)
			 {
				 mVoxelsIso[IJKlabel.i*mTamY*mTamZ + IJKlabel.j*mTamZ + IJKlabel.k].setState(VOXEL_SUPERFICIE);
				 mVoxelsIso[IJKlabel.i*mTamY*mTamZ + IJKlabel.j*mTamZ + IJKlabel.k].initTriangles();
				 for(unsigned int a = p_i; a < p_f; a++)
				 {
					 mVoxelsIso[IJKlabel.i*mTamY*mTamZ + IJKlabel.j*mTamZ + IJKlabel.k].pushTriangle(a);
				 }
			 }

             //RevisioNormal(vertexs,normals,cares,IJKlabel,p_i);
			 tvertexscaraiso.erase(tvertexscaraiso.begin(), tvertexscaraiso.end());
		 }

			ta.erase(ta.begin(), ta.end());
		}
		//hector
        else if(index_voxel==255 && mVoxelsIso!=0)
		{
			IJK IJKlabel = mc_llescaInf[i].GetIJKLabel();
			if(mVoxelsIso[IJKlabel.i*mTamY*mTamZ + IJKlabel.j*mTamZ + IJKlabel.k].getState()!=VOXEL_EXTERIOR) printf("ERROR chivato voxel totalmente interior\n");
			mVoxelsIso[IJKlabel.i*mTamY*mTamZ + IJKlabel.j*mTamZ + IJKlabel.k].setState(VOXEL_INTERIOR);
		}
	}
}


//-----------------------------------------------------------------------
void MarchingCubes::OmpleVeins(int PosicioLlesca, const IJK &posMon, float ValorProp, bool Estat)
//-----------------------------------------------------------------------
{
	int resol_i = mresolucio.i; 

	if (posMon.i*posMon.j!=0){ 
		mc_llescaActual[PosicioLlesca-resol_i-1].SetValue(3,ValorProp,Estat);
		mc_llescaActual[PosicioLlesca-1].SetValue(1,ValorProp,Estat);
		mc_llescaActual[PosicioLlesca-resol_i].SetValue(2,ValorProp,Estat);
	}
	else 
		if (posMon.i==0 && posMon.j!=0){ 
			mc_llescaActual[PosicioLlesca-resol_i].SetValue(2,ValorProp,Estat);
		}
		else{ 
			if (posMon.i!=0 && posMon.j==0){
				mc_llescaActual[PosicioLlesca-1].SetValue(1,ValorProp,Estat);
			}
		}

		if (posMon.i>=mResolucioMon.i-noupas.i){
			mc_llescaActual[PosicioLlesca].SetValue(1,ValorProp,Estat);
			if (posMon.j!=0){
				mc_llescaActual[PosicioLlesca-resol_i].SetValue(3,ValorProp,Estat);
			} 
		}
		if (posMon.j>=mResolucioMon.j-noupas.j){
			mc_llescaActual[PosicioLlesca].SetValue(2,ValorProp,Estat);
			if (posMon.i!=0){
				mc_llescaActual[PosicioLlesca-1].SetValue(3,ValorProp,Estat);
			}            
		}
		if (posMon.i>=mResolucioMon.i-noupas.i && posMon.j>=mResolucioMon.j-noupas.j){
			mc_llescaActual[PosicioLlesca].SetValue(3,ValorProp,Estat);
			if (mc_llescaInfPlena==0) 
			{
				mc_llescaInf = mc_llescaActual;
				mc_llescaInfPlena=1;
			}
			else {
				if (mc_llescaSupPlena==0) {
					mc_llescaSup = mc_llescaActual;
					mc_llescaSupPlena=1;
				}
				else {
					mc_llescaPost = mc_llescaActual;
					mc_llescaPostPlena=1;
				}
			}
		}
}

//-----------------------------------------------------------------
void MarchingCubes::OmpleLlesca(float ValorProp, bool Estat, const IJK &posMon, vector<double> *vertexs, 
								vector<double> *normals, vector<unsigned int> *cares, void *params)
								//-----------------------------------------------------------------
{
	int posicio_dins_llesca;

	posicio_dins_llesca=posMon.i/noupas.i+mresolucio.i*(posMon.j/noupas.j);

	mc_llescaActual[posicio_dins_llesca].SetIJKLabel(posMon);
	mc_llescaActual[posicio_dins_llesca].SetValue(0,ValorProp,Estat);

	OmpleVeins(posicio_dins_llesca, posMon, ValorProp, Estat);

    if (mc_llescaInfPlena && mc_llescaSupPlena && mc_llescaPostPlena)

	{
		//dim_aresta_voxel.z = CalcularSeparacioLlesques(posMon.k-1,posMon.k-2);
		KActual = posMon.k;

		ClassificaVoxels(vertexs, normals, cares, params);

		mc_llescaAnt = mc_llescaInf;
		mc_llescaInf = mc_llescaSup;
		mc_llescaSup = mc_llescaPost;

		mc_llescaAntPlena=1;
		mc_llescaInfPlena=1;
		mc_llescaSupPlena=1;
		mc_llescaPostPlena=0;

		PuntsIntIsoVox[2].swap(PuntsIntIsoVox[1]);
		PuntsIntIsoVox[1].swap(PuntsIntIsoVox[0]);
		for(unsigned int i=0; i<PuntsIntIsoVox[0].size(); i++) PuntsIntIsoVox[0][i].IJKVoxel048.i = -1;   
	}
	if(m_bCloseSurface)
		if ((posMon.i == mResolucioMon.i-1) && (posMon.j == mResolucioMon.j-1) && 
			(posMon.k == mResolucioMon.k-1))
		{

			ClassificaVoxels(vertexs, normals, cares, params);

			PuntsIntIsoVox[2].swap(PuntsIntIsoVox[1]);
			PuntsIntIsoVox[1].swap(PuntsIntIsoVox[0]);
			PuntsIntIsoVox[0] = vector<Voxel048>(mresolucio.i * mresolucio.j);//(PuntsIntIsoVox[0]);
		}
}


//-----------------------------------------------------------------
void MarchingCubes::SetAresta (Voxel048 &vox, int num_aresta)
//-----------------------------------------------------------------

{ 
	float component_punt;
    float p0 = 0,p1 = 0, aresta;

	switch(metode)
	{
	case 0: // interpolacio en el punt mig de l'aresta
		component_punt=1.0/2.0;
		//component_punt=aresta*taresta/2.0;
		break;
	case 1: //interpolacio linial: p0-thrshold /(p0-p1)
		if (!retoca_interpolacio)
			//p0=mc_llescaInf[posicio_dins_llesca].GetValue(0);
			p0=inf_posicio_aresta.GetValue(0);
		else
			//p0=mc_llescaSup[posicio_dins_llesca].GetValue(0);
			p0=sup_posicio_aresta.GetValue(0);
		break;
	default: 
		cout << metode << "****Metode interpolacio desconegut *** (0) Punt mig (1)Interpolacio linial **********\n";
		exit(-1);
	}


	switch(num_aresta)
	{
	case 0:
		aresta = dim_aresta_voxel.x;
		if (metode == 0){
			component_punt=component_punt*noupas.i*aresta;
		}
		else //(metode==1)
		{
			if (!retoca_interpolacio)
				//p1=mc_llescaInf[posicio_dins_llesca].GetValue(1);
				p1=inf_posicio_aresta.GetValue(1);
			else
				//p1=mc_llescaSup[posicio_dins_llesca].GetValue(1);
				p1=sup_posicio_aresta.GetValue(1);

			component_punt = InterpolacioLinialPunt(p0,p1, threshold);
			component_punt = component_punt * noupas.i * aresta;
		}
		vox.PuntAresta0=component_punt;
		break;          
	case 4:
		aresta = dim_aresta_voxel.y;
		if (metode==0){
			component_punt=component_punt*noupas.j*aresta;
		}
		else // (metode==1)
		{
			if (!retoca_interpolacio)
				//p1=mc_llescaInf[posicio_dins_llesca].GetValue(2);
				p1=inf_posicio_aresta.GetValue(2);
			else
				//p1=mc_llescaSup[posicio_dins_llesca].GetValue(2);
				p1=sup_posicio_aresta.GetValue(2);

			component_punt = InterpolacioLinialPunt(p0,p1,threshold);
			component_punt = component_punt * noupas.j * aresta;
		}
		vox.PuntAresta4=component_punt;
		break; 
	case 8:
		aresta = dim_aresta_voxel.z;
		if (metode==0){
			component_punt=component_punt*noupas.k*aresta;
		}
		else // (metode==1)
		{
			//p1 = mc_llescaSup[posicio_dins_llesca].GetValue(0);
			p1=sup_posicio_aresta.GetValue(0);
			component_punt = InterpolacioLinialPunt(p0,p1,threshold);		
			component_punt = component_punt * noupas.k * aresta;
		}
		vox.PuntAresta8=component_punt;
		break;          
	}

	CalculaGradient(vox, num_aresta);

}
//-----------------------------------------------------------------
void MarchingCubes::CalculaGradient(Voxel048 &vox, int num_aresta)
//-----------------------------------------------------------------
{
	//int posicio,IJKi,IJKj;
    glm::vec3 v1,v2;


	/* Si la posicio i o j es igual a 0 o a mresolucio.i o .j, es produeix un error 
	perque ens anem fora de rang dins de la llesca. Per solventar-ho el mourem
	+1 o -1, d´aquesta manera segur que estem dins del rang => no es un error
	greu suposar que el gradient en un punt sera molt similar al del vei      */

	/*  SERGI tenia això 
	if (IJKVoxel048.i==0) IJKi = IJKVoxel048.i+1;
	else if (IJKVoxel048.i==mresolucio.i) IJKi = IJKVoxel048.i-1;
	else IJKi = IJKVoxel048.i;
	if (IJKVoxel048.j==0) IJKj = IJKVoxel048.j+1;
	else if (IJKVoxel048.j==mresolucio.j) IJKj = IJKVoxel048.j-1;
	else IJKj = IJKVoxel048.j;
	*/

	/* Ho modifiquem pel que ha arreglat el Marc */
	/*  if (pvox.v->IJKVoxel048.i==0) IJKi = pvox.v->IJKVoxel048.i+noupas.i;
	else if (pvox.v->IJKVoxel048.i==mresolucio.i) IJKi = pvox.v->IJKVoxel048.i-noupas.i;
	else IJKi = pvox.v->IJKVoxel048.i;
	if (pvox.v->IJKVoxel048.j==0) IJKj = pvox.v->IJKVoxel048.j+noupas.j;
	else if (pvox.v->IJKVoxel048.j==mresolucio.j) IJKj = pvox.v->IJKVoxel048.j-noupas.j;
	else IJKj = pvox.v->IJKVoxel048.j;

	posicio = IJKi/noupas.i+mresolucio.i*IJKj/noupas.j;

	Si mc_llescaAnt no esta plena es produeix un error, perque no peti
	copiarem mc_llescaInf sobre mc_llescaAnt 
	if (!mc_llescaAntPlena) { 
	mc_llescaAnt = mc_llescaInf;
	mc_llescaAntPlena = 1;
	}
	*/    
	//Gx(i,j,k) = (D(i+1,j,k) - D(i-1,j,k)) / dx
	//Gy(i,j,k) = (D(i,j+1,k) - D(i,j-1,k)) / dy
	//Gz(i,j,k) = (D(i,j,k+1) - D(i,j,k-1)) / dz


	if (!retoca_interpolacio)
	{
		v2[0] = (inf_posiciomi.GetValue(0) - 
			inf_posicio.GetValue(1))
			/dim_aresta_voxel.x;
		v2[1] = (inf_posiciomj.GetValue(0) -
			inf_posicio.GetValue(2))
			/dim_aresta_voxel.y;
		v2[2] = (ant_posicio.GetValue(0) -
			sup_posicio.GetValue(0)) 
			/ dim_aresta_voxel.z;
	}
	else
	{
		v2[0] = (sup_posiciomi.GetValue(0) -
			sup_posicio.GetValue(1))
			/dim_aresta_voxel.x;
		v2[1] = (sup_posiciomj.GetValue(0) -
			sup_posicio.GetValue(2))
			/dim_aresta_voxel.y;
		v2[2] = (-post_posicio.GetValue(0) + 
			inf_posicio.GetValue(0)) 
			/ dim_aresta_voxel.z;
	}

	switch (num_aresta)
	{
	case 0:
		if (!retoca_interpolacio)
		{
			v1[0] = (inf_posicio.GetValue(0) -
				inf_posiciopi.GetValue(1))
				/dim_aresta_voxel.x;
			v1[1] = (inf_posiciomj.GetValue(1)-
				inf_posicio.GetValue(3))
				/dim_aresta_voxel.y;
			v1[2] = (ant_posicio.GetValue(1) -
				sup_posicio.GetValue(1)) 
				/dim_aresta_voxel.z;
		}
		else
		{
			v1[0] = (sup_posicio.GetValue(0) -
				sup_posiciopi.GetValue(1))
				/dim_aresta_voxel.x;
			v1[1] = (sup_posiciomj.GetValue(1)-
				sup_posicio.GetValue(3))
				/dim_aresta_voxel.y;
			v1[2] = (inf_posicio.GetValue(1) -
				post_posicio.GetValue(1)) 
				/dim_aresta_voxel.z;
		}  

        vox.Grad0 = (v1 * vox.PuntAresta0 + v2 * (noupas.i * dim_aresta_voxel.x - vox.PuntAresta0)) / 2.0f;
        vox.Grad0 = glm::normalize(vox.Grad0);

		break;
	case 4:
		if (!retoca_interpolacio)
		{
			v1[0] = (inf_posiciomi.GetValue(2) -
				inf_posicio.GetValue(3))
				/dim_aresta_voxel.x;
			v1[1] = (inf_posicio.GetValue(0) -
				inf_posiciopj.GetValue(2))
				/dim_aresta_voxel.y;
			v1[2] = (ant_posicio.GetValue(2) -
				sup_posicio.GetValue(2)) 
				/ dim_aresta_voxel.z;
		}
		else
		{
			v1[0] = (sup_posiciomi.GetValue(2) -
				sup_posicio.GetValue(3))
				/dim_aresta_voxel.x;
			v1[1] = (sup_posicio.GetValue(0) -
				sup_posiciopj.GetValue(2))
				/dim_aresta_voxel.y;
			v1[2] = (inf_posicio.GetValue(2) -
				post_posicio.GetValue(2))
				/dim_aresta_voxel.z;
		}

        vox.Grad4 = (v1 * vox.PuntAresta4 + v2 * (noupas.j * dim_aresta_voxel.y - vox.PuntAresta4)) / 2.0f;
        vox.Grad4 = glm::normalize(vox.Grad4);

		break;
	case 8:
		if (!retoca_interpolacio)
		{
			v1[0] = (sup_posiciomi.GetValue(0) -
				sup_posicio.GetValue(1))
				/dim_aresta_voxel.x;
			v1[1] = (sup_posiciomj.GetValue(0)-
				sup_posicio.GetValue(2))
				/dim_aresta_voxel.y;
			v1[2] = (inf_posicio.GetValue(0) -
				post_posicio.GetValue(0)) 
				/dim_aresta_voxel.z;
		}
		else
		{
			// No pasara per aqui mai perque a les arestes verticals no es 
			// retoca la interpolacio
		}

        vox.Grad8 = (v1 * vox.PuntAresta8 + v2 * (noupas.k * dim_aresta_voxel.z - vox.PuntAresta8)) / 2.0f;
        vox.Grad8 = glm::normalize(vox.Grad8);

		break;
	}


}




//-----------------------------------------------------------------
void MarchingCubes::ObtenirValorEstat(const IJK &ijk, float& val, bool& estat)
//-----------------------------------------------------------------
{


	// estat = mSocInterior (ijk); // astribut mSocInterior de tipus bool f ();
	// val = mCaptacio->GetValorPixel(ijk)(); // valor real !!!! (no seleccio activa)
#ifdef USING_MV
	if (using_vo2)
	{
		val = mMonVoxels->GetValorPropVoxel (ijk,idpropietat)[0];
		estat = (val >=threshold.min)&&(val <= threshold.max);
	}
	else
#endif
	{
        val = mGrid->get(ijk.i, ijk.j, ijk.k);
        estat = (val >=threshold.min)&&(val <= threshold.max);
	}
}
//-----------------------------------------------------------------
void MarchingCubes::RevisioNormal(vector<double> *vertexs, vector<double> *normals, vector<unsigned int> *cares,const IJK &ijk, unsigned int cara_inici)
//-----------------------------------------------------------------
{
	unsigned int id_cares=cara_inici, t,v;
	// recorregut pels vèrtexs afegits mirant si el seu gradient . normal_geometrica > 0
	for (t = id_cares; t < cares->size()/3; t++)
	{

        glm::vec3 normal = glm::cross(
                           glm::vec3( (*vertexs)[(*cares)[t*3]*3]-(*vertexs)[(*cares)[t*3+2]*3],
                                      (*vertexs)[(*cares)[t*3]*3+1]-(*vertexs)[(*cares)[t*3+2]*3+1],
                                      (*vertexs)[(*cares)[t*3]*3+2]-(*vertexs)[(*cares)[t*3+2]*3+2] ),
                           glm::vec3( (*vertexs)[(*cares)[t*3+1]*3]-(*vertexs)[(*cares)[t*3]*3],
                                      (*vertexs)[(*cares)[t*3+1]*3+1]-(*vertexs)[(*cares)[t*3]*3+1],
                                      (*vertexs)[(*cares)[t*3+1]*3+2]-(*vertexs)[(*cares)[t*3]*3+2]));
        if (glm::length(normal) > 1e-3)
            normal = glm::normalize(normal);


        if ((ijk.i <= 0) || (ijk.i >= mResolucioMon.i - noupas.i) ||
			(ijk.j <= 0) || (ijk.j >= mResolucioMon.j - noupas.j) ||
			(ijk.k <= 0) || (ijk.k >= mResolucioMon.k - noupas.k))
		{
			for (v = 0; v < 3; v++)
			{
                (*normals)[(*cares)[t*3+v]*3]   = normal.x;
                (*normals)[(*cares)[t*3+v]*3+1] = normal.y;
                (*normals)[(*cares)[t*3+v]*3+2] = normal.z;
			}
		}
		else{


			for (v = 0; v < 3; v++)
			{
                glm::vec3 normal2((*normals)[(*cares)[t*3+v]*3],(*normals)[(*cares)[t*3+v]*3+1],(*normals)[(*cares)[t*3+v]*3+2]);
                if (glm::dot(normal, normal2) < 0)
				{
					(*normals)[(*cares)[t*3+v]*3]   = -(*normals)[(*cares)[t*3+v]*3];
					(*normals)[(*cares)[t*3+v]*3+1] = -(*normals)[(*cares)[t*3+v]*3+1];
					(*normals)[(*cares)[t*3+v]*3+2] = -(*normals)[(*cares)[t*3+v]*3+2];
				}
			}
		}
	}
#ifdef USING_MV
	if (augmentar_vo2) 
	{
		float tmp[6] = {-1,-1,-1,-1,-1,-1};
		Valor v(6, tmp);
		for (t = id_cares; t < cares->size()/3; t++)
			v[ t - id_cares] = t;
		mMonVoxels->SetValorPropVoxel (ijk, idPropAug, v);
	}
#endif
}
//-----------------------------------------------------------------
//void MarchingCubes::RecorregutLlista(list<IJK> &llista, vector<double> &vertexs, vector<double> &normals, vector<unsigned int> &cares,RangValors &rang, funcioEsSel * f, void * param)
//-----------------------------------------------------------------
//{
/*int index_voxel, num_patro, orientacio;
//vector<int> ta, tvertexs;
list<IJK>::iterator it = llista.begin();
recorregut=false;

//PuntsIntIsoVox = vector <pVoxel048>(mresolucio.i*mresolucio.j*mresolucio.k);

threshold.min = rang.min;
threshold.max = rang.max;
m_EsSelLlistaf = f;
m_EsSelLlistap = param;

while(it!=llista.end())
{
if((*it).k<mresolucio.k-1)
{
origen = mCaptacio->GetpLlesca((*it).k)->GetPosicioLlesca();
origenzp1 = mCaptacio->GetpLlesca((*it).k+1)->GetPosicioLlesca();

SetLabelsPatro(*it);
index_voxel=CreaIndex(inf_posicio,sup_posicio);
BuscaPatro(index_voxel, num_patro, orientacio);
if (num_patro!=0)
{
CalculaPuntsInterseccio(ta,(*it), &vertexs, &normals);
if (tvertexscaraiso.size() > 0)
{
unsigned int i=cares.size();
CreaCares(num_patro, orientacio, tvertexscaraiso, &cares);
RevisioNormal(&vertexs,&normals,&cares,(*it),i);
}
tvertexscaraiso.clear();
}
ta.clear();
it++;
}
}*/
void MarchingCubes::RecorregutLlista(const list<IJK> &llista, vector<double> &vertexs, vector<double> &normals, vector<unsigned int> &cares)
{
	list<IJK>::const_iterator it, it_inici, it_fi;
	it_inici = it = llista.begin();
	it_fi = llista.end(); it_fi--;
	int x,y,z;
	float ValorProp;
	bool estat;
	IJK ijk_actual;


	ini_patrons ();

	mc_llescaInfPlena=0;
	mc_llescaSupPlena=0;
	mc_llescaAntPlena=0;
	mc_llescaPostPlena=0;

	mc_llescaInf.clear();
	mc_llescaSup.clear();
	mc_llescaActual.clear();
	mc_llescaAnt.clear();
	mc_llescaPost.clear();
	//PuntsIntIsoVox.clear();

	retoca_interpolacio = 0; //chapuza per interpolacio linial
	recorregut=true;

	int posicions = mresolucio.i * mresolucio.j;
	mc_llescaActual = vector<Label> (posicions);
	PuntsIntIsoVox.push_back(vector<Voxel048>(posicions));
	PuntsIntIsoVox.push_back(vector<Voxel048>(posicions));
	PuntsIntIsoVox.push_back(vector<Voxel048>(posicions));

	IJK primer(0, 0, 0);
	IJK ultim (mResolucioMon.i-1, mResolucioMon.j-1, mResolucioMon.k-1);

    mVoxelsIso = 0;

	void *parametres[8];
	parametres[0] = this;
	parametres[1] = &vertexs;
	parametres[2] = &normals;
	parametres[3] = &cares;
    parametres[4] = 0;
    parametres[5] = 0;

	cout << "INICI " << it_inici->k << " FI " << it_fi->k << endl;
	for (z=it_inici->k; z<=it_fi->k; z+=noupas.k)
		for (y=0; y<mResolucioMon.j; y+=noupas.j)
			for (x=0; x<mResolucioMon.i; x+=noupas.i)
			{
				ijk_actual = IJK(x,y,z);
                ValorProp = mGrid->get(ijk_actual.i, ijk_actual.j, ijk_actual.k);

				if(it!=llista.end())
					estat = (ijk_actual==(*it));
				else
					estat = false;

				if (estat)
					it++;

				if(m_bCloseSurface)
				{
					if ((ijk_actual.i == 0) || (ijk_actual.i >= mResolucioMon.i - noupas.i) ||
						(ijk_actual.j == 0) || (ijk_actual.j >= mResolucioMon.j - noupas.j) ||
						(ijk_actual.k == it_inici->k) || (ijk_actual.k >= it_fi->k - noupas.k))
					{
						if(estat)
						{
							if (ValorProp - threshold.min > threshold.max - ValorProp)
							{
								ValorProp = threshold.max + 1;
							}
							else
							{
								ValorProp = threshold.min - 1;
							}
							estat = false;
						}
					}
				}
				OmpleLlesca(ValorProp, estat, ijk_actual, &vertexs, &normals, &cares, parametres);
			}

}

/*
void MarchingCubes::RecorregutLlavor(const IJK &ijk, vector<double> &vertexs, vector<double> &normals, vector<unsigned int> &cares)
{
list<IJK> llista_voxels;
IJK voxel_actual;
int index_voxel, num_patro, orientacio;
vector<int> ta, tvertexs;

llista_voxels.push_back(ijk);
while(llista_voxels.size()!=0)
{
voxel_actual=llista_voxels.front();
SetLabelsPatro(voxel_actual);
index_voxel=CreaIndex(inf_posicio,sup_posicio);
ta = BuscaPatro(index_voxel, num_patro, orientacio);
if (num_patro!=0)
{
tvertexs = CalculaPuntsInterseccio(ta,voxel_actual, &vertexs, &normals);
CreaCares(num_patro, orientacio, tvertexs, &cares);
RevisioNormal(&vertexs,&normals,&cares,voxel_actual);	 
}
AfegirVeins(ta, llista_voxels, voxel_actual);
llista_voxels.pop_front();
ta.clear();
tvertexs.clear();
}
}
*/
//-----------------------------------------------------------------
void MarchingCubes::AfegirVeins(const vector<int> &arestes, list<IJK> &llista, IJK &voxel_actual)
//-----------------------------------------------------------------
{
	int i;
	IJK ijk_i(1,0,0), ijk_j(0,1,0), ijk_k(0,0,1);
	bool a[12]={false,false,false,false,false,false,false,false,false,false,false,false};
	for(i=0; i<(int)arestes.size(); i++)
		a[arestes[i]]=true;
	if (a[3] || a[10] || a[1] || a[11]) llista.push_back(voxel_actual + ijk_j);
	if (a[4] || a[0] || a[5] || a[1]) llista.push_back(voxel_actual - ijk_k);
	if (a[8] || a[0] || a[2] || a[9]) llista.push_back(voxel_actual - ijk_j);
	if (a[2] || a[6] || a[7] || a[3]) llista.push_back(voxel_actual + ijk_k);
	if (a[6] || a[8] || a[4] || a[10]) llista.push_back(voxel_actual - ijk_i);
	if (a[7] || a[11] || a[5] || a[9]) llista.push_back(voxel_actual + ijk_i);
}

//-----------------------------------------------------------------
void MarchingCubes::SetLabelsPatro(const IJK &ijk)
//-----------------------------------------------------------------
{
	int i=ijk.i, j=ijk.j, k=ijk.k;
	IJK ijk_k(0,0,1);
	IJK ijk_0(i,j,k), ijk_1(i+1,j,k), ijk_2(i,j+1,k), ijk_3(i+1,j+1,k);
	float valor; bool estat;

	ObtenirValorEstat(ijk_0,valor,estat);
	inf_posicio.SetValue(0,valor,estat);

	ObtenirValorEstat(ijk_1,valor,estat);
	inf_posicio.SetValue(1,valor,estat);

	ObtenirValorEstat(ijk_2,valor,estat);
	inf_posicio.SetValue(2,valor,estat);

	ObtenirValorEstat(ijk_3,valor,estat);
	inf_posicio.SetValue(3,valor,estat);

	ObtenirValorEstat(ijk_0 + ijk_k,valor,estat);
	sup_posicio.SetValue(0,valor,estat);

	ObtenirValorEstat(ijk_1 + ijk_k,valor,estat);
	sup_posicio.SetValue(1,valor,estat);

	ObtenirValorEstat(ijk_2 + ijk_k,valor,estat);
	sup_posicio.SetValue(2,valor,estat);

	ObtenirValorEstat(ijk_3 + ijk_k,valor,estat);
	sup_posicio.SetValue(3,valor,estat);
}
//-----------------------------------------------------------------
void MarchingCubes::SetLabelsVoxel(const Voxel048 &vox)
//-----------------------------------------------------------------
{
	IJK ijk_i(1,0,0), ijk_j(0,1,0), ijk_k(0,0,1);
	IJK ijk(vox.IJKVoxel048.i,vox.IJKVoxel048.j,vox.IJKVoxel048.k);
	int i=ijk.i, j=ijk.j, k=ijk.k;
	IJK ijk_0(i,j,k), ijk_1(i+1,j,k), ijk_2(i,j+1,k), ijk_3(i+1,j+1,k);
	float valor; bool estat;

	ObtenirValorEstat(ijk_0,valor,estat);
	inf_posicio_aresta.SetValue(0,valor,estat);

	ObtenirValorEstat(ijk_1,valor,estat);
	inf_posicio_aresta.SetValue(1,valor,estat);

	ObtenirValorEstat(ijk_2,valor,estat);
	inf_posicio_aresta.SetValue(2,valor,estat);

	ObtenirValorEstat(ijk_3,valor,estat);
	inf_posicio_aresta.SetValue(3,valor,estat);

	//ijk.k = ijk.k + 1; //ens situem a la label superior.
	if(ijk_0.k+1<mresolucio.k)
	{
		ObtenirValorEstat(ijk_0 + ijk_k,valor,estat);
		sup_posicio_aresta.SetValue(0,valor,estat);

		ObtenirValorEstat(ijk_1 + ijk_k,valor,estat);
		sup_posicio_aresta.SetValue(1,valor,estat);

		ObtenirValorEstat(ijk_2 + ijk_k,valor,estat);
		sup_posicio_aresta.SetValue(2,valor,estat);

		ObtenirValorEstat(ijk_3 + ijk_k,valor,estat);
		sup_posicio_aresta.SetValue(3,valor,estat);
	}
	else
	{
		ObtenirValorEstat(ijk_0,valor,estat);
		sup_posicio_aresta.SetValue(0,valor,estat);

		ObtenirValorEstat(ijk_1,valor,estat);
		sup_posicio_aresta.SetValue(1,valor,estat);

		ObtenirValorEstat(ijk_2,valor,estat);
		sup_posicio_aresta.SetValue(2,valor,estat);

		ObtenirValorEstat(ijk_3,valor,estat);
		sup_posicio_aresta.SetValue(3,valor,estat);
	}

	if (ijk.i==0) { ijk_0.i++; ijk_1.i++; ijk_2.i++; ijk_3.i++; }
	else if (ijk.i==mresolucio.i) { ijk_0.i--; ijk_1.i--; ijk_2.i--; ijk_3.i--; }
	//else ijk.i = pvox.v->IJKVoxel048.i;//IJKi = pvox.v->IJKVoxel048.i;
	if (ijk.j==0) { ijk_0.j++; ijk_1.j++; ijk_2.j++; ijk_3.j++; }
	else if (ijk.j==mresolucio.j) { ijk_0.j--; ijk_1.j--; ijk_2.j--; ijk_3.j--; }
	//else ijk.j = pvox.v->IJKVoxel048.j;//IJKj = pvox.v->IJKVoxel048.j;

	//ijk.k=pvox.v->IJKVoxel048.k; //ens situem a la label inferior.

	if(ijk.i==0 || ijk.i==mresolucio.i || ijk.j==0 || ijk.j==mresolucio.j)
	{
		ObtenirValorEstat(ijk_0,valor,estat);
		inf_posicio.SetValue(0,valor,estat);
		inf_posiciomi.SetValue(1,valor,estat);
		inf_posiciomj.SetValue(2,valor,estat);

		ObtenirValorEstat(ijk_1,valor,estat);
		inf_posicio.SetValue(1,valor,estat);
		inf_posiciopi.SetValue(0,valor,estat);
		inf_posiciomj.SetValue(3,valor,estat); 

		ObtenirValorEstat(ijk_2,valor,estat);
		inf_posicio.SetValue(2,valor,estat);
		inf_posiciomi.SetValue(3,valor,estat);
		inf_posiciopj.SetValue(0,valor,estat);

		ObtenirValorEstat(ijk_3,valor,estat);
		inf_posicio.SetValue(3,valor,estat);
		inf_posiciopi.SetValue(2,valor,estat);
		inf_posiciopj.SetValue(1,valor,estat);
	}
	else
	{
		valor=inf_posicio_aresta.GetValue(0);
		estat=inf_posicio_aresta.GetState(0);
		inf_posicio.SetValue(0,valor,estat);
		inf_posiciomi.SetValue(1,valor,estat);
		inf_posiciomj.SetValue(2,valor,estat);

		valor=inf_posicio_aresta.GetValue(1);
		estat=inf_posicio_aresta.GetState(1);
		inf_posicio.SetValue(1,valor,estat);
		inf_posiciopi.SetValue(0,valor,estat);
		inf_posiciomj.SetValue(3,valor,estat);

		valor=inf_posicio_aresta.GetValue(2);
		estat=inf_posicio_aresta.GetState(2);
		inf_posicio.SetValue(2,valor,estat);
		inf_posiciomi.SetValue(3,valor,estat);
		inf_posiciopj.SetValue(0,valor,estat);

		valor=inf_posicio_aresta.GetValue(3);
		estat=inf_posicio_aresta.GetState(3);
		inf_posicio.SetValue(3,valor,estat);
		inf_posiciopi.SetValue(2,valor,estat);
		inf_posiciopj.SetValue(1,valor,estat);
	}
	ObtenirValorEstat(ijk_0 - ijk_i,valor,estat);
	inf_posiciomi.SetValue(0,valor,estat);

	ObtenirValorEstat(ijk_2 - ijk_i,valor,estat);
	inf_posiciomi.SetValue(2,valor,estat);

	ObtenirValorEstat(ijk_0 - ijk_j,valor,estat);
	inf_posiciomj.SetValue(0,valor,estat);

	ObtenirValorEstat(ijk_1 - ijk_j,valor,estat);
	inf_posiciomj.SetValue(1,valor,estat);

	ObtenirValorEstat(ijk_1 + ijk_i,valor,estat);
	inf_posiciopi.SetValue(1,valor,estat);

	ObtenirValorEstat(ijk_3 + ijk_i,valor,estat);
	inf_posiciopi.SetValue(3,valor,estat);

	ObtenirValorEstat(ijk_2 + ijk_j,valor,estat);
	inf_posiciopj.SetValue(2,valor,estat);

	ObtenirValorEstat(ijk_3 + ijk_j,valor,estat);
	inf_posiciopj.SetValue(3,valor,estat);

	ijk_0.k++; ijk_1.k++; ijk_2.k++; ijk_3.k++; //ens situem a les labels superiors

	if(ijk.i==0 || ijk.i==mresolucio.i || ijk.j==0 || ijk.j==mresolucio.j)
	{
		ObtenirValorEstat(ijk_0,valor,estat);
		sup_posicio.SetValue(0,valor,estat);
		sup_posiciomi.SetValue(1,valor,estat);
		sup_posiciomj.SetValue(2,valor,estat);

		ObtenirValorEstat(ijk_1,valor,estat);
		sup_posicio.SetValue(1,valor,estat);
		sup_posiciopi.SetValue(0,valor,estat);
		sup_posiciomj.SetValue(3,valor,estat);

		ObtenirValorEstat(ijk_2,valor,estat);
		sup_posicio.SetValue(2,valor,estat);
		sup_posiciomi.SetValue(3,valor,estat);
		sup_posiciopj.SetValue(0,valor,estat);

		ObtenirValorEstat(ijk_3,valor,estat);
		sup_posicio.SetValue(3,valor,estat);
		sup_posiciopi.SetValue(2,valor,estat);
		sup_posiciopj.SetValue(1,valor,estat);
	}
	else
	{
		valor=sup_posicio_aresta.GetValue(0);
		estat=sup_posicio_aresta.GetState(0);
		sup_posicio.SetValue(0,valor,estat);
		sup_posiciomi.SetValue(1,valor,estat);
		sup_posiciomj.SetValue(2,valor,estat);

		valor=sup_posicio_aresta.GetValue(1);
		estat=sup_posicio_aresta.GetState(1);
		sup_posicio.SetValue(1,valor,estat);
		sup_posiciopi.SetValue(0,valor,estat);
		sup_posiciomj.SetValue(3,valor,estat);

		valor=sup_posicio_aresta.GetValue(2);
		estat=sup_posicio_aresta.GetState(2);
		sup_posicio.SetValue(2,valor,estat);
		sup_posiciomi.SetValue(3,valor,estat);
		sup_posiciopj.SetValue(0,valor,estat);

		valor=sup_posicio_aresta.GetValue(3);
		estat=sup_posicio_aresta.GetState(3);
		sup_posicio.SetValue(3,valor,estat);
		sup_posiciopi.SetValue(2,valor,estat);
		sup_posiciopj.SetValue(1,valor,estat);
	}

	if(ijk_0.k<mresolucio.k)
	{

		ObtenirValorEstat(ijk_0 - ijk_i,valor,estat);
		sup_posiciomi.SetValue(0,valor,estat);

		ObtenirValorEstat(ijk_2 - ijk_i,valor,estat);
		sup_posiciomi.SetValue(2,valor,estat);

		ObtenirValorEstat(ijk_0 - ijk_j,valor,estat);
		sup_posiciomj.SetValue(0,valor,estat);

		ObtenirValorEstat(ijk_1 - ijk_j,valor,estat);
		sup_posiciomj.SetValue(1,valor,estat);

		ObtenirValorEstat(ijk_1 + ijk_i,valor,estat);
		sup_posiciopi.SetValue(1,valor,estat);

		ObtenirValorEstat(ijk_3 + ijk_i,valor,estat);
		sup_posiciopi.SetValue(3,valor,estat);

		ObtenirValorEstat(ijk_2 + ijk_j,valor,estat);
		sup_posiciopj.SetValue(2,valor,estat);

		ObtenirValorEstat(ijk_3 + ijk_j,valor,estat);
		sup_posiciopj.SetValue(3,valor,estat);

	}
	else
	{
		ObtenirValorEstat(ijk_0 - ijk_i - ijk_k,valor,estat);
		sup_posiciomi.SetValue(0,valor,estat);

		ObtenirValorEstat(ijk_2 - ijk_i - ijk_k,valor,estat);
		sup_posiciomi.SetValue(2,valor,estat);

		ObtenirValorEstat(ijk_0 - ijk_j - ijk_k,valor,estat);
		sup_posiciomj.SetValue(0,valor,estat);

		ObtenirValorEstat(ijk_1 - ijk_j - ijk_k,valor,estat);
		sup_posiciomj.SetValue(1,valor,estat);

		ObtenirValorEstat(ijk_1 + ijk_i - ijk_k,valor,estat);
		sup_posiciopi.SetValue(1,valor,estat);

		ObtenirValorEstat(ijk_3 + ijk_i - ijk_k,valor,estat);
		sup_posiciopi.SetValue(3,valor,estat);

		ObtenirValorEstat(ijk_2 + ijk_j - ijk_k,valor,estat);
		sup_posiciopj.SetValue(2,valor,estat);

		ObtenirValorEstat(ijk_3 + ijk_j - ijk_k,valor,estat);
		sup_posiciopj.SetValue(3,valor,estat);

	}

	if ((ijk_0.k)+1<mresolucio.k)
	{
		ijk_0.k++; ijk_1.k++; ijk_2.k++; ijk_3.k++; //ens situem a les labels posteriors


		ObtenirValorEstat(ijk_0,valor,estat);
		post_posicio.SetValue(0,valor,estat);

		ObtenirValorEstat(ijk_1,valor,estat);
		post_posicio.SetValue(1,valor,estat);

		ObtenirValorEstat(ijk_2,valor,estat);
		post_posicio.SetValue(2,valor,estat);

		ObtenirValorEstat(ijk_3,valor,estat);
		post_posicio.SetValue(3,valor,estat);

	}
	else
	{
		post_posicio = sup_posicio;
	}
	int k_ant = ijk.k-1;

	if(k_ant>=0)
	{
		ijk_0.k=k_ant; ijk_1.k=k_ant; ijk_2.k=k_ant; ijk_3.k=k_ant; //ens situem a la label anterior;

		ObtenirValorEstat(ijk_0,valor,estat);
		ant_posicio.SetValue(0,valor,estat);

		ObtenirValorEstat(ijk_1,valor,estat);
		ant_posicio.SetValue(1,valor,estat);

		ObtenirValorEstat(ijk_2,valor,estat);
		ant_posicio.SetValue(2,valor,estat);

		ObtenirValorEstat(ijk_3,valor,estat);
		ant_posicio.SetValue(3,valor,estat);
	}
	else
	{
		ant_posicio = inf_posicio;
	}

}

//-----------------------------------------------------------------
void MarchingCubes::SetLabelsRecorregut(const Voxel048 &vox)
//-----------------------------------------------------------------
{
	int IJKi, IJKj, posicio;

	//fixar posicio de SetAresta:
	posicio=vox.IJKVoxel048.i/noupas.i + mresolucio.i*vox.IJKVoxel048.j/noupas.j;
	inf_posicio_aresta=mc_llescaInf[posicio];
	sup_posicio_aresta=mc_llescaSup[posicio];

	//fixar posicio de CalculaGradient:
	if (vox.IJKVoxel048.i==0) IJKi = vox.IJKVoxel048.i+noupas.i;
	else if (vox.IJKVoxel048.i==mresolucio.i) IJKi = vox.IJKVoxel048.i-noupas.i;
	else IJKi = vox.IJKVoxel048.i;
	if (vox.IJKVoxel048.j==0) IJKj = vox.IJKVoxel048.j+noupas.j;
	else if (vox.IJKVoxel048.j==mresolucio.j) IJKj = vox.IJKVoxel048.j-noupas.j;
	else IJKj = vox.IJKVoxel048.j;

	posicio = IJKi/noupas.i + mresolucio.i*IJKj/noupas.j;

	if (!mc_llescaAntPlena) { 
		mc_llescaAnt = mc_llescaInf;
		mc_llescaAntPlena = 1;
	}

	inf_posicio   = mc_llescaInf[posicio];
	inf_posiciomi = posicio > 0                                     ? mc_llescaInf[posicio-1]            : mc_llescaInf[posicio];
	inf_posiciopi = posicio < int(mc_llescaInf.size()-1)            ? mc_llescaInf[posicio+1]            : mc_llescaInf[posicio];
	inf_posiciomj = posicio >= mresolucio.i                         ? mc_llescaInf[posicio-mresolucio.i] : mc_llescaInf[posicio];
	inf_posiciopj = posicio < int(mc_llescaInf.size()-mresolucio.i) ? mc_llescaInf[posicio+mresolucio.i] : mc_llescaInf[posicio];

	sup_posicio=mc_llescaSup[posicio];
	sup_posiciomi = posicio > 0                                     ? mc_llescaSup[posicio-1]            : mc_llescaSup[posicio];
	sup_posiciopi = posicio < int(mc_llescaInf.size()-1)            ? mc_llescaSup[posicio+1]            : mc_llescaSup[posicio];
	sup_posiciomj = posicio >= mresolucio.i                         ? mc_llescaSup[posicio-mresolucio.i] : mc_llescaSup[posicio];
	sup_posiciopj = posicio < int(mc_llescaInf.size()-mresolucio.i) ? mc_llescaSup[posicio+mresolucio.i] : mc_llescaSup[posicio];

	post_posicio=mc_llescaPost[posicio];
	ant_posicio=mc_llescaAnt[posicio];
}     
/*
float MarchingCubes::CalcularSeparacioLlesques(int ll0,int ll1)
{
float result;
LGPt3d p0,p1;
p0 = mCaptacio->GetpLlesca(ll0)->GetPosicioLlesca();
p1 = mCaptacio->GetpLlesca(ll1)->GetPosicioLlesca();

result = p0.Distance(p1);
//  result = mCaptacio->GetpLlesca(ll0)->GetSliceLocalization() - mCaptacio->GetpLlesca(ll1)->GetSliceLocalization();
if (result == 0) // estava tot en un fitxer
result = mCaptacio->GetpLlesca(ll0)->GetGruixLlesca();

if (result<0) result = result * -1.;

cout<<"CalcularSeparacioLlesques = "<<result<<endl;
return result;
}
*/

float InterpolacioLinialPunt(float p0, float p1, const RangValors &threshold)
{
    float den,res;

	// Calcul del valor interpolat: (Valor0 - th)/(Valor0 - Valor1)
	// Com tenim un rang, ens quedarem amb el valor correcte (0<=Valor<=1) 

	den = (p0 - p1);

	if (!den) res = 0.5;
	else {
		res=(p0 - threshold.min)/den;
		if ((res<0.0) || (res>1.0)) {
			res=(p0 - threshold.max)/den;
		}
	}

	if ((res<0.0) || (res>1.0)) {
		//fprintf(stderr,"Cagada en 0. p0= %f, p1= %f ==> comp= %f threshold.max= %f threshold.min= %f\n",p0,p1,res,threshold.max,threshold.min);
		res = 0.5;
	}

	if (res<float(MIN_RATIO)) res=float(MIN_RATIO);
	else
		if (res>float(MAX_RATIO)) res=float(MAX_RATIO);

	return res;
}

//hector
void MarchingCubes::Run (vector<double> &vertexs, vector<double> &normals, vector<unsigned int> &cares, InfoVoxel* voxels_iso) 
{    
	ini_patrons ();
	mc_llescaInfPlena=0;
	mc_llescaSupPlena=0;
	mc_llescaAntPlena=0;
	mc_llescaPostPlena=0;

	mc_llescaInf.clear();
	mc_llescaSup.clear();
	mc_llescaActual.clear();
	mc_llescaAnt.clear();
	mc_llescaPost.clear();
	//PuntsIntIsoVox.clear();

	retoca_interpolacio = 0; //chapuza per interpolacio linial
	recorregut=true;

	int posicions = mresolucio.i * mresolucio.j;
	mc_llescaActual = vector<Label> (posicions);


	//posicions = posicions * mresolucio.k;//esto no esta en el otro

	//PuntsIntIsoVox = vector <pVoxel048> (posicions);

	//ESTO LO HACE EN EL OTRO RUN
	PuntsIntIsoVox.push_back(vector<Voxel048>(posicions));
	PuntsIntIsoVox.push_back(vector<Voxel048>(posicions));
	PuntsIntIsoVox.push_back(vector<Voxel048>(posicions));


	IJK primer(0, 0, 0);
	IJK ultim (mResolucioMon.i-1, mResolucioMon.j-1, mResolucioMon.k-1);

	mVoxelsIso = voxels_iso;

	//hector SOLO FUNCIONARA PARA MON VOXELES 
	mTamX = mResolucioMon.i;
	mTamY = mResolucioMon.j;
	mTamZ = mResolucioMon.k;

	printf("RESOLUCION MARCHING %d %d %d\n",mResolucioMon.i,mResolucioMon.j,mResolucioMon.k);
	/*mTamX = (int)(((float)mResolucioMon.i/(float)(*mVoxelsIso).size())+.5);
	mTamY = (int)(((float)mResolucioMon.j/(float)(*mVoxelsIso)[0].size())+.5);
	mTamZ = (int)(((float)mResolucioMon.k/(float)(*mVoxelsIso)[0][0].size())+.5);
	*/
    {
		void *parametres[4];
		parametres[0] = this;
		parametres[1] = &vertexs;
		parametres[2] = &normals;
		parametres[3] = &cares;

        for (int k = primer.k; k <= ultim.k; k++) {
            for (int i = primer.i; i <= ultim.i; i++) {
                for (int j = primer.j; j <= ultim.j; j++) {
                    this->f_rec_Captacio(mGrid, IJK(i, j, k), parametres);
                }
            }
        }
    }


#ifdef BENCHMARKING_MC
	cerr << "patro 0 " << vp0 << "\n patro 1 " << vp1 << "\n patro 2 " << vp2 << "\n patro 3 " << vp3 << "\n patro 4 " << vp4 
		<< "\n patro 5 " << vp5 << "\n patro 6 " << vp6 << "\n patro 7 " << vp7 << "\n patro 8 " << vp8 << "\n patro 9 " 
		<< vp9 << "\n patro 10 " << vp10 << "\n patro 11 " << vp11 << "\n patro 12 " << vp12 << "\n patro 13 " << vp13 
		<< "\n patro 14 " << vp14 << "\n";
	cerr << "Discret 3:" << voxelspatro3 << "\nDiscret 6:" << voxelspatro6 << "\nDiscret 7:" << voxelspatro7 << "\n";
	cerr << "Quants voxels frontera " << vp0 + vp1 + vp3 + vp4 + vp5 + vp6 + vp7 + vp8 + vp9 + vp10 + vp11+ vp12 + vp13 + vp14 +
		voxelspatro3 + voxelspatro6 + voxelspatro7 << endl;
#endif
}


