CC = clang

INC_DIR = -Iinc
LIB_DIR = -Llib

RAYLIB_INC_DIR = -Iinc/raylib
HOMEBREW_INC_DIR = -I/usr/local/include -I/opt/homebrew/include -I/opt/local/include
HOMEBREW_LIB_DIR = -L/usr/local/lib -L/opt/homebrew/lib -L/opt/local/lib

FRAMEWORKS = -framework CoreVideo \
		-framework Cocoa  \
		-framework OpenGL -framework IOKit 
LIBS = -lmegadrive -lraylib

all:
	cd backend && make
	clang $(FRAMEWORKS) $(INC_DIR) $(RAYLIB_INC_DIR) \
	$(HOMEBREW_INC_DIR) $(HOMEBREW_LIB_DIR) $(LIB_DIR) $(LIBS) \
	src/*.c -o maria

clean:
	cd backend && make clean

run:
	./maria

sntest:
	./maria test/sonic.md
