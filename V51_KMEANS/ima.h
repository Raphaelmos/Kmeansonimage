#ifndef IMA_H
#define IMA_H
/* pour Bocal */
#include <GL/gl.h> 
//#include <gl/glut.h>
//#include <GL/glut.h>
#include "GL/glut.h"
#include "GL/freeglut.h"  
#include <stdint.h>
#include <assert.h>
#include <stdio.h> 
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#define HISTOGRAM_SIZE 256

//#include <GL/glut.h>


#define MAX_ITERATIONS 100
#define K 16


typedef struct {
GLubyte red;
GLubyte green;
GLubyte blue;
int count;
} Cluster;

typedef struct {
  int sizeX;
  int sizeY;
  int width;
  int height;
Cluster *clusters;
GLubyte *data;
} Image;

typedef struct {
GLubyte red;
GLubyte green;
GLubyte blue;
} Pixel;

typedef struct {
GLubyte red;
GLubyte green;
GLubyte blue;
int frequency;
} Color;

double distance(GLubyte* p1, GLubyte* p2);

extern GLubyte *CLUT;
extern Cluster *centers;  
extern int *assignments;


//// KMEANS /////
void kmeans(Image* image, int k, int maxIter, Cluster** centers, int** assignments);
void kmeansInitCenters(Image* image, Cluster centers[], int k);
void kmeansAssignPixels(Image* image, Cluster centers[], int *assignments[], int k);
void kmeansUpdateCenters(Image* image, Cluster centers[], int assignments[], int k);
void kmeansQuantize(Image* image, Cluster centers[], int assignments[]);
int get_num_pixels(Image* image);
extern int histogram[HISTOGRAM_SIZE][HISTOGRAM_SIZE][HISTOGRAM_SIZE];

/// Utilisation de PPM ///
int ImageLoad_PPM(char *filename, Image *image);
void imagesave_PPM(char *filename, Image *image);

// COMPRESSION DECOMPRESSION KMEANS ///
void compressImage(Image* image, int k, char* filename);
Image* decompressImage(char* filename);


//// RLE ////
void sortColors(GLubyte* image, int width, int height, GLubyte* sortedColors);
void compressRLE(GLubyte* data, int size, GLubyte** compressedData, int* compressedSize);
void decompressRLE(GLubyte* compressedData, int compressedSize, GLubyte** decompressedData, int* decompressedSize);

void compressRLE2(GLubyte* data, int size, GLubyte** compressedData, int* compressedSize);
void decompressRLE2(GLubyte* compressedData, int compressedSize, GLubyte** decompressedData, int* decompressedSize);

/// Essai de dithering 

void dither(GLubyte* src, GLubyte* dst, int width, int height);
void applyDither(Image* image);

//// C-LUT ////
void buildCLUT(GLubyte* image, int width, int height, Color** clut, int* clutSize);
void sortCLUT(Color* clut, int clutSize);
void reduceColors(GLubyte* image, int width, int height, Color* clut, int clutSize, int k);

void upsidedown(Image *im);
void vert(Image *image);

void vert_uniform(Image *image);
void gris_uniform (Image *image);
void rouge_origin(Image *image);
void swap_green_red(Image *image);
void swap_blue_red(Image *image);



#endif

#ifndef INT_MAX
   #define INT_MAX 2147483647  
#endif

/* 

Fonctions qui iraient avec ce qui est commentaire dans kmeans.c et main.c

void compressWithKmeans(Image* image, int k, int numPixels, char* filename);
//void readCompressedImage(int k, int numPixels, char* filename, Image** image);
Cluster* readCompressedImage(int k, int numPixels, char* filename);
Cluster* readFile(int k, int* width, int* height);
// Cluster* readCompressedImage(int k, int numPixels, char* filename);
void freeImage(Image* image);
//void readCompressedImage(char* filename, Image** image);


*/


