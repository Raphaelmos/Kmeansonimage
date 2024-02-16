#include "ima.h"

Image *image;
Cluster *centers;
int *assignments;
#define ESCAPE 27
GLubyte *CLUT;

int get_num_pixels(Image* image) {
  return image->sizeX * image->sizeY;
}

GLubyte* compressedData = NULL;
int compressedSize = 0;

void Keyboard(unsigned char key, int x __attribute__((unused)), int y __attribute__((unused))) {
  switch(key) {
    case ESCAPE:
      exit(0);
      break;
    default:
      fprintf(stderr, "Unused key\n");
  }
}

void Mouse(int button, int state __attribute__((unused)), int x __attribute__((unused)), int y __attribute__((unused))) {
  switch(button) {
    case GLUT_LEFT_BUTTON:
    case GLUT_MIDDLE_BUTTON:
    case GLUT_RIGHT_BUTTON:
      break;
  }
  glutPostRedisplay();
}

int Init(char *s) {
  image = (Image *) malloc(sizeof(Image));
  if (image == NULL) {
    fprintf(stderr, "Out of memory\n");
    return -1;
  }
  if (ImageLoad_PPM(s, image) == -1)
    return -1;
  printf("sizes %d %d\n", image->sizeX, image->sizeY);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glutReshapeWindow(image->sizeX, image->sizeY);

  return 0;
}

int ReInit() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glutReshapeWindow(image->sizeX, image->sizeY);

  return 0;
}

void Display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawPixels(image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image->data);
  glFlush();
}

void Reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void menuFunc(int item) {
  char s[256];
  switch(item){
  case 0:
    free(image);
    exit(0);
    break;
  case 1:
    printf("Vert \n");
    vert(image);
    Display();
    break;
  case 2:
    printf("Vert uniforme \n");
    vert_uniform(image);
    Display();
    break;
  case 3:
    printf("Gris uniforme \n");
    gris_uniform(image);
    Display();
    break;
  case 4:
    printf("Rouge origin \n");
    rouge_origin(image);
    Display();
    break;
  case 5:
    printf("Swap red and green \n");
    swap_green_red(image);
    Display();
    break;
  case 6:
    printf("Swap blue and red \n");
    swap_blue_red(image);
    Display();
    break;
  case 7:
    printf("Entrer le nom pour l'image dans cette taille\n");
    scanf("%s", &s[0]);
    imagesave_PPM(s, image);
    break;
  case 8:
    printf("Taille de l image : %d %d\n", (int) image->sizeX, (int) image->sizeY);
    break;
  case 9:
    {
      GLubyte* sortedColors = (GLubyte*)malloc(image->sizeX * image->sizeY * 3 * sizeof(GLubyte));
      if (sortedColors == NULL) {
        fprintf(stderr, "Out of memory for sortedColors\n");
        break;
      }

      sortColors(image->data, image->sizeX, image->sizeY, sortedColors);

      memcpy(image->data, sortedColors, image->sizeX * image->sizeY * 3);

      free(sortedColors);
      Display();
      break;
    }
  case 10:
    {
      if (compressedData != NULL) {
        free(compressedData);
        compressedData = NULL;
      }

      compressRLE2(image->data, image->sizeX * image->sizeY * 3, &compressedData, &compressedSize);
      printf("Original size: %d, Compressed size: %d, Compression ratio: %.2f\n", image->sizeX * image->sizeY * 3, compressedSize, (float)compressedSize / (image->sizeX * image->sizeY * 3));

      FILE *file = fopen("compressed_image.rle", "wb");
      if (file != NULL) {
        fwrite(compressedData, sizeof(GLubyte), compressedSize, file);
        fclose(file);
        printf("Compressed image saved to 'compressed_image.rle'\n");
      } else {
        fprintf(stderr, "Unable to open file 'compressed_image.rle'\n");
      }

      break;
    }
  case 11:
    {
      if (compressedData == NULL) {
        printf("No compressed data available.\n");
        break;
      }

      GLubyte* decompressedData = NULL;
      int decompressedSize = 0;
      decompressRLE2(compressedData, compressedSize, &decompressedData, &decompressedSize);

      if (decompressedData != NULL) {
        if ((int)(decompressedSize * 3) <= (int)(image->sizeX * image->sizeY * 3)) {
          memcpy(image->data, decompressedData, decompressedSize * 3);
        } else {
          fprintf(stderr, "Decompressed data is larger than original image.\n");
        }
        free(decompressedData);
      }

      Display();
      break;
    }
  case 12:
    {
      Color* clut = NULL;
      int clutSize = 0;
      int k = 16;

      buildCLUT(image->data, image->sizeX, image->sizeY, &clut, &clutSize);
      if (clutSize > 0) {
        sortCLUT(clut, clutSize);
        reduceColors(image->data, image->sizeX, image->sizeY, clut, clutSize, k);
      } else {
        fprintf(stderr, "CLUT is empty or failed to build.\n");
      }

      if (clut != NULL) {
        free(clut);
      }

      Display();
      break;
    }

case 13:
 {
  

  //CLUT = malloc(256 * sizeof(GLubyte) * 3);  

  int k = 16;
 // int numPixels = get_num_pixels(image);

  compressImage(image, k, "compressed_image.bin");
  // Décompression immédiate
  image = decompressImage("compressed_image.bin");
  Display();
  free(centers);
  free(assignments);

 /* int numPixels = image->width * image->height;
  Cluster* centers = malloc(k * sizeof(Cluster));
  int* assignments = malloc(numPixels * sizeof(int));

  // Perform K-Means clustering 
  kmeans(image, k, 100);  

  for(int i=0; i<numPixels; i++) {
    int index = assignments[i];
    image->data[i*3] = CLUT[index*3];
    image->data[i*3+1] = CLUT[index*3+1];
    image->data[i*3+2] = CLUT[index*3+2]; 
  }
  free(assignments);
  free(centers);
  free(CLUT);
  //displayImage(image);
  Display();
*/

 default:
  break;
     } 


  case 14:
    {
  Image* decompressedImage;
  decompressedImage = decompressImage("compressed_image.bin");

  if(!decompressedImage) {
    printf("Error decompressing image\n");
    break; 
  }
  // Assign decompressed image to original 
  image = decompressedImage;
  Display();
  // Free resources
  free(decompressedImage);

  break;
  }

  case 15:
    {
  applyDither(image);
  Display();
  break;
  }

}
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: palette filename\n");
    exit(0);
  }
  CLUT = malloc(256 * 3);
  if (CLUT == NULL) {
    fprintf(stderr, "Failed to allocate memory for CLUT\n");
    exit(1);
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("VPUP8");

  if (Init(argv[1]) == -1) {
    fprintf(stderr, "Failed to initialize\n");
    exit(1);
  } 

  
  Init(argv[1]);

  glutCreateMenu(menuFunc);
  glutAddMenuEntry("Quit", 0);
  glutAddMenuEntry("Vert", 1);
  glutAddMenuEntry("Vert uniforme", 2);
  glutAddMenuEntry("Gris uniforme", 3);
  glutAddMenuEntry("Rouge origin", 4);
  glutAddMenuEntry("Swap G and R", 5);
  glutAddMenuEntry("Swap B and R", 6);
  glutAddMenuEntry("Save", 7);
  glutAddMenuEntry("Infos", 8);
  glutAddMenuEntry("Sort colors", 9);
  glutAddMenuEntry("Compress RLE", 10);
  glutAddMenuEntry("Decompress RLE", 11);
  glutAddMenuEntry("Reduce colors", 12);
  glutAddMenuEntry("K-Means clustering", 13);
  glutAddMenuEntry("K-means Read/Decompress", 14);
  glutAddMenuEntry("Dithering", 15);
  glutAttachMenu(GLUT_LEFT_BUTTON);

  glutDisplayFunc(Display);  
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);
  
  glutMouseFunc(Mouse);

  glutMainLoop();  
  //menuFunc(13);
  return 1; 
}

