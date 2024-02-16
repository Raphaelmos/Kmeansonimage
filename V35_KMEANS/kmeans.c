#include "ima.h"


void kmeans(Image* image, int k, int maxIter) {

    // Check image pointer
  if(!image) {
    printf("Invalid image pointer\n");
    return; 
  }

  // Check image data
  if(!image->data) {
    printf("Invalid image data\n");
    return;
  }


  printf("k value received: %d\n", k);
  if(!image || !image->data) {
    fprintf(stderr, "Image is NULL or image data is invalid\n");
    return;
  }

  int numPixels = get_num_pixels(image);

  if(k <= 0 || k > numPixels) {
     fprintf(stderr, "Invalid k value: k must be > 0 and <= numPixels\n");
     return;
  }

  Cluster* centers = malloc(k * sizeof(Cluster));
    if(!centers) {
    printf("Cluster allocation failed\n");
    return;}
       
  int* assignments = malloc(image->sizeX*image->sizeY * sizeof(int));

    if(!centers || !assignments) {
    free(centers);
    free(assignments);
    return;
  }

  kmeansInitCenters(image, centers, k);
  for(int i=0; i<maxIter; i++) {
    printf("Loading...%d / %d\n", i+1, maxIter);
    kmeansAssignPixels(image, centers, &assignments, k);
    kmeansUpdateCenters(image, centers, assignments, k);

  }
  kmeansQuantize(image, centers, assignments);
  free(centers);
  centers = NULL;
  free(assignments);
  assignments = NULL;

}

// Fonctions de kmeans : Init -> Assign -> Update -> Quantize et distance

void kmeansInitCenters(Image* image, Cluster centers[], int k) {


    
      if(!image) {
    printf("Invalid image pointer\n");  
    return;
    }

      if(!centers) {
    printf("Invalid centers pointer\n");
    return;
      }

    if (centers == NULL) {
    centers = malloc(k * sizeof(Cluster));
    if (centers == NULL) {
      fprintf(stderr, "Failed to allocate cluster array\n");  
      return;
    }

  if (image == NULL) {
    fprintf(stderr, "Image pointer is NULL\n");
    return;
  }
  srand(time(NULL));

  printf("k = %d\n", k);

  for(int i=0; i<1; i++) {

    int pixelIndex = rand() % (image->sizeX * image->sizeY);

    

    centers[i].red = image->data[pixelIndex*3]; 
    centers[i].green = image->data[pixelIndex*3 + 1];
    centers[i].blue = image->data[pixelIndex*3 + 2];

    centers[i].count = 0;

  }

}

}

void kmeansAssignPixels(Image* image, Cluster centers[], int *assignments[], int k) {
  for(int i=0; i<image->sizeX*image->sizeY; i++) {

      if(i >= (image->sizeX*image->sizeY)) {
      printf("Invalid assignments index!");
      return;
    }

    int closest = 0;
    double minDist = distance(image->data+i*3, &centers[0].red);

    for(int j=1; j<k; j++) {

      double dist = distance(image->data+i*3, &centers[j].red);

      if(dist < minDist) {
        closest = j;
        minDist = dist;
      }

    }

    (*assignments)[i] = closest;

  }
}

void kmeansUpdateCenters(Image* image, Cluster centers[], 
                         int assignments[], int k) {

   if(k <= 0) {
    printf("Invalid k not good");
    return; 
  }                        

  if(!image || !centers || !assignments) {
    printf("Null pointer error\n");
    return;
  }

  if(k > (int)image->sizeX * image->sizeY) {
    printf("Invalid k value\n");
    return;
  }

  for(int i=0; i<k; i++) {
    centers[i].count = 0u; 
  }

  for(int i=0; i<image->sizeX*image->sizeY; i++) {

    int clusterIndex = assignments[i];

    if(clusterIndex >= k) {
      printf("Invalid cluster index: %u\n", clusterIndex);
      return;
    }

    centers[clusterIndex].count++;

if(centers[clusterIndex].count) {
  printf("Cluster count not 0\n");
  return;
}

unsigned char prevRed = centers[clusterIndex].red;

centers[clusterIndex].red += image->data[i*3];

if(centers[clusterIndex].red < prevRed) {
  // overflow occurred  
  printf("Overflow updating red\n");
  return;
}

unsigned char prevGreen = centers[clusterIndex].green;

centers[clusterIndex].green += image->data[i*3+1]; 

if(centers[clusterIndex].green < prevGreen) {
  printf("Overflow updating green\n");
  return;
}

unsigned char prevBlue = centers[clusterIndex].blue;

centers[clusterIndex].blue += image->data[i*3+1]; 

if(centers[clusterIndex].blue < prevBlue) {
  printf("Overflow updating blue\n");
  return;
}

  }
  
  for(int i=0; i<k; i++) {

    int count = (unsigned int)centers[i].count;

    if(count == 0) continue;

    centers[i].red /= count;  
    centers[i].green /= count;
    centers[i].blue /= count;

  }
  
}
// Pas d'utilisation de k (je peux enlever mais pas d'utilisation)
void kmeansQuantize(Image* image, Cluster centers[], int assignments[]) {
  for(int i=0; i<image->sizeX*image->sizeY; i++) {

    printf("%d\n", i);
    printf("%d\n", assignments[i]);
    printf("%d %d %d\n", centers[assignments[i]].red, centers[assignments[i]].green, centers[assignments[i]].blue);
    // get nearest cluster index
    int clusterIndex = assignments[i]; 

    // quantize pixel to cluster color
    image->data[i*3] = centers[clusterIndex].red;
    image->data[i*3+1] = centers[clusterIndex].green;
    image->data[i*3+2] = centers[clusterIndex].blue;

  }

}

double distance(GLubyte* p1, GLubyte* p2) {

  int rDiff = p1[0] - p2[0];
  int gDiff = p1[1] - p2[1];
  int bDiff = p1[2] - p2[2];

  return sqrt(rDiff*rDiff + gDiff*gDiff + bDiff*bDiff);

}
