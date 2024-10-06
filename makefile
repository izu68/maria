CC = clang

INTERNAL_INC = inc
EXTERNAL_INC = inc/external
INTERNAL_LIB = lib
EXTERNAL_LIB = /usr/local/lib

all:
	cd backend && make
	clang -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL \
	 -I$(INTERNAL_INC) -I$(EXTERNAL_INC) -L$(INTERNAL_LIB) -L$(EXTERNAL_LIB) -lmegadrive -lraylib src/*.c -o maria

clean:
	cd backend && make clean

run:
	./maria
