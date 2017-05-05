#include "common.h"

float getValue(__global unsigned char *img, int cols, int i, int j)
{
  // i -> fila
  // j -> columna
  float val = img[i * cols + j]; 
  return val;
}

void setValue(__global float *out, int cols, int i, int j, float value)
{
  // i -> fila
  // j -> columna
  out[i * cols + j] = value; 
}

// img: imatge amb una vora de 15 pixels per la dreta i per sota
// pat: patro de 16 x 16 pixels
// rows: files de img (incloent la vora de 15 pixels)
// cols: columnes de img (incloent la vora de 15 pixels)
// out: image resultant sense cap vora

__kernel void pattern_matching(
    __global unsigned char *img,      
    __global unsigned char *pat,
    int rows, 
    int cols, 
    __global float *out) //Espai de memoria local       
{
    int i, j;
    char val;  
    
    __local float matriu[64][48];
    
    //Sistema con foto lena
    i = get_group_id(1)*32 + get_local_id(1)*4;
    j = get_group_id(0)*32 + get_local_id(0);
   
    int out_rows = rows - PADDING;
    int out_cols = cols - PADDING;

   //Copia de Dades
   for(int n=get_local_id(0) ; n < 64 ; n += get_local_size(0))
     for(int s=get_local_id(1); s < 48; s += get_local_size(1))
	matriu[n][s]= getValue(img,cols,n,s);
   
   //Barrera
   barrier(CLK_LOCAL_MEM_FENCE);
    
   //Procesament
    for (int m=0; m < 4; m++){
      float sum = 0.0;
      for(int k = 0; k < MIDAP; k++){
	  for(int l = 0; l < MIDAP; l++){

	      float value =  matriu[k+m][l] -  getValue(pat,MIDAP,k,l); 
	      sum += value * value;
	  }
      }
      val =sum / (float) (MIDAP*MIDAP);
      setValue(out,out_cols, i+m, j, val);
    }
}



