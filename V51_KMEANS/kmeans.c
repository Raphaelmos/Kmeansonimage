#include "ima.h"


void kmeans(Image* image, int k, int maxIter, Cluster** centers, int** assignments) {
  // Multiple check pour essayer de détecter les problèmes survenus
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

   *centers = malloc(k * sizeof(Cluster));
    if(!centers) {
    printf("Cluster allocation failed\n");
    return;}
       
   *assignments = malloc(image->sizeX*image->sizeY * sizeof(int));

    if(!centers || !assignments) {
    free(centers);
    free(assignments);
    return;
  }

  kmeansInitCenters(image, *centers, k);
  for(int i=0; i<maxIter; i++) {
    printf("Loading...%d / %d\n", i+1, maxIter);
    kmeansAssignPixels(image, *centers, assignments, k);
    kmeansUpdateCenters(image, *centers, *assignments, k);

  }
  kmeansQuantize(image, *centers, *assignments);

  free(centers);
  free(assignments); 

}

// Fonctions de kmeans : Init -> Assign -> Update -> Quantize et distance -> pour opti compress -> read compress

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

  printf("k = %d\n", k);

  int histogram[HISTOGRAM_SIZE][HISTOGRAM_SIZE][HISTOGRAM_SIZE];

  // Initialize histogram
  for(int i=0; i<HISTOGRAM_SIZE; i++) {
    for(int j=0; j<HISTOGRAM_SIZE; j++) {
      for(int k=0; k<HISTOGRAM_SIZE; k++) {
        histogram[i][j][k] = 0;
      }
    }
  }

  // Count pixels in histogram
  for(int i=0; i<image->width; i++) {
    for(int j=0; j<image->height; j++) {
      int red = image->data[i*3];
      int green = image->data[i*3+1];
      int blue = image->data[i*3+2];
      histogram[red][green][blue]++;
    }
  }

  // Initialize centers from histogram
  for(int i=0; i<k; i++) {
    int maxCount = 0;
    int maxRed = 0; 
    int maxGreen = 0;
    int maxBlue = 0;
    
    for(int red=0; red<HISTOGRAM_SIZE; red++) {
      for(int green=0; green<HISTOGRAM_SIZE; green++) {
        for(int blue=0; blue<HISTOGRAM_SIZE; blue++) {
          if(histogram[red][green][blue] > maxCount) {
            maxCount = histogram[red][green][blue];
            maxRed = red;
            maxGreen = green;
            maxBlue = blue;
          }
        }  
      }
    }

    centers[i].red = maxRed;
    centers[i].green = maxGreen;
    centers[i].blue = maxBlue;
  }

/*
 Précédent code utilisant random
  for(int i=0; i<1; i++) {

    int pixelIndex = rand() % (image->sizeX * image->sizeY);

    

    centers[i].red = image->data[pixelIndex*3]; 
    centers[i].green = image->data[pixelIndex*3 + 1];
    centers[i].blue = image->data[pixelIndex*3 + 2];

    centers[i].count = 0;

  }
*/
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

