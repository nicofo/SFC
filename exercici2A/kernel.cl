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
    __global float *out)        
{
    int i, j;
    char val;  

    i = get_group_id(1)*32 + get_local_id(1)*21;
    j = get_group_id(0)*32 + get_local_id(0);

    int out_rows = rows - PADDING;
    int out_cols = cols - PADDING;

    
    
    for (int m=0; m<8; m++){
      float sum = 0.0;
      for(int k = 0; k < MIDAP; k++){
	  for(int l = 0; l < MIDAP; l++){

	      float value =  getValue(img,cols,i+k+m,j+l) -  getValue(pat,MIDAP,k,l); 
	      sum += value * value;
	  }
      }
      val =sum / (float) (MIDAP*MIDAP);
      setValue(out,out_cols, i+m, j, val);
    }
}



