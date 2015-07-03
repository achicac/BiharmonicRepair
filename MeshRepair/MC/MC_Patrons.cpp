#include <MC/MarchingCubes.h>

/*vector<int>*/ void MarchingCubes::BuscaPatro(int index_voxel,int &num_patro,int &orient)
{
    //vector<int> ta;
    
  /*  if (index_voxel==0 || index_voxel==255)
	{
		num_patro=0;
		return;
	}*/

	switch(index_voxel){
   
    case 0:num_patro=0;break;
    case 255:num_patro=0;break;

// PATRO 1

//e1,e2,e3
   case 1:ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),8);num_patro=1;orient=1;break;
   case 2:ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),5);num_patro=1;orient=1;break;
   case 4:ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),4);num_patro=1;orient=1;break;
   case 8:ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),11);num_patro=1;orient=1;break;
   case 16:ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),6);num_patro=1;orient=1;break;
   case 32:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);num_patro=1;orient=1;break;
   case 64:ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);num_patro=1;orient=1;break;   
   case 128:ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),7);num_patro=1;orient=1;break;   

//e1,e3,e2        
   case 254:ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),8);num_patro=1;orient=-1;break;
   case 253:ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),5);num_patro=1;orient=-1;break;
   case 251:ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),4);num_patro=1;orient=-1;break;
   case 247:ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),11);num_patro=1;orient=-1;break;
   case 239:ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),6);num_patro=1;orient=-1;break;
   case 223:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);num_patro=1;orient=-1;break;
   case 191:ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);num_patro=1;orient=-1;break;   
   case 127:ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),7);num_patro=1;orient=-1;break;   

// PATRO 2

//e1,e2,e3,e4            
   case 3:ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),5);num_patro=2;orient=1;break;
   case 5:ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),8);num_patro=2;orient=1;break;
   case 10:ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),1);num_patro=2;orient=1;break;
   case 12:ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),11);ta.insert(ta.end(),10);num_patro=2;orient=1;break;
   case 17:ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),6);ta.insert(ta.end(),2);num_patro=2;orient=1;break;
   case 34:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),0);num_patro=2;orient=1;break;
   case 48:ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),8);num_patro=2;orient=1;break; 
   case 68:ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),3);num_patro=2;orient=1;break; 
   case 80:ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),10);num_patro=2;orient=1;break;    
   case 136:ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),7);ta.insert(ta.end(),3);num_patro=2;orient=1;break;  
   case 160:ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),9);num_patro=2;orient=1;break;  
   case 192:ta.insert(ta.end(),11);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),10);num_patro=2;orient=1;break; 
//e1,e4,e3,e2   
   case 252:ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),5);num_patro=2;orient=-1;break;
   case 250:ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),8);num_patro=2;orient=-1;break;
   case 245:ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),1);num_patro=2;orient=-1;break;
   case 243:ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),11);ta.insert(ta.end(),10);num_patro=2;orient=-1;break;
   case 238:ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),6);ta.insert(ta.end(),2);num_patro=2;orient=-1;break;
   case 221:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),0);num_patro=2;orient=-1;break;
   case 207:ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),8);num_patro=2;orient=-1;break; 
   case 187:ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),3);num_patro=2;orient=-1;break; 
   case 175:ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),10);num_patro=2;orient=-1;break;    
   case 119:ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),7);ta.insert(ta.end(),3);num_patro=2;orient=-1;break;  
   case 95:ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),9);num_patro=2;orient=-1;break;  
   case 63:ta.insert(ta.end(),11);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),10);num_patro=2;orient=-1;break;
 
// PATRO 3

//e1,e2,e3 i e4,e5,e6

   case 6:ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1);num_patro=3;orient=1;break;
   case 9:ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4);num_patro=3;orient=1;break;
   case 18:ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);num_patro=3;orient=1;break;
   case 20:ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);num_patro=3;orient=1;break;
   case 33:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4);num_patro=3;orient=1;break;
   case 40:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5);num_patro=3;orient=1;break;
   case 65:ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4);num_patro=3;orient=1;break;
   case 72:ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);num_patro=3;orient=1;break;
   case 96:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);num_patro=3;orient=1;break;
   case 130:ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11);num_patro=3;orient=1;break;
   case 132:ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1);num_patro=3;orient=1;break;
   case 144:ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);num_patro=3;orient=1;break;

