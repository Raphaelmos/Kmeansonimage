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


void Keyboard(unsigned char key, int x, int y)  {
  switch(key){
  case ESCAPE :
    exit(0);                   
    break;
  default:
    fprintf(stderr, "Unused key\n");
  }
}

void Mouse(int button, int state, int x, int y) {
  switch(button){
  case GLUT_LEFT_BUTTON:
    break;
  case GLUT_MIDDLE_BUTTON:
    break;
  case GLUT_RIGHT_BUTTON:
    break;    
  }
  glutPostRedisplay();
}

int Init(char *s) {
  image = (Image *) malloc(sizeof(Image));
  if (image == NULL) {
    fprintf(stderr, "Out of memory\n");
    return(-1);
  }
  if (ImageLoad_PPM(s, image)==-1) 
	return(-1);
  printf("tailles %d %d\n",(int) image->sizeX, (int) image->sizeY);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glutReshapeWindow(image->sizeX, image->sizeY);
  
  return (0);
}
int ReInit() {
  /*
  if (ImageLoad_PPM(s, image)==-1) 
	return(-1);
  */
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glutReshapeWindow(image->sizeX, image->sizeY);
  
  return (0);
}

void Display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  //GLint w = glutGet(GLUT_WINDOW_WIDTH);
  //GLint h = glutGet(GLUT_WINDOW_HEIGHT);
  glDrawPixels(image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, 
	       image->data);

  glFlush();
}

void Reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble)h);
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
      printf("Original size: %ld, Compressed size: %d, Compression ratio: %.2f\n", image->sizeX * image->sizeY * 3, compressedSize, (float)compressedSize / (image->sizeX * image->sizeY * 3));

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

     // int k;  
    //k = 16;

   // Allocate CLUT
  CLUT = malloc(256 * sizeof(GLubyte) * 3);  

  // Allocate arrays
  int k = 16;
  int numPixels = image->width * image->height;
  Cluster* centers = malloc(k * sizeof(Cluster));
  int* assignments = malloc(numPixels * sizeof(int));

  if(k <= 0){
  printf("Invalid k not good ");
  return;
}
  // Perform K-Means clustering 
  kmeans(image, k, 100);  

  // Quantize image using CLUT
  for(int i=0; i<numPixels; i++) {
    int index = assignments[i];
    image->data[i*3] = CLUT[index*3];
    image->data[i*3+1] = CLUT[index*3+1];
    image->data[i*3+2] = CLUT[index*3+2]; 
  }
    /*centers = malloc(k * sizeof(Cluster));
    if(!centers) { }
    int numPixels = get_num_pixels(image);
    assignments = malloc(numPixels * sizeof(int));
    if(!assignments) { } */
  free(assignments);
  free(centers);
  free(CLUT);
  //displayImage(image);
  Display();

  break;

 default:
  break;
     } 


  case 14:
    {
      /*
    GLubyte *dst = malloc(width*height);

    dither(image, dst, width, height);

  Display(dst);

  free(dst);

      Display();
      break;
    }
*/
  break;
  }
  
}
}

int main(int argc, char **argv) {  

  if (argc<2) {
    fprintf(stderr, "Usage : palette nom_de_fichier\n");
    exit(0);
  }
  CLUT = malloc(256*3);

  glutInit(&argc, argv); 
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowSize(640,480);  
  glutInitWindowPosition(100, 100);  
  glutCreateWindow("VPUP8");  

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
  glutAddMenuEntry("Dithering", 14);
  glutAttachMenu(GLUT_LEFT_BUTTON);

  glutDisplayFunc(Display);  
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);
  
  glutMouseFunc(Mouse);

  glutMainLoop();  

  return 1;
}
