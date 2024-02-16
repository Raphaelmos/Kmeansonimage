#  Makefile 
#  Auteur : Fares BELHADJ
#  Email  : amsi@ai.univ-paris8.fr
#  Date   : 23/02/2006

#  Modificiation : Mehdi Aghayani
#  Email  : mehdi.aghayani@gmail.com
#  Commentaire : modifi� afin de fonctionner avec OpenGL et MacOS X et libpng
#  
#  Universit� Paris VIII

SHELL = /bin/sh
#definition des commandes utilisees
CC = gcc
MACHINE= $(shell uname -s)
#declaration des options du compilateur
#PG_FLAGS = -DOPENGL_1_5

ifeq ($(MACHINE), Darwin)
GL_LDFLAGS = -framework OpenGL -framework GLUT -framework Cocoa
else
GL_LDFLAGS = -lGL -lglut -lGLU
endif

CFLAGS = -Wall -Wno-sequence-point -Wextra -fsanitize=address -fsanitize=leak -ggdb3
LDFLAGS = -lm $(GL_LDFLAGS)
#definition des fichiers et dossiers
PROGNAME = palette
HEADERS = ima.h
SOURCES = main.c ppm.c modif.c rgb_hls.c clut.c kmeans.c dither.c
OBJ = $(SOURCES:.c=.o)
DELPROG = compressed_image.rle

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(PROGNAME)

%.o: %.c $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	@$(RM) $(PROGNAME) $(OBJ) *~ gmon.out core.*
	@$(RM) $(DELPROG)