/*
void freeTestImage(Image* image) {
  free(image->data);
  free(image);
} */
/*
int main() {

  // Set up test image -> Run -> Free -> Run -> Valide -> Free -> Clean
  Image* testImage = createTestImage(100, 100); 
  printf("Compressing image...\n");
  compressWithKmeans(testImage, 10, "test.bin");
  freeTestImage(testImage);
  printf("Decompressing image...\n");
  Image* decompressed = decompressImage("test.bin");
  validateImage(decompressed);
  freeTestImage(decompressed);

  // Clean
  remove("test.bin");

  return 0;

}
*/


/// ----------------------------------------------------------------

/// ----------------------------------------------------------------

// ci dessous des nombreuses tentatives reliés au kmeans.c 

/*
// Compress image
case 13: {

  int numPixels = get_num_pixels(image);

  compressWithKmeans(image, 16, numPixels, "file.bin");

  Display();

  break;

}

// Decompress image  
case 14: {

  int k = 16;
  int numPixels = get_num_pixels(image);

  // Read directly into image
  image->clusters = readCompressedImage(k, numPixels, "file.bin");  

  if(!image->clusters){
    printf("Error reading file\n");
    break;
  }

  Display();

  // Liberree  
  free(image->clusters);  

  break;

}

*/


/*
case 13: {

  int numPixels = get_num_pixels(image);

  compressWithKmeans(image, 16, numPixels, "file"); 

  // Essai sans lecture dans l'image

  Display();

  break;

}
*/
//case 14: {
/*
  int k = 16;
  int numPixels = get_num_pixels(image);


  //Image* decompressedImage = NULL;
  Cluster* decompressedImage;
  decompressedImage = readCompressedImage(k, numPixels, "file");

  if(!decompressedImage) {
    printf("Error reading file\n");
    break;
  }

  // Essai sans original image
  freeImage(image);  

  image = decompressedImage;

  Display();

  // Libere
  free(assignments); 
  free(centers); // from readCompressedImage  
 
   int k = 16; 
  int numPixels = get_num_pixels(image);

  Image* decompressedImage;

  decompressedImage = readCompressedImage(k, numPixels, "file");

  if(!decompressedImage) {
    printf("Error reading file\n");
    break;
  }

  
  Image* newImage = malloc(sizeof(Image));

  newImage->width = decompressedImage->width;
  newImage->height = decompressedImage->height; 
  newImage->clusters = decompressedImage->clusters;

  // Assign to original variable
  image = newImage;  

  Display();

  // Clean up
  free(assignments);
  free(centers); 
  free(decompressedImage);

  break;
 */
  
  
  // Enieme tentative pour solution 

  /*int k = 16;  
  char* filename = "compressed_image.bin";

  Image* image;

  image = readCompressedImage(k, filename);

  if(!image) {
    printf("Error reading file\n");
    break;
  }
  Display(image);
  // Liberree
  free(image->clusters);
  free(image);
  break;
  */
 /*
  int k = 16;
  int numPixels = get_num_pixels(image);
  int width, height;
  Cluster* decompressedClusters;

  decompressedClusters = readCompressedImage(k, "filename");

  if(!decompressedClusters) {
    printf("Error reading file\n");
    break;
  }

  Image* decompressedImage = malloc(sizeof(Image));

  decompressedImage->width = width; 
  decompressedImage->height = height;
  decompressedImage->clusters = decompressedClusters;

  image = decompressedImage;

  Display();

  // Liberree
  free(assignments);
  free(centers);
  free(decompressedClusters); 
  free(decompressedImage);
  
  break;

}
*/
