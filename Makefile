OS := $(shell uname)

ifeq ($(OS), Darwin)
	CC     = /Developer/usr/llvm-gcc-4.2/bin/llvm-gcc-4.2
	CFLAGS = -std=c99 -g -Wall -Wextra -I/usr/local/include -L/usr/local/lib
	LFLAGS = -lglfw -lm -lc -framework AGL -framework OpenGL -framework Cocoa
endif
ifeq ($(OS), Linux)
	CC     = /usr/bin/gcc
	CFLAGS = -std=c99 -g -Wall -Wextra
	LFLAGS = -lglfw -lm -lc
endif

gol: gol.c
	$(CC) $(CFLAGS) gol.c gol_backend.c $(LFLAGS) -o gol

clean:
	rm -Rf gol
	rm -Rf gol.dSYM	
