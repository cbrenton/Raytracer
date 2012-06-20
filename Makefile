UNAME=$(shell uname)
ifeq ($(UNAME), Linux)
   # Linux stuff
   PLATFORMSPECIFICCFLAGS=
   PLATFORMSPECIFICLDFLAGS=
else
   # Mac/Windows stuff
   PLATFORMSPECIFICCFLAGS=
   PLATFORMSPECIFICLDFLAGS=
endif

DEBUG=-g
PROFILE=-pg
OPTIMIZE=-O3
STRICT=-Wconversion -Werror
LDFLAGS=$(PLATFORMSPECIFICLDFLAGS) $(DEBUG) $(PROFILE) $(OPTIMIZE)
CFLAGS=$(PLATFORMSPECIFICCFLAGS) -Wall -c $(DEBUG) $(PROFILE) $(OPTIMIZE) $(STRICT)

CC=g++

#FILES=main.cpp Pixel.cpp Image.cpp Scene.cpp Geometry.cpp Camera.cpp Light.cpp Plane.cpp Triangle.cpp Box.cpp Cone.cpp Sphere.cpp Matrix4.cpp Semi.cpp Material.cpp Ray.cpp bvh_node.cpp Mesh.cpp QuadMesh.cpp Quad.cpp
FILES=main.cpp Pixel.cpp Image.cpp Scene.cpp Geometry.cpp Camera.cpp Light.cpp Plane.cpp Triangle.cpp Box.cpp Cone.cpp Sphere.cpp Matrix4.cpp Semi.cpp Material.cpp Ray.cpp bvh_node.cpp Mesh.cpp

PROGNAME=raytrace

INPUT_EXT=pov
INPUT_DIR=input
OUTPUT_DIR=output
#INPUTNAME=test_mesh
#INPUTNAME=cube_mesh
#INPUTNAME=diamond_mesh
INPUTNAME=pawn

HANDINDIR=csc473final

AA_FLAGS=-a -b
NO_AA_FLAGS=-b

OBJECTS=$(FILES:.cpp=.o)

all: $(FILES) $(PROGNAME)
	-@ if [ ! -d ${OUTPUT_DIR} ]; then mkdir ${OUTPUT_DIR}; fi

$(PROGNAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

run:
	./${PROGNAME} +W640 -H 480 ${AA_FLAGS} -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

urn: run

rnu: run

test: noaa eog

8k:
	./${PROGNAME} -a 4 -w 8192 -h 4320 -b -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

HUGE:
	./${PROGNAME} -w 5120 -h 3840 -b -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

NICEHUGE:
	./${PROGNAME} -a 16 -w 5120 -h 3840 -b -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

nicebig:
	./${PROGNAME} +W1280 -H 960 -a=16 -b -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

big:
	./${PROGNAME} +W1280 -H 960 -b -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

small:
	./${PROGNAME} +W320 -H 240 ${AA_FLAGS} -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

tiny:
	./${PROGNAME} +W160 -H 120 ${AA_FLAGS} -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

teeny:
	./${PROGNAME} +W40 -H 30 ${AA_FLAGS} -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

noaa:
	./${PROGNAME} +W640 -H 480 ${NO_AA_FLAGS} -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

smallnoaa:
	./${PROGNAME} +W320 -H 240 ${NO_AA_FLAGS} -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

tinynoaa:
	./${PROGNAME} +W160 -H 120 ${NO_AA_FLAGS} -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

teenynoaa:
	./${PROGNAME} +W40 -H 30 ${NO_AA_FLAGS} -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

bunny:
	./${PROGNAME} -w640 -h480 -i bunny_small.${INPUT_EXT}

bigbun0:
	./${PROGNAME} -w160 -h120 -i bunny_large.${INPUT_EXT}

bigbun1:
	./${PROGNAME} -w320 -h240 -i bunny_large.${INPUT_EXT}

pokeball:
	./${PROGNAME} +W640 -H 480 -a -I pokeball.${INPUT_EXT}

bigpokeball:
	./${PROGNAME} +W1280 -H 960 -a -I pokeball.${INPUT_EXT}

nicepokeball:
	./${PROGNAME} +W640 -H 480 -a=16 -I pokeball.${INPUT_EXT}

nicebigpokeball:
	./${PROGNAME} +W1280 -H 960 -a=16 -I pokeball.${INPUT_EXT}

pokeballnoaa:
	./${PROGNAME} +W640 -H 480 -I pokeball.${INPUT_EXT}

pov:
	vim ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

clean:
	rm -f *.o */*.o ${PROGNAME}

gdb:
	gdb --args ./${PROGNAME} +W640 -H 480 ${AA_FLAGS} -I ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

vg: valgrind

vgb: valgrindbig

vgf: valgrindfull

vge: valgrinderror

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./${PROGNAME} -h 20 -w 20 -b -i ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

valgrindbig:
	valgrind --tool=memcheck --leak-check=yes ./${PROGNAME} -h 100 -w 100 -b -i ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

valgrindfull:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./${PROGNAME} -h 200 -w 200 -b -i ${INPUT_DIR}/${INPUTNAME}.${INPUT_EXT}

valgrinderror:
	./${PROGNAME} -h 20 -w 20 -i ${INPUT_DIR}/simple.${INPUT_EXT}

eog:
	eog ${OUTPUT_DIR}/${INPUTNAME}.tga

handin:
	handin zwood $(HANDINDIR) $(FILES)
	handin zwood $(HANDINDIR)
