# Compiler settings
CC     = gcc
CFLAGS = -std=c99 -g -Wall -I/opt/local/include -L/opt/local/lib

# Linker settings
LFLAGS = -lglfw -lm -lc -framework AGL -framework OpenGL -framework Carbon

gol: gol.c
	${CC} ${CFLAGS} gol.c gol_backend.c ${LFLAGS} -o gol

install:
	mv gol gol.app/Contents/MacOS 
clean:
	rm -Rf gol
