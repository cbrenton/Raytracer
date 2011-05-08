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

LDFLAGS=$(PLATFORMSPECIFICLDFLAGS) -g -pg
CFLAGS=$(PLATFORMSPECIFICCFLAGS) -Wall -c -g -Wconversion -Werror -O3 -pg
#CFLAGS=$(PLATFORMSPECIFICCFLAGS) -Wall -c -g -Wconversion -Werror -O1 -pg

CC=g++

FILES=main.cpp Pixel.cpp Image.cpp Scene.cpp Geometry.cpp Camera.cpp Light.cpp Plane.cpp Triangle.cpp Box.cpp Cone.cpp Sphere.cpp Matrix4.cpp Semi.cpp Material.cpp

PROGNAME=raytrace

#INPUTNAME=bunny_large_tris
INPUTNAME=bunny_small_tris

HANDINDIR=csc473proj1

OBJECTS=$(FILES:.cpp=.o)

all: $(FILES) $(PROGNAME)

$(PROGNAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

run:
	./${PROGNAME} +W640 -H 480 -a -I ${INPUTNAME}.pov

small:
	./${PROGNAME} +W320 -H 240 -a -I ${INPUTNAME}.pov

tiny:
	./${PROGNAME} +W160 -H 120 -a -I ${INPUTNAME}.pov

noaa:
	./${PROGNAME} +W640 -H 480 -I ${INPUTNAME}.pov

smallnoaa:
	./${PROGNAME} +W320 -H 240 -I ${INPUTNAME}.pov

tinynoaa:
	./${PROGNAME} +W160 -H 120 -I ${INPUTNAME}.pov

bunny:
	./${PROGNAME} -w640 -h480 -i bunny_small.pov

bigbun0:
	./${PROGNAME} -w160 -h120 -i bunny_large.pov

bigbun1:
	./${PROGNAME} -w320 -h240 -i bunny_large.pov

pokeball:
	./${PROGNAME} +W640 -H 480 -a -I pokeball.pov

pokeballnoaa:
	./${PROGNAME} +W640 -H 480 -I pokeball.pov

clean:
	rm -f *.o */*.o ${PROGNAME}

gdb:
	gdb ${PROGNAME}

eog:
	eog ${INPUTNAME}.tga

handin:
	handin zwood $(HANDINDIR) $(FILES)
	handin zwood $(HANDINDIR)
