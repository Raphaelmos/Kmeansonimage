/*

// ESSAI d'optimisation de tout le code utilisant CLUT permettant une efficacité supérieur mais dû aux certains problèmes de pointeurs 
// (que des warnings pas d'erreurs) je pense qu'il est inutilisable 

#include "ima.h"

void kmeans(Image* image, int k, int maxIter, Color** clut) {

  kmeansInitCenters(image, &clut, k);

  for(int i = 0; i < maxIter; i++) {

    kmeansAssignPixels(image, clut, assignments, k);

    kmeansUpdateCenters(image, clut, assignments, k);

  }

  kmeansQuantize(image, clut, assignments);

}



void kmeansInitCenters(Image* image, Color** clut, int k) {

  int clutSize;

  buildCLUT(image->data, image->width, image->height, clut, &clutSize);

  sortCLUT(*clut, clutSize);

  reduceColors(image->data, image->width, image->height, *clut, clutSize, k);
}


void kmeansAssignPixels(Image* image, Color** clut, int* assignments, int k) {

  int numPixels = image->width * image->height;

  for(int i=0; i<numPixels; i++) {

    int pixelR = image->data[i*3];
    int pixelG = image->data[i*3+1]; 
    int pixelB = image->data[i*3+2];

    int minDist = INT_MAX;
    int closestCluster = 0;

    for(int j=0; j<k; j++) {
      int dist = distance(pixelR, pixelG, pixelB, centers[j].red, centers[j].green, centers[j].blue);
      if(dist < minDist) {
        minDist = dist;
        closestCluster = j;  
      }
    }

    assignments[i] = closestCluster;

  }

}

void kmeansUpdateCenters(Image* image, Color* clut, int* assignments, int k) {

  for(int i=0; i<k; i++) {
    clut[i].frequency = 0;
  }

  int numPixels = image->width * image->height;

  for(int i=0; i<numPixels; i++) {

    int cluster = assignments[i];
    clut[cluster].frequency++;

  }

  // normalize color frequencies  

  for(int i=0; i<k; i++) {

    int count = clut[i].frequency;

    if(count > 0) {
      clut[i].red = clut[i].red / count;  
      clut[i].green = clut[i].green / count;
      clut[i].blue = clut[i].blue / count;
    }

  }

}

void kmeansQuantize(Image* image, Color* clut, int* assignments) {

  int numPixels = image->width * image->height;

  for(int i=0; i<numPixels; i++) {

    int cluster = assignments[i];
    image->data[i*3] = clut[cluster].red;
    image->data[i*3+1] = clut[cluster].green; 
    image->data[i*3+2] = clut[cluster].blue;

  }

}

int getIndexInCLUT(GLubyte* pixel, Color* clut, int k) {

  int red = pixel[0];
  int green = pixel[1]; 
  int blue = pixel[2];

  int minDistance = INT_MAX;
  int closestIndex = 0;

  for(int i = 0; i < k; i++) {

    int dred = red - clut[i].red;
    int dgreen = green - clut[i].green; 
    int dblue = blue - clut[i].blue;

    int distance = dred*dred + dgreen*dgreen + dblue*dblue;

    if(distance < minDistance) {
      minDistance = distance;
      closestIndex = i;
    }

  }

  return closestIndex;

}

void compress(Image* image, int k, char* filename) {

  Color* clut;
  int* indices;
  int clutSize;
  // Build CLUT from image
  buildCLUT(image->data, image->width, image->height, &clut, &clutSize); 

  // Sort and reduce CLUT
  sortCLUT(clut, clutSize);
  reduceColors(image->data, image->width, image->height, clut, clutSize, k);

  // Allocate space for indices
  int numPixels = image->width * image->height; 
  indices = malloc(numPixels * sizeof(int));

  // Map pixels to palette indices
  for(int i=0; i<numPixels; i++) {
    indices[i] = getIndexInCLUT(image->data + i*3, clut, k);
  }

  // Open file for writing
  FILE* file = fopen(filename, "wb");

  // Write CLUT and indices to file
  fwrite(clut, sizeof(Color), k, file);
  fwrite(indices, sizeof(int), numPixels, file);

  // Clean up
  free(indices);
  fclose(file);

}

Color* readCompressedFile(char* filename, int* width, int* height) {

  FILE* file = fopen(filename, "rb");

  // Read CLUT size and dimensions
  int k, w, h;
  fread(&k, sizeof(int), 1, file);
  fread(&w, sizeof(int), 1, file);
  fread(&h, sizeof(int), 1, file);

  *width = w;
  *height = h;

  // Allocate CLUT
  Color* clut = malloc(k * sizeof(Color));

  // Read CLUT from file  
  fread(clut, sizeof(Color), k, file);

  fclose(file);

  return clut;

}

*/





