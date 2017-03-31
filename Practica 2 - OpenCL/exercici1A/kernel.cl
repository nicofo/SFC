#include "common.h"

// Kernel code

unsigned char getValue(__global unsigned char *img, int cols, int i, int j)
{
  float val = img[i * cols + j]; 
  return val;
}

void setValue(__global float *out, int cols, int i, int j, float value)
{
  out[i * cols + j] = value; 
}

__kernel void pattern_matching(
    __global unsigned char *img,
    __global unsigned char *pat,
    int rows,
    int cols,
    __global float *out)
{

  int i = get_global_id(1);
  int j = get_global_id(0);

  float sum = 0.0;
  float value;
  for(int k = 0; k < 16; k++){
      for(int l = 0; l < 16; l++){

	  value= (float) getValue(img,cols,j+l,i+k) - (float) getValue(pat, 16, l,k);
	  sum += value * value;
      }
  }
  setValue(out, cols,j,i,sum / (float) (16.0*16.0));
}