//e1,e3,e2 i e4,e6,e5   
   

   case 249:ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1);num_patro=3;orient=-1;
            voxelspatro3++;break;    
   case 246:ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4);num_patro=3;orient=-1;
            voxelspatro3++;break;
   case 237:ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);num_patro=3;orient=-1;
            voxelspatro3++;break;    
   case 235:ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),6);num_patro=3;orient=-1;
            voxelspatro3++;break;    
   case 222:ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),2);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),0);num_patro=3;orient=-1;
            voxelspatro3++;break;    
   case 215:ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),11);ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);num_patro=3;orient=-1;
            voxelspatro3++;break;    
   case 190:ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),8);num_patro=3;orient=-1;
            voxelspatro3++;break;    
   case 183:ta.insert(ta.end(),5);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),6);ta.insert(ta.end(),10);ta.insert(ta.end(),3);num_patro=3;orient=-1;
            voxelspatro3++;break;    
   case 159:ta.insert(ta.end(),9);ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),10);ta.insert(ta.end(),3);ta.insert(ta.end(),6);num_patro=3;orient=-1;
            voxelspatro3++;break;    
   case 125:ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),7);num_patro=3;orient=-1;
            voxelspatro3++;break;    
   case 123:ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),10);num_patro=3;orient=-1;
            voxelspatro3++;break;    
   case 111:ta.insert(ta.end(),11);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),8);ta.insert(ta.end(),6);ta.insert(ta.end(),2);num_patro=3;orient=-1;
            voxelspatro3++;break;    


// PATRO 4

  // MODIFICAT SERGI

//e1,e2,e3 i e4,e5,e6

   case 24:ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);num_patro=4;orient=1;break; 
   case 36:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1);num_patro=4;orient=1;break; 
   case 66:ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);num_patro=4;orient=1;break; 
   case 129:ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4);num_patro=4;orient=1;break;

//e1,e3,e2 i e4,e6,e5         

   case 126:ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4);num_patro=4;orient=-1;break;
   case 189:ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);num_patro=4;orient=-1;break;
   case 219:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1);num_patro=4;orient=-1;break;
   case 231:ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);num_patro=4;orient=-1;break;


// PATRO 5

// Les cares es formen e1,e2,e3 i e3,e4,e5,e1

   case 7: ta.insert(ta.end(),10);ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),1);num_patro=5;orient=1;break;
   case 11:ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),4);num_patro=5;orient=1;break;
   case 13:ta.insert(ta.end(),11);ta.insert(ta.end(),10);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),5);num_patro=5;orient=1;break;
   case 14:ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),0);num_patro=5;orient=1;break;
   case 19:ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),9);num_patro=5;orient=1;break; 
   case 21:ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),6);num_patro=5;orient=1;break; 
   case 35:ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),2);num_patro=5;orient=1;break;
   case 42:ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),11);num_patro=5;orient=1;break; 
   case 49:ta.insert(ta.end(),4);ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),0);num_patro=5;orient=1;break; 
   case 50:ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),8);num_patro=5;orient=1;break; 
   case 69:ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),8);num_patro=5;orient=1;break; 
   case 76:ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),11);ta.insert(ta.end(),3);num_patro=5;orient=1;break ;
   case 81:ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),10);num_patro=5;orient=1;break ; 
   case 84:ta.insert(ta.end(),1);ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),4);num_patro=5;orient=1;break; 
   case 112:ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),3);ta.insert(ta.end(),7);num_patro=5;orient=1;break; 
   case 138:ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),7);num_patro=5;orient=1;break; 
   case 140:ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),10);num_patro=5;orient=1;break;
   case 162:ta.insert(ta.end(),0);ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),5);num_patro=5;orient=1;break;
   case 168:ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),9);num_patro=5;orient=1;break;
   case 176:ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),6);ta.insert(ta.end(),3);num_patro=5;orient=1;break;
   case 196:ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),11);num_patro=5;orient=1;break;
   case 200:ta.insert(ta.end(),5);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),10);ta.insert(ta.end(),1);num_patro=5;orient=1;break;
   case 208:ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),7);ta.insert(ta.end(),2);num_patro= 5;orient=1;break;
   case 224:ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),2);ta.insert(ta.end(),6);num_patro= 5;orient=1;break;

// invers e1,e3,e2 i e1,e5,e4,e3

   case 248: ta.insert(ta.end(),10);ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),1);num_patro=5;orient=-1;break;
   case 244:ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),4);num_patro=5;orient=-1;break;
   case 242:ta.insert(ta.end(),11);ta.insert(ta.end(),10);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),5);num_patro=5;orient=-1;break;
   case 241:ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),0);num_patro=5;orient=-1;break;
   case 236:ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),9);num_patro=5;orient=-1;break; 
   case 234:ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),6);num_patro=5;orient=-1;break; 
   case 220:ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),2);num_patro=5;orient=-1;break;
   case 213:ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),11);num_patro=5;orient=-1;break; 
   case 206:ta.insert(ta.end(),4);ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),0);num_patro=5;orient=-1;break; 
   case 205:ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),8);num_patro=5;orient=-1;break; 
   case 186:ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),8);num_patro=5;orient=-1;break; 
   case 179:ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),11);ta.insert(ta.end(),3);num_patro=5;orient=-1;break ;
   case 174:ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),10);num_patro=5;orient=-1;break ; 
   case 171:ta.insert(ta.end(),1);ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),4);num_patro=5;orient=-1;break; 
   case 143:ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),3);ta.insert(ta.end(),7);num_patro=5;orient=-1;break; 
   case 117:ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),7);num_patro=5;orient=-1;break; 
   case 115:ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),10);num_patro=5;orient=-1;break;
   case 93:ta.insert(ta.end(),0);ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),5);num_patro=5;orient=-1;break;
   case 87:ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),9);num_patro=5;orient=-1;break;
   case 79:ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),6);ta.insert(ta.end(),3);num_patro=5;orient=-1;break;
   case 59:ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),11);num_patro=5;orient=-1;break;
   case 55:ta.insert(ta.end(),5);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),10);ta.insert(ta.end(),1);num_patro=5;orient=-1;break;
   case 47:ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),7);ta.insert(ta.end(),2);num_patro= 5;orient=-1;break;
   case 31:ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),2);ta.insert(ta.end(),6);num_patro= 5;orient=-1;break;

