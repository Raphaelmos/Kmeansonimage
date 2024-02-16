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


//#include <GL/glut.h>


#define MAX_ITERATIONS 100
#define K 16

typedef struct {
  int sizeX;
  int sizeY;
  int width;
  int height;
GLubyte *data;
} Image;

typedef struct {
GLubyte red;
GLubyte green;
GLubyte blue;
int count;
} Cluster;

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
void kmeans(Image* image, int k, int maxIter);
void kmeansInitCenters(Image* image, Cluster centers[], int k);
void kmeansAssignPixels(Image* image, Cluster centers[], int *assignments[], int k);
void kmeansUpdateCenters(Image* image, Cluster centers[], int assignments[], int k);
void kmeansQuantize(Image* image, Cluster centers[], int assignments[]);
int get_num_pixels(Image* image);

/// Utilisation de PPM ///
int ImageLoad_PPM(char *filename, Image *image);
void imagesave_PPM(char *filename, Image *image);

//// RLE ////
void sortColors(GLubyte* image, int width, int height, GLubyte* sortedColors);
void compressRLE(GLubyte* data, int size, GLubyte** compressedData, int* compressedSize);
void decompressRLE(GLubyte* compressedData, int compressedSize, GLubyte** decompressedData, int* decompressedSize);

void compressRLE2(GLubyte* data, int size, GLubyte** compressedData, int* compressedSize);
void decompressRLE2(GLubyte* compressedData, int compressedSize, GLubyte** decompressedData, int* decompressedSize);

/// Essai de dithering 

void dither(GLubyte *src, GLubyte *dst, int width, int height);

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





/* pour Mac */
// #include <GLUT/glut.h>
/*
typedef struct {
    unsigned char red, green, blue;
    int count;
} KMeanCluster;

void kmeansInitCenters(uint32_t width, uint32_t height, Image* image, KMeanCluster clusters[], int k);
void kmeansAssignPixels(Image* image, KMeanCluster clusters[], int assignments[]);
void kmeansUpdateCenters(Image* image, KMeanCluster clusters[], int assignments[]);
double distance(KMeanCluster c1, KMeanCluster c2);
*/
/*
double distance(GLubyte* p1, GLubyte* p2);

void kmeans(Image* image, int k, int maxIter);
void kmeansInitCenters(Image* image, Cluster centers[], int k);
void kmeansAssignPixels(Image* image, Cluster centers[], int assignments[], int k); 
void kmeansUpdateCenters(Image* image, Cluster centers[], int assignments[], int k);
void kmeansQuantize(Image* image, Cluster centers[], int assignments[], int k);

typedef struct {
  GLubyte red;
  GLubyte green; 
  GLubyte blue;
  int count;
} Cluster;


double distance(GLubyte* p1, GLubyte* p2);

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

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    unsigned long width;
    unsigned long height;
};


typedef struct Image Image; 
typedef unsigned short utab [3][3][3];

int ImageLoad_PPM(char *filename, Image *image);
void imagesave_PPM(char *filename, Image *image);
void upsidedown(Image *im);
void vert(Image *image);

void vert_uniform(Image *image);
void gris_uniform (Image *image);
void rouge_origin(Image *image);
void swap_green_red(Image *image);
void swap_blue_red(Image *image);



//// RLE ////
void sortColors (GLubyte* image, int width, int height, GLubyte* sortedColors);
void compressRLE(GLubyte* data, int size, GLubyte** compressedData, int* compressedSize);
void decompressRLE(GLubyte* compressedData, int compressedSize, GLubyte** decompressedData, int* decompressedSize);

void compressRLE2(GLubyte* data, int size, GLubyte** compressedData, int* compressedSize);
void decompressRLE2(GLubyte* compressedData, int compressedSize, GLubyte** decompressedData, int* decompressedSize);

//// C-LUT ////
void buildCLUT (GLubyte* image, int width, int height, Color** clut, int* clutSize);
void sortCLUT (Color* clut, int clutSize);
void reduceColors (GLubyte* image, int width, int height, Color* clut, int clutSize, int k);



typedef struct {
  unsigned long width;
  unsigned long height; 
  GLubyte* pixels;
} Image;
*/
