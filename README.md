# K-means Image Clustering



# About this project

This project has been developed on my own, incrementally through several process with the aim of debugging and improving the source code step by step. The
test images used are in PPM format.
Digital images take up a considerable amount of data storage space. Compressing them reduces this memory footprint. 


The aim of this project was to implement
the main steps involved in compressing and decompressing a still image:
  1. Color quantization using the k-means algorithm
  2. Creation of a color indexing table (CLUT)
  3. Image compression and decompression using RLE coding
  4. Decompression and display

In addition, the project sheet stipulated:
  1. Possible application of a dithering algorithm
  2. Performance validation in terms of quality and compression ratios
  3. Implementation of functionalities in the form of an interactive menu


# How to use ?

Clone the repository
```
https://github.com/Raphaelmos/Kmeansonimage.git
```
# Usage

1. Compile the code using make
2. Run the executable with a source image file and desired number of clusters as arguments
3. The image will now appear on your screen, left-click and a menu will appear with many differnet possibilities, to use k-means on the image, select it and in your bash a loading will appear and when at 100%.. Wait a few seconds

Now you see the result of it !



## Possible improvements

Add more and more algorithms, currently have K-means, RLE, Dithering
Extend to more than .ppm images

## French 

Ce projet a été développé de manière seule et incrémentale au fil de plusieurs itérations
et de nombreux essais, dans le but de déboguer et améliorer pas à pas le code source. Les
images tests utilisées sont au format PPM.
L’image numérique occupe une place considérable dans le stockage de données. Sa compression permet de réduire cette empreinte mémoire. L’objectif de ce projet était d’implémenter
les principales étapes de la compression et décompression d’une image fixes :
1. Quantification des couleurs via l’algorithme des k-means
2. Création d’une table d’indexation des couleurs (CLUT)
3. Compression et décompression de l’image selon le codage RLE
4. Décompression et affichage
En outre, la fiche projet stipulait :
1. L’application éventuelle d’un algorithme de dithering
2. La validation des performances en termes de qualité et ratios de compression
3. L’implémentation des fonctionnalités sous forme de menu interactif