// PATRO 6
           
//e1,e2,e3,e4 i e5,e6,e7
        
   case 25:ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5); ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),4);
           num_patro=6;orient=1;break;
   case 26:ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8); ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),9);
           num_patro=6;orient=1;break;
   case 28:ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8); ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),11);
           num_patro=6;orient=1;break;
   case 37:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9); ta.insert(ta.end(),10);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),1);
           num_patro=6;orient=1;break;
   case 38:ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1); ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),0);
           num_patro=6;orient=1;break;
   case 44:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9); ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),11);
           num_patro=6;orient=1;break;
   case 52:ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1); ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),8);
           num_patro= 6;orient=1;break;
   case 56:ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5); ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),8);
           num_patro=6;orient=1;break;
   case 67:ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10); ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),9);
           num_patro=6;orient=1;break;
   case 70:ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0); ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),3);
           num_patro=6;orient=1;break;
   case 74:ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10); ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),9);
           num_patro= 6;orient=1;break;
   case 82:ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0); ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),10);
           num_patro=6;orient=1;break;
   case 88:ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5); ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),10);
           num_patro= 6;orient=1;break;
   case 98:ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10); ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),0);
           num_patro=6;orient=1;break;
   case 100:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9); ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),3);
            num_patro=6;orient=1;break;
   case 131:ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11); ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),9);
            num_patro=6;orient=1;break;
   case 133:ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11); ta.insert(ta.end(),10);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),1);
            num_patro= 6;orient=1;break;
   case 137:ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),8); ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),7); // SERGI
            num_patro=6;orient=1;break;
   case 145:ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11); ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),4);
            num_patro= 6;orient=1;break;
   case 152:ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8); ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),7);
            num_patro=6;orient=1;break;
   case 161:ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4); ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),2);
            num_patro= 6;orient=1;break;
   case 164:ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1); ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),2);
            num_patro= 6;orient=1;break;
   case 193:ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4); ta.insert(ta.end(),11);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
            num_patro=6;orient=1;break;
   case 194:ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),11);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
            num_patro=6;orient=1;break;
       


   case 230:ta.insert(ta.end(),1);ta.insert(ta.end(),11);ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),0);
            num_patro=6;orient=-1;break;
   case 229:ta.insert(ta.end(),2);ta.insert(ta.end(),6);ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),0); 
            num_patro=6;orient=-1;break;
   case 227:ta.insert(ta.end(),8);ta.insert(ta.end(),2);ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),11);ta.insert(ta.end(),10); 
            num_patro=6;orient=-1;break;
   case 218:ta.insert(ta.end(),9);ta.insert(ta.end(),7);ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),8); 
            num_patro=6;orient=-1;break;
   case 217:ta.insert(ta.end(),4);ta.insert(ta.end(),10);ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),5); 
            num_patro=6;orient=-1;break;
   case 211:ta.insert(ta.end(),7);ta.insert(ta.end(),2);ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),5); 
            num_patro=6;orient=-1;break;
   case 203:ta.insert(ta.end(),10);ta.insert(ta.end(),1);ta.insert(ta.end(),4);ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),8); 
            num_patro= 6;orient=-1;break;
   case 199:ta.insert(ta.end(),5);ta.insert(ta.end(),1);ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),6);ta.insert(ta.end(),7); 
            num_patro=6;orient=-1;break;
   case 188:ta.insert(ta.end(),6);ta.insert(ta.end(),3);ta.insert(ta.end(),10);ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),4); 
            num_patro=6;orient=-1;break;
   case 185:ta.insert(ta.end(),5);ta.insert(ta.end(),9);ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),4); 
            num_patro=6;orient=-1;break;
   case 181:ta.insert(ta.end(),10);ta.insert(ta.end(),6);ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),11); 
            num_patro= 6;orient=-1;break;
   case 173:ta.insert(ta.end(),0);ta.insert(ta.end(),5);ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),3);ta.insert(ta.end(),2);
            num_patro=6;orient=-1;break;
   case 167:ta.insert(ta.end(),11);ta.insert(ta.end(),5);ta.insert(ta.end(),1);ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),10); 
            num_patro= 6;orient=-1;break;
   case 157:ta.insert(ta.end(),3);ta.insert(ta.end(),10);ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),2); 
            num_patro=6;orient=-1;break;
   case 155:ta.insert(ta.end(),2);ta.insert(ta.end(),9);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),1); ta.insert(ta.end(),3);   //SERGI
            num_patro=6;orient=-1;break;
   case 124:ta.insert(ta.end(),11);ta.insert(ta.end(),3);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),9); 
            num_patro=6;orient=-1;break;
   case 122:ta.insert(ta.end(),3);ta.insert(ta.end(),7);ta.insert(ta.end(),11);ta.insert(ta.end(),10);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),1); 
            num_patro= 6;orient=-1;break;
   case 118:ta.insert(ta.end(),0);ta.insert(ta.end(),8);ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),1);
            num_patro=6;orient=-1;break;
   case 110:ta.insert(ta.end(),7);ta.insert(ta.end(),11);ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),6); 
            num_patro= 6;orient=-1;break;
   case 103:ta.insert(ta.end(),6);ta.insert(ta.end(),8);ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),7); 
            num_patro=6;orient=-1;break;
   case 94:ta.insert(ta.end(),8);ta.insert(ta.end(),4);ta.insert(ta.end(),0);ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),9); 
           num_patro= 6;orient=-1;break;
   case 91:ta.insert(ta.end(),1);ta.insert(ta.end(),4);ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),2);ta.insert(ta.end(),3);
           num_patro= 6;orient=-1;break;
   case 62:ta.insert(ta.end(),4);ta.insert(ta.end(),0);ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),7);ta.insert(ta.end(),6); 
           num_patro=6;orient=-1;break;
   case 61:ta.insert(ta.end(),9);ta.insert(ta.end(),0);ta.insert(ta.end(),5);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),10);ta.insert(ta.end(),11); 
           num_patro=6;orient=-1;break;

