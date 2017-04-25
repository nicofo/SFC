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

    i = get_global_id(1);
    j = get_global_id(0);

    int out_rows = rows - PADDING;
    int out_cols = cols - PADDING;

    val = getValue(img, cols, i, j);
    setValue(out, out_cols, i, j, val);
}



