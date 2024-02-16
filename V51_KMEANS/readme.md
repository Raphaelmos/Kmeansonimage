

# Why this new version ? 

This new version exist because of the part of unexpected that came with the previous versions.

⚠ Warning : This code is full of comments of the many attempts to progress for a better outcome.


# How to execute
```
make && ./palette <image.ppm>
```


# What exactly changed ? 

- One of the main changes is the remove of the random part that was initialized in the start of the K-means algorithms in kmeans.c which one of the problem for the colors. It got replaced by a clean histogram.
- The calculation in the function "kmeansUpdateCenters" had a overflow and got fixed.


### French 

Cette dernière version doit normalement bien s'éxecuter avec les deux nouvelles fonctions et pas de warnings, pas de random, et l'ajout d'un histogram.

Les commentaires permettent de comprendre comment j'ai avancé.


Made by Raphaël 
