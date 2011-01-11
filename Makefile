# Compiler settings
CC     = /Developer/usr/llvm-gcc-4.2/bin/llvm-gcc-4.2
CFLAGS = -std=c99 -g -Wall -I/usr/local/include -L/usr/local/lib

OS := $(shell uname)

# Linker settings
ifeq ($(OS),Darwin)
	LFLAGS = -lglfw -lm -lc -framework AGL -framework OpenGL -framework Cocoa
endif
ifeq ($(OS), Linux)
	# add linux linker stuff here.
endif

gol: gol.c
	${CC} ${CFLAGS} gol.c gol_backend.c ${LFLAGS} -o gol

clean:
	rm -Rf gol
	rm -Rf gol.dSYM