// PATRO 7
            
// e1,e2,e3 e4,e5,e6 i e7,e8,e9

   case 22:ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);
           num_patro=7;orient=1;break;
   case 41:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),11); 
           num_patro=7;orient=1;break;
   case 73:ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),1); ta.insert(ta.end(),5);ta.insert(ta.end(),11);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
           num_patro=7;orient=1;break;
   case 97:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
           num_patro=7;orient=1;break;
   case 104:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),11);
            num_patro= 7;orient=1;break;
   case 134:ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1);
            num_patro=7;orient=1;break;
   case 146:ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);
            num_patro=7;orient=1;break;
   case 148:ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);
            num_patro=7;orient=1;break;

// e1,e3,e2 e4,e6,e5 i e7,e9,e8

   case 233:ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),8);ta.insert(ta.end(),6);ta.insert(ta.end(),2);
            num_patro=7;orient=-1;break;
   case 214:ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),11);ta.insert(ta.end(),1);
            num_patro=7;orient=-1;break;
   case 182:ta.insert(ta.end(),10);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),11);
            num_patro=7;orient=-1;break;
   case 158:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),6);ta.insert(ta.end(),10);ta.insert(ta.end(),3);
            num_patro=7;orient=-1;break;
   case 151:ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
            num_patro= 7;orient=-1;break;
   case 121:ta.insert(ta.end(),11);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),9);
            num_patro=7;orient=-1;break;
   case 109:ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11);
            num_patro=7;orient=-1;break;
   case 107:ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);
            num_patro=7;orient=-1;break;

// PATRO 8

//e1,e2,e3,e4

   case 15:ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),10);num_patro=8;orient=1;break;
   case 51:ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),4);num_patro=8;orient=1;break;
   case 85:ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),1);num_patro=8;orient=1;break;
       
   case 240:ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),10);num_patro=8;orient=-1;break;
   case 204:ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),4);num_patro=8;orient=-1;break;
   case 170:ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),1);num_patro=8;orient=-1;break;     
     //case 240:ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),9);num_patro=8;orient=-1;break;
     //case 204:ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),7);num_patro=8;orient=-1;break;
     //case 170:ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),2);num_patro=8;orient=-1;break;
        
// PATRO 9

//e1,e2,e3,e4,e5,e6         
   case 23:ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),6);ta.insert(ta.end(),2);num_patro=9;orient=1;break;
   case 43:ta.insert(ta.end(),7);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),2);num_patro=9;orient=1;break;
   case 77:ta.insert(ta.end(),5);ta.insert(ta.end(),11);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),8);ta.insert(ta.end(),0);num_patro=9;orient=1;break;
   case 113:ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),10);ta.insert(ta.end(),3);num_patro=9;orient=1;break;
