#include "MC/Label.h"


Label::Label(IJK IJK_Labeli,float v0i,float v1i,float v2i,float v3i)
{
  IJK_Label=IJK_Labeli;
  //v0=v0i; v1=v1i; v2=v2i; v3=v3i;
  valors[0]=v0i;
  valors[1]=v1i;
  valors[2]=v2i;
  valors[3]=v3i;
}

void Label::SetValue(int posicio,float valor, bool estat)
{
  /*
  switch(posicio) {
    case 0: v0=valor; s0=estat; break;
    case 1: v1=valor; s1=estat; break;
    case 2: v2=valor; s2=estat; break;
    case 3: v3=valor; s3=estat; break;
  }*/

  valors[posicio]=valor;
  estats[posicio]=estat;
}

void Label::SetIJKLabel(IJK IJK_Labeli)
{
   IJK_Label=IJK_Labeli;
}

float Label::GetValue(int posicio)
{
  
/*  float valor=0.0;

  switch(posicio) {
    case 0: valor=v0; break;
    case 1: valor=v1; break;
    case 2: valor=v2; break;
    case 3: valor=v3; break;
  }

  return valor;*/
  return valors[posicio];
}

bool Label::GetState(int posicio)
{
 
/*  bool estat=false;

  switch(posicio) {
    case 0: estat=s0; break;
    case 1: estat=s1; break;
    case 2: estat=s2; break;
    case 3: estat=s3; break;
  }

  return estat;*/
  
  return estats[posicio];
}
