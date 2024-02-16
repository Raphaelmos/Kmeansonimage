
#include <stdlib.h>
#include <stdio.h>
#include "ima.h"



/*

int ColorToInt(GLubyte red, GlLubyte green, GLubyte blue){

// convertie un RBG en un int 

 int bits = 8;
 int rgb = 0;
 int nb = 0;
 
 for (int i = bits ; i > 0;i--){
  rgb = rgb | ((1&r)<<nb);
  r = r >> 1;
  nb++;
  rgb = rgb | ((1&g)<<nb);
  g = g >> 1;
  nb++;
  rgb = rgb | ((1&b)<<nb);
  b = b >> 1;
  nb++;
 }
 return rgb;
 }

// Pour la distance 
// abs(ColorToInt(red,green,blue)-ColorToInt(red',green',blue'))

 */
 
 
/*Écrire une fonction qui permet de construire la C-LUT : on utilisera une structure
Color pour construire un tableau de couleurs.*/
void buildCLUT (GLubyte* image, int width, int height, Color** clut, int* clutSize) {
    int nb_pixels = width * height;
    int i, j;
    int found;
    *clutSize = 0;

    *clut = (Color*)malloc(nb_pixels * sizeof(Color));
    if (*clut == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    for (i = 0; i < nb_pixels; i++) {
        found = 0;
        for (j = 0; j < *clutSize; j++) {
            if ((*clut)[j].red == image[i * 3] &&
                (*clut)[j].green == image[i * 3 + 1] &&
                (*clut)[j].blue == image[i * 3 + 2]) {
                (*clut)[j].frequency++;
                found = 1;
                break;
            }
        }

        if (!found) {
            (*clut)[*clutSize].red = image[i * 3];
            (*clut)[*clutSize].green = image[i * 3 + 1];
            (*clut)[*clutSize].blue = image[i * 3 + 2];
            (*clut)[*clutSize].frequency = 1;
            (*clutSize)++;
        }
    }
}


/*Écrire une fonction qui permet de déterminer pour chaque couleur dans la C-LUT
sa fréquence d'apparition, afin de ne garder au final que les couleurs prédominantes.
On pourra commencer par trier la C-LUT via une fonction qui à une couleur associe
un entier, par exemple celle vue en cours. Pour ce faire, on pourra utiliser la fonction
qsort de std lib.h, ou réimplémenter par exemple un tri à bulles.*/
int compareColors(const void* a, const void* b) {
    Color* colorA = (Color*)a;
    Color* colorB = (Color*)b;

    return colorB->frequency - colorA->frequency;
}

void sortCLUT (Color* clut, int clutSize) {
    qsort(clut, clutSize, sizeof(Color), compareColors);
}


/*Écrire une fonction qui sélectionne k couleurs de la C-LUT (les k plus fréquentes),
et qui remplace chaque pixel de l'image par un pixel de la couleur la plus proche
parmi celles sélectionnées. On testera le résultat pro duit pour différentes valeurs de k.
*/
void reduceColors(GLubyte* image, int width, int height, Color* clut, int clutSize, int k) {
    int nb_pixels = width * height;
    k = (k > clutSize) ? clutSize : k;

    for (int i = 0; i < nb_pixels; i++) {
        int red = image[i * 3];
        int green = image[i * 3 + 1];
        int blue = image[i * 3 + 2];

        int minDistance = INT_MAX;
        int minIndex = 0;

        for (int j = 0; j < k; j++) {
            int dr = red - clut[j].red;
            int dg = green - clut[j].green;
            int db = blue - clut[j].blue;
            int distance = dr * dr + dg * dg + db * db;
            if (distance < minDistance) {
                minDistance = distance;
                minIndex = j;
            }
        }

        image[i * 3] = clut[minIndex].red;
        image[i * 3 + 1] = clut[minIndex].green;
        image[i * 3 + 2] = clut[minIndex].blue;
    }
}

