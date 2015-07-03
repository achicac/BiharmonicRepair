#ifndef __LABEL_H__
#define __LABEL_H__

#include <MC/IJK.h>


class Label {
    IJK IJK_Label;
    //float v0,v1,v2,v3;
    //bool s0,s1,s2,s3;
    float valors[4];
    bool estats[4];

public:
    Label(){}
    Label(IJK IJK_Labeli,float v0i,float v1i,float v2i,float v3i);
    //Label operator=(const Label& l);

    // Modificadors
    void SetValue(int posicio,float valor, bool estat);
    void SetIJKLabel(IJK IJK_Labeli);

    // Consultors
    float GetValue(int posicio);
    bool  GetState(int posicio);
    IJK   GetIJKLabel(void) {return IJK_Label;}

};

#endif

//-- eof Label.h