void kmeansUpdateCenters(Image* image, Cluster centers[], int assignments[], int k) {

    //centers[i].color = ColorToInt(red,green,blue)                      

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

if(centers[clusterIndex].count != 0) {
  printf("Cluster count not 0\n");
  return;  
}

unsigned char prevRed = centers[clusterIndex].red;

centers[clusterIndex].red += image->data[i*3];

if(centers[clusterIndex].red < prevRed) {
  // check si overflow  
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

centers[clusterIndex].blue += image->data[i*3+2]; 

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
// Pas d'utilisation de k ()
void kmeansQuantize(Image* image, Cluster centers[], int assignments[]) {
  for(int i=0; i<image->sizeX*image->sizeY; i++) {
    // Retirer les prints si ça pose problème 
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
  // Essai du 16/12 : Avec ce code ajouté il devrait bien compresser et afficher l'image compresser, la sauvegarder et le decompress permettant de le lire comme demandé.
  // 

void compressImage(Image* image, int k, char* filename) {
  int numPixels = image->width * image->height;

  Cluster* centers;
  centers = malloc(k * sizeof(Cluster));
  int* assignments;  
  assignments = malloc(numPixels * sizeof(int));

  // Exécuter kmeans
  kmeans(image, k, 100, &centers, &assignments);

  // Copier les données dans des tableaux temporaires
  Cluster* centers_copy = malloc(k * sizeof(Cluster));
  int* assignments_copy = malloc(numPixels * sizeof(int));

  // Copier le contenu
  memcpy(centers_copy, centers, k * sizeof(Cluster)); 
  memcpy(assignments_copy, assignments, numPixels * sizeof(int));

  // Ouvrir fichier en écriture 
  FILE* f = fopen(filename, "wb");

  // Écrire les données
  fwrite(centers_copy, sizeof(Cluster), k, f);
  fwrite(assignments_copy, sizeof(int), numPixels, f);

  // Libérer la mémoire
  free(centers_copy);
  free(assignments_copy);
  free(centers);
  free(assignments);

  // Fermer fichier
  fclose(f);
}

Image* decompressImage(char* filename) {

  // Ouvrir fichier en lecture
  FILE* f = fopen(filename, "rb");

  // Lire métadonnées
  int k, width, height; 
  fread(&k, sizeof(int), 1, f);
  fread(&width, sizeof(int), 1, f); 
  fread(&height, sizeof(int), 1, f);

  // Allouer image
  Image* image = malloc(sizeof(Image));
  image->width = width;
  image->height = height;

  // Allouer tableaux 
  Cluster* centers = malloc(k*sizeof(Cluster));
  int* assignments = malloc(width*height*sizeof(int));

  // Lire données
  fread(centers, sizeof(Cluster), k, f);
  fread(assignments, sizeof(int), width*height, f);

  // Fermer fichier
  fclose(f);
  free(centers);
  free(assignments);  

  // Retourner image
  free(image->data); 
  free(image);

  return NULL;
}


/* 
// D'après valgrind 

    Lectures/écritures non valides : Aucune 
    Fuites de mémoire : Aucune
    Erreurs d’allocation/libération de mémoire : Aucune


// Version fonctionnel donc 70% de chance que le problème viennent de : 

    //Dans compressImage(), centers et assignments sont passé vers kmeans(), mais leur mémoire est pas bien allouer ?:


// ci-dessous le code dans main.c aidant à faire un comparatif du code ci-dessus avec ces deux nouvelles fonctions
case 13:
 {
   // Allocation CLUT et arrays 
  CLUT = malloc(256 * sizeof(GLubyte) * 3);  
  int k = 16;
  int numPixels = image->width * image->height;
  Cluster* centers = malloc(k * sizeof(Cluster));
  int* assignments = malloc(numPixels * sizeof(int));
  // Performe K-Means clustering 
  kmeans(image, k, 100);  
  // Quantize image avec CLUT
  for(int i=0; i<numPixels; i++) {
    int index = assignments[i];
    image->data[i*3] = CLUT[index*3];
    image->data[i*3+1] = CLUT[index*3+1];
    image->data[i*3+2] = CLUT[index*3+2]; 
  }
  free(assignments);
  free(centers);
  free(CLUT);
  Display();
 default:
  break;
 } 
*/









// ----------------------------------------------------------------

// Multiple essais d'arriver à une compression propre avec une lecture de fichier comme suggéré dans les mails mais j'arrive toujours à une Segmentation Fault que je n'ai malheureusment pas réussi à régler
// 
// ----------------------------------------------------------------

/*
void compressWithKmeans(Image* image, int k, int numPixels, char* filename) {

  centers = malloc(k * sizeof(Cluster));
  assignments = malloc(numPixels * sizeof(int));
  
  kmeans(image, k, 100); 
  FILE* f = fopen(filename, "wb");

  fclose(f);

  free(centers);
  free(assignments);
}

Cluster* readCompressedImage(int k, int numPixels, char* filename) {

  FILE* f = fopen(filename, "rb");

  centers = malloc(k * sizeof(Cluster));
  assignments = malloc(numPixels * sizeof(int));

  fread(centers, sizeof(Cluster), k, f);
  fread(assignments, sizeof(int), numPixels, f);

  fclose(f);
  
  return centers; 
}
*/
/*
// Compress image with kmeans
void compressWithKmeans(Image* image, int k, int numPixels, char* filename) {

  // Validate inputs
  if(!image || !image->data || k < 1 || numPixels < 1 || !filename) {
    return; 
  }

  // Run kmeans  
  kmeans(image, k, 100);

  // Deep copy data  
  Cluster* centers_copy = malloc(k * sizeof(Cluster));
  int* assignments_copy = malloc(numPixels * sizeof(int));

  if(!centers_copy || !assignments_copy) {
    free(centers_copy);
    free(assignments_copy);
    return;
  }

  memcpy(centers_copy, centers, k * sizeof(Cluster));
  memcpy(assignments_copy, assignments, numPixels * sizeof(int));

  // Open file  
  FILE* f = fopen(filename, "wb");
  if(!f) {
    free(centers_copy);
    free(assignments_copy);
    return;
  }

  // Write data
  if(fwrite(centers_copy, sizeof(Cluster), k, f) != k) {
    // error handling  
    free(centers_copy);
    free(assignments_copy);
    fclose(f);
    return;
  }

  if(fwrite(assignments_copy, sizeof(int), numPixels, f) != numPixels) {
   // error handling
   free(centers_copy);
   free(assignments_copy); 
   fclose(f);
   return;
  }

  fclose(f);

  // Cleanup
  free(centers_copy);
  free(assignments_copy);

  // Original data no longer needed
  free(centers);
  free(assignments);

}

void freeImage(Image* image) {
  free(image->data);
  free(image);
}

// Modify return type to Image*
Cluster* readCompressedImage(int k, char* filename) {

  int width, height;  

  Cluster* clusters = readFile(k, &width,&height);
  
  Image* image = malloc(sizeof(Image));
  image->width = width; 
  image->height = height;
  image->clusters = clusters; */

 // return clusters;

//}


/* TENTAIVE AVEC L'AUTRE TENTATIVE de main.c ********************************
Cluster* readCompressedImage(int k, char* filename) {

  int width, height;  

  Cluster* clusters = readFile(k, &width, &height);

  Image* image = malloc(sizeof(Image));

  image->width = width;
  image->height = height;
  image->clusters = clusters;

  return image;
}


*/



/*
// Read compressed image
Cluster* readCompressedImage(int k, int numPixels, char* filename) {

  FILE* f = fopen(filename, "rb");
  if(!f) return NULL;

  Cluster* centers = malloc(k * sizeof(Cluster));
  int* assignments = malloc(numPixels * sizeof(int));

  if(!centers || !assignments) {
    if(centers) free(centers);
    if(assignments) free(assignments);
    fclose(f);
    return NULL;
 }

  if(fread(centers, sizeof(Cluster), k, f) != k) {
    free(centers);
    free(assignments);
    fclose(f);
    return NULL; 
  }
  // similarly check read of assignments
  fclose(f);
  return centers;
} */

// MEMCPY 

/*void compressWithKmeans(Image* image, int k, int numPixels, char* filename) {

  kmeans(image, k, 100);


  // Deep copy data  
  Cluster* centers_copy = malloc(k * sizeof(Cluster));
  int* assignments_copy = malloc(numPixels * sizeof(int));

  memcpy(centers_copy, (void*)centers, k * sizeof(Cluster));
  memcpy(assignments_copy, (void*)assignments, numPixels * sizeof(int));

  FILE* f = fopen(filename, "wb");

  // Save deep copies  
  fwrite(centers_copy, sizeof(Cluster), k, f);
  fwrite(assignments_copy, sizeof(int), numPixels, f);

  fclose(f);
  
  free(centers_copy);
  free(assignments_copy);
  // original data no longer needed
  if(centers) {
  free(centers);
  centers = NULL;
}

  free(centers);
  centers = NULL;

  free(assignments);
  assignments = NULL;

} */

/* Cluster* readCompressedImage(int k, int numPixels, char* filename) {

  FILE* f = fopen(filename, "rb");

  if(!f) {
    perror("Failed to open file");
    return NULL;
  }

  // Allocate memory
  Cluster* centers = malloc(k * sizeof(Cluster));  
  int* assignments = malloc(numPixels * sizeof(int));

  if(!centers || !assignments) {
    free(centers);
    free(assignments);
    fclose(f);
    return NULL; 
  }

  // Read data
  if(fread(centers, sizeof(Cluster), k, f) != k) {
    free(centers);
    free(assignments);
    fclose(f);
    return NULL;
  }

  if(fread(assignments, sizeof(int), numPixels, f) != numPixels) {
    free(centers); 
    free(assignments);
    fclose(f);
    return NULL;
  }

  fclose(f);

  return centers;
} */



/*
void readCompressedImage(int k, int numPixels, char* filename, Image** image) {

  FILE* f = fopen(filename, "rb");

  // Alloc and load deep copies  
  centers = malloc(k * sizeof(Cluster));
  assignments = malloc(numPixels * sizeof(int));

  fread((void*)centers, sizeof(Cluster), k, f);
  fread((void*)assignments, sizeof(int), numPixels, f);  

  fclose(f);

}


*/
// void readCompressedImage(int k, int numPixels, char* filename, Image** image);
/*
void compressWithKmeans(Image* image, int k, char* filename) {

  kmeans(image, k, 100);

  CLUT = malloc(k * 3 * sizeof(GLubyte));

  for(int i = 0; i < k; i++) {
    CLUT[i*3] = centers[i].red;
    CLUT[i*3+1] = centers[i].green;
    CLUT[i*3+2] = centers[i].blue; 
  }

  FILE* f = fopen(filename, "wb");

  fwrite(&k, sizeof(int), 1, f);
  fwrite(&image->width, sizeof(int), 1, f);
  fwrite(&image->height, sizeof(int), 1, f);

  fwrite(CLUT, sizeof(GLubyte), k*3, f);

  fwrite(assignments, sizeof(int), image->width*image->height, f);

  fclose(f);

//  free(centers);
 // free(assignments);
 // free(CLUT);

}

void readCompressedImage(char* filename, Image** image) {

  FILE* f = fopen(filename, "rb");

  int k;
  fread(&k, sizeof(int), 1, f);

  int width, height;
  fread(&width, sizeof(int), 1, f);
  fread(&height, sizeof(int), 1, f);

  *image = malloc(sizeof(Image));
  (*image)->width = width; 
  (*image)->height = height;

  assignments = malloc(width * height * sizeof(int));
  
  CLUT = malloc(k*3*sizeof(GLubyte));

  fread(CLUT, sizeof(GLubyte), k*3, f);

  fread(assignments, sizeof(int), width*height, f);

  fclose(f);

}
*/

/*

// Juste du fwrite fread 
void compressWithKmeans(Image* image, int k, char* filename) {

  kmeans(image, k, 100);

  FILE* f = fopen(filename, "wb");

  // Sauvegarde à partir de l'original
  fwrite(centers, sizeof(Cluster), k, f);  
  fwrite(assignments, sizeof(int), numPixels, f);

  fclose(f);

}

void readCompressedImage(char* filename) {

  FILE* f = fopen(filename, "rb");

  // Load à partir de l'original
  fread(centers, sizeof(Cluster), k, f);
  fread(assignments, sizeof(int), numPixels, f);  

  fclose(f);

}

*/
