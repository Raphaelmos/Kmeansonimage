#include "ima.h"
// Essai d'un dithering pour avoir un comparatif avec le k-means clustering et RLE

/*
Le dithering propage l'erreur de quantification sur les pixels voisins en utilisant une matrice de diffusion d'erreur. 
Cela permet de réduire les effets de crénelage en quantifiant l'image sur moins de couleurs.
*/

void applyDither(Image* image) {

  GLubyte* ditheredData = malloc(image->sizeX * image->sizeY * 3);

  dither(image->data, ditheredData, image->sizeX, image->sizeY);  

  free(image->data);


  image->data = ditheredData;

}

void dither(GLubyte* src, GLubyte* dst, int width, int height) {

  GLubyte dither_matrix[4][4] =  
     {{0,8,2,10},  
      {12,4,14,6}, 
      {3,11,1,9},
      {15,7,13,5}};

  for(int y=0; y<height; y++) {
    for(int x=0; x<width; x++) {
 
      int dy = y % 4;    
      int dx = x % 4;

     
      int dither_val = dither_matrix[dy][dx];    

       
      int src_idx = (y*width + x) * 3; 
      int dst_idx = src_idx;


      if(dither_val < src[src_idx]) {
         dst[dst_idx]--;  
      }

      dst_idx++;

      if(dither_val < src[src_idx+1]) {
         dst[dst_idx]--;   
      }

      dst_idx++; 

       if(dither_val < src[src_idx+2]) {
          dst[dst_idx]--;
       }

    }
  }
}