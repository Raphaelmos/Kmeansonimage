#include "ima.h"
// Essaie d'un dithering pour avoir un comparatif avec le k-means clustering et RLE
// ( Non fonctionnel )
#define NGRAYS 256 

void dither(GLubyte *src, GLubyte *dst, int width, int height) {

  GLubyte error[4];
  
  for(int y=0; y<height; y++) {
    for(int x=0; x<width; x++) {

      int pixel = src[y*width+x];
      int quantized = pixel/(NGRAYS/256)*(NGRAYS/256);

      dst[y*width+x] = quantized;

      error[0] = pixel - quantized;  
      error[1] = error[0] * 7/16; 
      error[2] = error[0] * 3/16;
      error[3] = error[0] * 5/16;

      if(x < width-1)
        dst[y*width+x+1] += error[1];  

      if(y < height-1) {
        if(x > 0)  
          dst[(y+1)*width+x-1] += error[2];
        dst[(y+1)*width+x] += error[3]; 
      }
    }
  }
}
