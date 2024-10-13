OS := $(shell uname)

CC = clang
AR = ar 
WARNINGS = -Wno-unused-function \
		-Wno-absolute-value \
		-Wno-tautological-pointer-compare
CFLAGS = $(WARNINGS) -c -Im68k -I. -I../inc/external -O2 --std=c99 -fPIC
CFLAGS_M68K = $(WARNINGS) -c -Im68k -I. -I../inc/external -O2 --std=c99 -fPIC
LDFLAGS = -L../lib

ifdef COVERAGE
ifeq ($(OS),Darwin)
	CFLAGS += -fprofile-instr-generate -fcoverage-mapping
	LDFLAGS += -fprofile-instr-generate
else
	CFLAGS += -fprofile-arcs -ftest-coverage
	LDFLAGS += -lgcov
endif
endif

all: libmegadrive

clean:
	rm *.o m68k/*.o hqx/*.o m68k/m68kops.h m68k/m68kmake

libmegadrive: m68k/m68kcpu.o m68k/m68kops.o m68k/m68kopac.o m68k/m68kopdm.o m68k/m68kopnz.o m68k/m68kdasm.o eva.o evasound.o megadrive.o vdp.o input.o scale.o z80.o hqx/init.o hqx/hq2x.o hqx/hq3x.o hqx/hq4x.o
		@echo "Linking libmegadrive.a and moving to (root)/lib"
		@$(AR) rc libmegadrive.a m68k/m68kcpu.o m68k/m68kops.o m68k/m68kopac.o m68k/m68kopdm.o m68k/m68kopnz.o m68k/m68kdasm.o eva.o evasound.o megadrive.o vdp.o input.o scale.o z80.o hqx/init.o hqx/hq2x.o hqx/hq3x.o hqx/hq4x.o
		@mv libmegadrive.a ../lib

%.o: %.c
		@echo "Compiling $<"
		$(CC) $(CFLAGS) $^ -std=c99 -o $@

m68k/m68kcpu.o: m68k/m68kops.h m68k/m68k.h m68k/m68kconf.h m68k/m68kcpu.c
		@echo "Compiling m68k/m68kcpu.c"
		$(CC) $(CFLAGS_M68K) m68k/m68kcpu.c -o m68k/m68kcpu.o

m68k/m68kdasm.o: m68k/m68kdasm.c m68k/m68k.h m68k/m68kconf.h
		@echo "Compiling m68k/m68kdasm.c"
		@$(CC) $(CFLAGS_M68K) m68k/m68kdasm.c -o m68k/m68kdasm.o

m68k/m68kops.o: m68k/m68kmake m68k/m68kops.h m68k/m68kops.c m68k/m68k.h m68k/m68kconf.h
		@echo "Compiling m68k/m68kops.c"
		@$(CC) $(CFLAGS_M68K) m68k/m68kops.c -o m68k/m68kops.o

m68k/m68kopac.o: m68k/m68kmake m68k/m68kops.h m68k/m68kopac.c m68k/m68k.h m68k/m68kconf.h
		@echo "Compiling m68k/m68kopac.c"
		@$(CC) $(CFLAGS_M68K) m68k/m68kopac.c -o m68k/m68kopac.o

m68k/m68kopdm.o: m68k/m68kmake m68k/m68kops.h m68k/m68kopdm.c m68k/m68k.h m68k/m68kconf.h
		@echo "Compiling m68k/m68kopdm.c"
		@$(CC) $(CFLAGS_M68K) m68k/m68kopdm.c -o m68k/m68kopdm.o

m68k/m68kopnz.o: m68k/m68kmake m68k/m68kops.h m68k/m68kopnz.c m68k/m68k.h m68k/m68kconf.h
		@echo "Compiling m68k/m68kopnz.c"
		@$(CC) $(CFLAGS_M68K) m68k/m68kopnz.c -o m68k/m68kopnz.o

m68k/m68kops.h: m68k/m68kmake
		@echo "Generating m68k/m68kops.h"
		@m68k/m68kmake m68k m68k/m68k_in.c >/dev/null

m68k/m68kmake: m68k/m68kmake.c m68k/m68k_in.c
		@echo "Compiling m68k/m68kmake"
		@$(CC) $(WARNINGS) m68k/m68kmake.c -o m68k/m68kmake