//e6,e5,e4,e3,e2,e1
   case 232:ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),6);ta.insert(ta.end(),2);num_patro=9;orient=-1;break;
   case 212:ta.insert(ta.end(),7);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),2);num_patro=9;orient=-1;break;
   case 178:ta.insert(ta.end(),5);ta.insert(ta.end(),11);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),8);ta.insert(ta.end(),0);num_patro=9;orient=-1;break;
   case 142:ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),10);ta.insert(ta.end(),3);num_patro=9;orient=-1;break;
         
// PATRO 10

//e1,e2,e3,e4 i e5,e6,e7,e8 
   case 60:ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),11);ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),8); //SERGI
           num_patro=10;orient=1;break;
   case 90:ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),9); //SERGI
           num_patro=10;orient=1;break;
   case 102:ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),2); //SERGI
            num_patro=10;orient=1;break;
 
   case 195:ta.insert(ta.end(),11);ta.insert(ta.end(),10);ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),9);ta.insert(ta.end(),8); //SERGI
            num_patro=10;orient=-1;break;
   case 165:ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),10);ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),8); //SERGI
            num_patro=10;orient=-1;break;
   case 153:ta.insert(ta.end(),3);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),1);ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),0);ta.insert(ta.end(),2); //SERGI
            num_patro=10;orient=-1;break;  

// PATRO 11 

// (e1,e2,e3) (e4,e5,e6) (e7,e8,e9) i (e10,e11,e12)
   case 27:ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),9);ta.insert(ta.end(),1);ta.insert(ta.end(),6);
           ta.insert(ta.end(),6);ta.insert(ta.end(),1);ta.insert(ta.end(),4);ta.insert(ta.end(),2);ta.insert(ta.end(),9);ta.insert(ta.end(),6);
	   num_patro=11;orient=1;break;
   case 114:ta.insert(ta.end(),10);ta.insert(ta.end(),0);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),10);ta.insert(ta.end(),7);
            ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),10);ta.insert(ta.end(),3);ta.insert(ta.end(),7);
	    num_patro=11;orient=1;break;
   case 163:ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),2);ta.insert(ta.end(),11);ta.insert(ta.end(),4);
            ta.insert(ta.end(),4);ta.insert(ta.end(),11);ta.insert(ta.end(),5);ta.insert(ta.end(),8);ta.insert(ta.end(),2);ta.insert(ta.end(),4);
	    num_patro=11;orient=1;break;
   case 184:ta.insert(ta.end(),8);ta.insert(ta.end(),5);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),8);ta.insert(ta.end(),3);
            ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),8);ta.insert(ta.end(),6);ta.insert(ta.end(),3);
	    num_patro=11;orient=1;break;
   case 202:ta.insert(ta.end(),6);ta.insert(ta.end(),9);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),1);ta.insert(ta.end(),0);
            ta.insert(ta.end(),9);ta.insert(ta.end(),6);ta.insert(ta.end(),1);ta.insert(ta.end(),1);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
	    num_patro=11;orient=1;break;
   case 209:ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),2);ta.insert(ta.end(),4);ta.insert(ta.end(),11);
            ta.insert(ta.end(),11);ta.insert(ta.end(),4);ta.insert(ta.end(),10);ta.insert(ta.end(),7);ta.insert(ta.end(),2);ta.insert(ta.end(),11);
	    num_patro=11;orient=1;break;

   case 228:ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),9);ta.insert(ta.end(),1);ta.insert(ta.end(),6);
            ta.insert(ta.end(),6);ta.insert(ta.end(),1);ta.insert(ta.end(),4);ta.insert(ta.end(),2);ta.insert(ta.end(),9);ta.insert(ta.end(),6);
	    num_patro=11;orient=-1;break;
   case 141:ta.insert(ta.end(),10);ta.insert(ta.end(),0);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),10);ta.insert(ta.end(),7);
            ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),10);ta.insert(ta.end(),3);ta.insert(ta.end(),7);
	    num_patro=11;orient=-1;break;
   case 92:ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),2);ta.insert(ta.end(),11);ta.insert(ta.end(),4);
           ta.insert(ta.end(),4);ta.insert(ta.end(),11);ta.insert(ta.end(),5);ta.insert(ta.end(),8);ta.insert(ta.end(),2);ta.insert(ta.end(),4);
	   num_patro=11;orient=-1;break;
   case 71:ta.insert(ta.end(),8);ta.insert(ta.end(),5);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),8);ta.insert(ta.end(),3);
           ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),8);ta.insert(ta.end(),6);ta.insert(ta.end(),3);
	   num_patro=11;orient=-1;break;
   case 53:ta.insert(ta.end(),6);ta.insert(ta.end(),9);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),1);ta.insert(ta.end(),0);
           ta.insert(ta.end(),9);ta.insert(ta.end(),6);ta.insert(ta.end(),1);ta.insert(ta.end(),1);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
	   num_patro=11;orient=-1;break;
   case 46:ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),2);ta.insert(ta.end(),4);ta.insert(ta.end(),11);
           ta.insert(ta.end(),11);ta.insert(ta.end(),4);ta.insert(ta.end(),10);ta.insert(ta.end(),7);ta.insert(ta.end(),2);ta.insert(ta.end(),11);
	   num_patro=11;orient=-1;break; 


