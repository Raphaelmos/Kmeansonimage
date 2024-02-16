/* Il s agit de prendre une image et de la modifier */

#include "ima.h"
#include <limits.h>


void vert(Image *im){
  GLubyte *crt,val1,val2;
  int i, nb_pixels;
  nb_pixels = im->sizeX * im->sizeY;
  val1=0;
  val2=255;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    *crt++=val1;
    *crt++=val2;
    *crt++=val1;
  }
}

void mystere (Image *im){
  GLubyte *crt,val1,val2;
  int i, nb_pixels;
  nb_pixels = im->sizeX * im->sizeY;
  val1=255;
  val2=0;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    *crt++=val1;
    *crt++=val2;
    *crt++=val1;
  }
}


void vert_moy(Image *im) {
  GLubyte *crt;
  int i, nb_pixels;
  nb_pixels = im->sizeX * im->sizeY;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    *crt++=(*crt+*(crt+1))/2;
    *crt++=(*crt+*(crt+1))/2;
    *crt++=(*crt+*(crt+1))/2;
  }
}


void vert_uniform (Image *im) {
  GLubyte *crt;
  int i, nb_pixels;
  nb_pixels = im->sizeX * im->sizeY;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    *crt++=0;
    *crt++=255;
    *crt++=0;
  }
}


void rouge_origin (Image *im) {
  GLubyte *crt;
  int i, nb_pixels;
  nb_pixels = im->sizeX * im->sizeY;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    *crt++=255;
    *crt++=0;
    *crt++=0;
  }
}


void gris_uniform (Image *im) {
  GLubyte *crt;
  int i, nb_pixels;
  nb_pixels = im->sizeX * im->sizeY;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    *crt++=255;
    *crt++=255;
    *crt++=255;
  }
}


void swap_green_red (Image *im) {
  GLubyte *crt;
  int i, nb_pixels;
  nb_pixels = im->sizeX * im->sizeY;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    GLubyte tmp;
    tmp=*crt;
    *crt=*(crt+1);
    *(crt+1)=tmp;
    crt+=3;
  }
}


void swap_blue_red (Image *im) {
  GLubyte *crt;
  int i, nb_pixels;
  nb_pixels = im->sizeX * im->sizeY;
  for (crt=im->data,i=0;i<nb_pixels;i++){
    GLubyte tmp;
    tmp=*crt;
    *crt=*(crt+2);
    *(crt+2)=tmp;
    crt+=3;
  }
}

