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
CFLAGS=$(PLATFORMSPECIFICCFLAGS) -Wall -c -g -Wconversion -Werror -pg

CC=g++

FILES=main.cpp Scene.cpp

PROGNAME=raytrace

HANDINDIR=csc473proj1

OBJECTS=$(FILES:.cpp=.o)

all: $(FILES) $(PROGNAME)

$(PROGNAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

run:
	./${PROGNAME} +W640 -H 480 -I simple.pov

run2:
	./${PROGNAME} +W640 -H 480 -I simple_spec.pov

bunny:
	./${PROGNAME} -w640 -h480 -i bunny_small.pov

bigbun0:
	./${PROGNAME} -w160 -h120 -i bunny_large.pov

bigbun1:
	./${PROGNAME} -w320 -h240 -i bunny_large.pov

pokeball:
	./${PROGNAME} +W640 -H 480 -I pokeball.pov

clean:
	rm -f *.o */*.o ${PROGNAME}

handin:
	handin zwood $(HANDINDIR) $(FILES)
	handin zwood $(HANDINDIR)