// PATRO 12

   case 57:ta.insert(ta.end(),4);ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),0);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5);
           num_patro=12;orient=1;break;
   case 75:ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),4);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
           num_patro=12;orient=1;break;
   case 89:ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5);
           num_patro=12;orient=1;break;
   case 99:ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
           num_patro=12;orient=1;break;
   case 106:ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),11);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
            num_patro=12;orient=1;break;
   case 120:ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),3);ta.insert(ta.end(),7);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5);
            num_patro=12;orient=1;break;
   case 147:ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),9);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11);
            num_patro=12;orient=1;break;
   case 154:ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);
            num_patro=12;orient=1;break;
   case 169:ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4);
            num_patro=12;orient=1;break;
   case 201:ta.insert(ta.end(),5);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),10);ta.insert(ta.end(),1);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4);
            num_patro=12;orient=1;break;
   case 210:ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),7);ta.insert(ta.end(),2);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);
            num_patro=12;orient=1;break;
   case 225:ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),2);ta.insert(ta.end(),6);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4);
            num_patro=12;orient=1;break;
    
     case 198:
// ta.insert(ta.end(),4);ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),0);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5);
        //ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),11);
        ta.insert(ta.end(),1);ta.insert(ta.end(),11);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),5);
        num_patro=12;orient=1;break;
     case 180:
// ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),4);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
//ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),1);ta.insert(ta.end(),3);
        ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),6);ta.insert(ta.end(),3);ta.insert(ta.end(),4);ta.insert(ta.end(),1);
        ta.insert(ta.end(),10);
        num_patro=12;orient=1;
        break;
   case 166:
// ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5);
      ta.insert(ta.end(),11);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1);
      num_patro=12;orient=1;
      break;
     case 156:
// ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
        //    ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),7);ta.insert(ta.end(),10);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),3);
         ta.insert(ta.end(),5);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),6);
        num_patro=12;orient=1;
        break;
     case 149:
// ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),11);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
        ta.insert(ta.end(),2);ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),6);ta.insert(ta.end(),11);ta.insert(ta.end(),7);
        ta.insert(ta.end(),3);
        num_patro=12;orient=1;
        break;
     case 135:
// ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),3);ta.insert(ta.end(),7);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),5);
        ta.insert(ta.end(),10);ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),1);ta.insert(ta.end(),7);ta.insert(ta.end(),3);
        ta.insert(ta.end(),11);
        num_patro=12;orient=1;
break;
     case 108:
// ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),9);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11);
        ta.insert(ta.end(),6);ta.insert(ta.end(),4);ta.insert(ta.end(),5);ta.insert(ta.end(),11);ta.insert(ta.end(),3);ta.insert(ta.end(),9);ta.insert(ta.end(),2);ta.insert(ta.end(),7);
        num_patro=12;orient=1;
        break;
     case 101:
// ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),0);ta.insert(ta.end(),9);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);
// ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),3);ta.insert(ta.end(),8);ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),6);
        //  ta.insert(ta.end(),9);ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),3);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),8);ta.insert(ta.end(),2);
        //  ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),8);ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9); --> ja és correcta
         ta.insert(ta.end(),6);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),1);ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);
        num_patro=12;orient=1;
        break;
     case 86:
//ta.insert(ta.end(),2);ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),9);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4);
        ta.insert(ta.end(),1);ta.insert(ta.end(),3);ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),4);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);
        num_patro=12;orient=1;
        break;
   case 54:
// ta.insert(ta.end(),5);ta.insert(ta.end(),7);ta.insert(ta.end(),6);ta.insert(ta.end(),10);ta.insert(ta.end(),1);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4);
      ta.insert(ta.end(),0);ta.insert(ta.end(),8);ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),5);ta.insert(ta.end(),1);ta.insert(ta.end(),10);ta.insert(ta.end(),4);
      num_patro=12;orient=1;
      break;
     case 45:
// ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),7);ta.insert(ta.end(),2);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);
        ta.insert(ta.end(),11);ta.insert(ta.end(),10);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),5);ta.insert(ta.end(),2);ta.insert(ta.end(),7);
        ta.insert(ta.end(),9);
        num_patro=12;orient=1;
        break;
     case 30:
// ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),2);ta.insert(ta.end(),6);ta.insert(ta.end(),8);ta.insert(ta.end(),0);ta.insert(ta.end(),4);
        ta.insert(ta.end(),9);ta.insert(ta.end(),11);ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),0);ta.insert(ta.end(),6);ta.insert(ta.end(),2);
        ta.insert(ta.end(),8);
        num_patro=12;orient=1;
        break;
// PATRO 13

// (e1,e2,e3) (e4,e5,e6) (e7,e8,e8) i (e9,e10,e11)
    case 105:ta.insert(ta.end(),2);ta.insert(ta.end(),7);ta.insert(ta.end(),9);ta.insert(ta.end(),3);ta.insert(ta.end(),6);ta.insert(ta.end(),10);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),8);
             ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),11);num_patro=13;orient=1;break;
    case 150:ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),11);ta.insert(ta.end(),6);ta.insert(ta.end(),2);ta.insert(ta.end(),8);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),0);
             ta.insert(ta.end(),10);ta.insert(ta.end(),4);ta.insert(ta.end(),1);num_patro=13;orient=1;break;

//PATRO 14 sense simplificar

// (e1,e2,e3) (e4,e5,e6) (e7,e8,e9) i (e10,e11,e12)
   case 58:ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),11);ta.insert(ta.end(),6);ta.insert(ta.end(),11);ta.insert(ta.end(),0);
           ta.insert(ta.end(),0);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),8);ta.insert(ta.end(),6);ta.insert(ta.end(),0);
	   num_patro=14;orient=1;break;
   case 83:ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),10);ta.insert(ta.end(),5);ta.insert(ta.end(),10);ta.insert(ta.end(),2);
           ta.insert(ta.end(),2);ta.insert(ta.end(),10);ta.insert(ta.end(),3);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),2);
	   num_patro=14;orient=1;break;
   case 139:ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),4);ta.insert(ta.end(),3);ta.insert(ta.end(),4);ta.insert(ta.end(),9);
            ta.insert(ta.end(),9);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),9);
	    num_patro=14;orient=1;break;
   case 177:ta.insert(ta.end(),11);ta.insert(ta.end(),0);ta.insert(ta.end(),6);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),6);
            ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),0);ta.insert(ta.end(),6);ta.insert(ta.end(),3);ta.insert(ta.end(),11);
	    num_patro=14;orient=1;break;
   case 216:ta.insert(ta.end(),7);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),1);ta.insert(ta.end(),7);ta.insert(ta.end(),8);
            ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),1);ta.insert(ta.end(),7);ta.insert(ta.end(),2);ta.insert(ta.end(),8);
	    num_patro=14;orient=1;break;
   case 226:ta.insert(ta.end(),2);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),5);ta.insert(ta.end(),2);ta.insert(ta.end(),10);
            ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),5);ta.insert(ta.end(),2);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
	    num_patro=14;orient=1;break;

   case 197:ta.insert(ta.end(),6);ta.insert(ta.end(),7);ta.insert(ta.end(),11);ta.insert(ta.end(),6);ta.insert(ta.end(),11);ta.insert(ta.end(),0);
            ta.insert(ta.end(),0);ta.insert(ta.end(),11);ta.insert(ta.end(),1);ta.insert(ta.end(),8);ta.insert(ta.end(),6);ta.insert(ta.end(),0);
	    num_patro=14;orient=-1;break;
   case 172:ta.insert(ta.end(),5);ta.insert(ta.end(),4);ta.insert(ta.end(),10);ta.insert(ta.end(),5);ta.insert(ta.end(),10);ta.insert(ta.end(),2);
            ta.insert(ta.end(),2);ta.insert(ta.end(),10);ta.insert(ta.end(),3);ta.insert(ta.end(),9);ta.insert(ta.end(),5);ta.insert(ta.end(),2);
	    num_patro=14;orient=-1;break;
   case 116:ta.insert(ta.end(),3);ta.insert(ta.end(),1);ta.insert(ta.end(),4);ta.insert(ta.end(),3);ta.insert(ta.end(),4);ta.insert(ta.end(),9);
            ta.insert(ta.end(),9);ta.insert(ta.end(),4);ta.insert(ta.end(),8);ta.insert(ta.end(),7);ta.insert(ta.end(),3);ta.insert(ta.end(),9);
	    num_patro=14;orient=-1;break;
   case 78:ta.insert(ta.end(),6);ta.insert(ta.end(),11);ta.insert(ta.end(),0);ta.insert(ta.end(),0);ta.insert(ta.end(),4);ta.insert(ta.end(),6);
           ta.insert(ta.end(),11);ta.insert(ta.end(),9);ta.insert(ta.end(),0);ta.insert(ta.end(),6);ta.insert(ta.end(),3);ta.insert(ta.end(),11);
	   num_patro=14;orient=-1;break;
   case 39:ta.insert(ta.end(),7);ta.insert(ta.end(),1);ta.insert(ta.end(),5);ta.insert(ta.end(),1);ta.insert(ta.end(),7);ta.insert(ta.end(),8);
           ta.insert(ta.end(),8);ta.insert(ta.end(),10);ta.insert(ta.end(),1);ta.insert(ta.end(),7);ta.insert(ta.end(),2);ta.insert(ta.end(),8);
	   num_patro=14;orient=-1;break;
   case 29:ta.insert(ta.end(),2);ta.insert(ta.end(),5);ta.insert(ta.end(),0);ta.insert(ta.end(),5);ta.insert(ta.end(),2);ta.insert(ta.end(),10);
           ta.insert(ta.end(),10);ta.insert(ta.end(),11);ta.insert(ta.end(),5);ta.insert(ta.end(),2);ta.insert(ta.end(),6);ta.insert(ta.end(),10);
	   num_patro=14;orient=-1;break;
   default:
   break;
  }
    //return ta;
}

