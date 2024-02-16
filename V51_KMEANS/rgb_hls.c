#include "ima.h"

/*Écrire une fonction qui p ermet de trier les 3 couleurs d'une image : on sto ckera
tous les GLubyte de rouge, de vert et de bleu dans un grand tableau.*/
void quickSort(GLubyte* arr, int left, int right) {
    int i = left, j = right;
    GLubyte tmp;
    GLubyte pivot = arr[(left + right) / 2 * 3];
    while (i <= j) {
        while (arr[i * 3] < pivot)
            i++;
        while (arr[j * 3] > pivot)
            j--;
        if (i <= j) {
            for (int k = 0; k < 3; k++) {
                tmp = arr[i * 3 + k];
                arr[i * 3 + k] = arr[j * 3 + k];
                arr[j * 3 + k] = tmp;
            }
            i++;
            j--;
        }
    };

    if (left < j)
        quickSort(arr, left, j);
    if (i < right)
        quickSort(arr, i, right);
}

void sortColors (GLubyte* image, int width, int height, GLubyte* sortedColors) {
    int nb_pixels = width * height;
    memcpy(sortedColors, image, nb_pixels * 3 * sizeof(GLubyte));

    quickSort(sortedColors, 0, nb_pixels - 1);
}


/*Écrire une fonction qui p ermet de compresser un tableau de GLubyte selon le
princip e de l'algorithme RLE naïf.*/
void compressRLE(GLubyte* data, int size, GLubyte** compressedData, int* compressedSize) {
    int i, count;
    int capacity = 100;

    *compressedData = (GLubyte*)malloc(capacity * sizeof(GLubyte));
    if (*compressedData == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    *compressedSize = 0;
    i = 0;
    while (i < size) {
        count = 1;
        while (i + count < size && data[i] == data[i + count] && count < 255) {
            count++;
        }

        if (*compressedSize + 2 > capacity) {
            capacity *= 2;
            *compressedData = (GLubyte*)realloc(*compressedData, capacity * sizeof(GLubyte));
            if (*compressedData == NULL) {
                fprintf(stderr, "Memory reallocation failed.\n");
                return;
            }
        }

        (*compressedData)[(*compressedSize)++] = count;
        (*compressedData)[(*compressedSize)++] = data[i];

        i += count;
    }
}


void compressRLE2(GLubyte* data, int size, GLubyte** compressedData, int* compressedSize) {
    int i = 0;
    int capacity = 100;

    *compressedData = (GLubyte*)malloc(capacity * sizeof(GLubyte));
    if (*compressedData == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    *compressedSize = 0;
    while (i < size) {
        int count = 1;

        while (i + count < size && data[i] == data[i + count] && count < 255) {
            count++;
        }

        if (count > 1 || (i + count < size && data[i] != data[i + count])) {
            if (*compressedSize + 2 > capacity) {
                capacity *= 2;
                *compressedData = (GLubyte*)realloc(*compressedData, capacity * sizeof(GLubyte));
                if (*compressedData == NULL) {
                    fprintf(stderr, "Memory reallocation failed.\n");
                    return;
                }
            }

            (*compressedData)[(*compressedSize)++] = count;
            (*compressedData)[(*compressedSize)++] = data[i];

            i += count;
        } else {
            int start = i;
            while (i < size && (i == start || data[i] != data[i - 1]) && i - start < 255) {
                i++;
            }

            if (*compressedSize + 1 + i - start > capacity) {
                capacity = capacity + i - start + 1;
                *compressedData = (GLubyte*)realloc(*compressedData, capacity * sizeof(GLubyte));
                if (*compressedData == NULL) {
                    fprintf(stderr, "Memory reallocation failed.\n");
                    return;
                }
            }

            (*compressedData)[(*compressedSize)++] = -(i - start);
            for (int j = start; j < i; j++) {
                (*compressedData)[(*compressedSize)++] = data[j];
            }
        }
    }
}


/*Écrire une fonction qui décompresse un tableau de GLubyte, puis une fonction qui
reconstitue l'image à partir du tableau trié de couleurs.*/
void decompressRLE(GLubyte* compressedData, int compressedSize, GLubyte** decompressedData, int* decompressedSize) {
    int i = 0, pos = 0;
    int capacity = 100;
    *decompressedData = (GLubyte*)malloc(capacity * sizeof(GLubyte) * 3);
    if (*decompressedData == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    while (i < compressedSize) {
        int count = compressedData[i++];
        GLubyte value = compressedData[i++];

        while (pos + count > capacity) {
            capacity *= 2;
            *decompressedData = (GLubyte*)realloc(*decompressedData, capacity * sizeof(GLubyte) * 3);
            if (*decompressedData == NULL) {
                fprintf(stderr, "Memory reallocation failed.\n");
                return;
            }
        }

        for (int j = 0; j < count; j++) {
            for (int k = 0; k < 3; k++) {
                (*decompressedData)[pos * 3 + k] = value;
            }
            pos++;
        }
    }
    *decompressedSize = pos;

    printf("Decompressed size: %d\n", *decompressedSize);
    // for (int i = 0; i < *decompressedSize; i++) {
    //     printf("%d ", (*decompressedData)[i]);
    // }
    printf("\n");
}


void decompressRLE2(GLubyte* compressedData, int compressedSize, GLubyte** decompressedData, int* decompressedSize) {
    int i = 0, pos = 0;
    int capacity = 100;

    *decompressedData = (GLubyte*)malloc(capacity * sizeof(GLubyte) * 3);
    if (*decompressedData == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    while (i < compressedSize) {
        int count = compressedData[i++];

        if (count >= 0) {
            // GLubyte value = compressedData[i++];
            while (pos + count > capacity) {
                capacity *= 2;
                *decompressedData = (GLubyte*)realloc(*decompressedData, capacity * sizeof(GLubyte) * 3);
                if (*decompressedData == NULL) {
                    fprintf(stderr, "Memory reallocation failed.\n");
                    return;
                }
            }
        } else {
            count = -count;
            for (int j = 0; j < count; j++) {
                (*decompressedData)[pos++] = compressedData[i++];
            }
        }
    }

    *decompressedSize = pos;
    printf("Decompressed size: %d\n", *decompressedSize);
}



/*
int ColorToInt(GLubyte red, GLubyte green, GLubyte blue){

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



