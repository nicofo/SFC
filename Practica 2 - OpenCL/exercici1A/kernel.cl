#include "common.h"

//MIDAP = 16
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
    float val;  

    i = get_global_id(1);
    j = get_global_id(0);

    int out_rows = rows - PADDING;
    int out_cols = cols - PADDING;

    
    float sum = 0.0;
    for(int k = 0; k < MIDAP; k++){
	for(int l = 0; l < MIDAP; l++){

	    //float value =  getValue(img,cols,i+l,j+k) -  getValue(pat,MIDAP,l,k); //anterior inicial
	    float value =  getValue(img,cols,i+k,j+l) -  getValue(pat,MIDAP,k,l); //Prueba cambiando l i k
	    sum += value * value;
	}
    }
    val =sum / (float) (MIDAP*MIDAP);
    setValue(out,out_cols, i, j, val);
    
    
    /*
    val = getValue(img, cols, i, j);
    setValue(out, out_cols, i, j, val);
    */
}

/* NOTA.- Para este apartado se ha puesto el localWorkSize a 1,1 */