/*
//---------------------------------------------------------------------
void ComprovacioPatrons(){
//---------------------------------------------------------------------
 int num_patro,orientacio;
 IJK offset;
 Taulafd<int> ta,tvertexs;
 cerr <<"\n#Inventor V1.0 ascii\n\n";
 for(int index_voxel=1;index_voxel<255;index_voxel++){
  offset.i=1;
  offset.j=((index_voxel%16)*2)+1;
  offset.k=2*((int)floor(index_voxel/16.))+1;
  BuscaPatro(index_voxel,&num_patro,&orientacio,ta);
  CalculaPuntsInterseccio(ta,3,offset,tvertexs);
  CreaCares(num_patro,orientacio,tvertexs);
  PuntsIntIsoVox.buida();
  taulaVertexs_iv.buida();
  taulaCares_iv.buida();
 }
 cerr << "# -- eof";
}
*/
/*
//--------------------------------------------------------------------------
void DibuixaVoxel(IJK& offset){
//--------------------------------------------------------------------------
 cerr << "ShapeHints {\n   hints (ORDERED|CONVEX)\n}";
 cerr <<"#Material {\n#   diffuseColor 0.3 1.0 0.3\n#   specularColor 1 1 1\n#   shininess 1.0\n#   transparency 0.8\n#}\n";
 cerr <<"Separator {\n   Coordinate3 {\n      point [\n";
 fprintf(stderr,"1 %d %d,\n",offset.j,offset.k);
 fprintf(stderr,"1 %d %d,\n",offset.j+1,offset.k);
 fprintf(stderr,"2 %d %d,\n",offset.j+1,offset.k);
 fprintf(stderr,"2 %d %d,\n",offset.j,offset.k);
 fprintf(stderr,"1 %d %d,\n",offset.j,offset.k+1);
 fprintf(stderr,"1 %d %d,\n",offset.j+1,offset.k+1);
 fprintf(stderr,"2 %d %d,\n",offset.j+1,offset.k+1);
 fprintf(stderr,"2 %d %d,\n",offset.j,offset.k+1);
 cerr << "    ]\n   }\n";
 cerr << "# IndexedFaceSet {\n#      coordIndex [\n#        4,5,6,7,-1, # top face\n#	0,4,7,3,-1,\n#	3,7,6,2,-1,\n#	2,6,5,1,-1,\n#	1,5,4,0,-1\n#      ]\n#   }\n Material {\n    diffuseColor 0.3 0.3 1.0\n    specularColor 1 1 1\n    shininess 1.0\n    transparency 0.5\n }\n";
 cerr <<"IndexedFaceSet {\n      coordIndex [\n        0,1,2,3,-1 # bottom face\n      ]\n   }\n";
 cerr <<" Material {\n    diffuseColor 1 1 1\n    specularColor 1 1 1\n    shininess 1.0\n }\n";
 cerr<<"# lines around the cube\n";
 cerr<<"Material { diffuseColor 1 0.1 0.1 }\n IndexedLineSet { coordIndex [0,3,-1] }\n";
 cerr <<"Material { diffuseColor 0.1 1 0.1 }\n IndexedLineSet { coordIndex [0,1,-1] }\n";
 cerr <<"Material { diffuseColor 0.1 0.1 1 }\n IndexedLineSet { coordIndex [0,4,-1] }\n";
 cerr <<" Material { diffuseColor 1 1 1 }\n";
 cerr <<"IndexedLineSet {\n     coordIndex [\n";
 cerr <<"	2,3,-1,\n	1,2,-1,\n	3,7,-1,\n	2,6,-1,\n	1,5,-1,\n	6,7,-1,\n	5,6,-1,\n	4,5,-1,\n	7,4,-1\n      ]\n }\n }\n";
}
*/







// --eof
