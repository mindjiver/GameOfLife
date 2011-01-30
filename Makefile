OS := $(shell uname)

ifeq ($(OS), Darwin)
	CC     = /Developer/usr/clang+llvm-2.8-x86_64-apple-darwin10/bin/clang
	CFLAGS = -std=c99 -g -Wall -I/usr/local/include -L/usr/local/lib
	LFLAGS = -lglfw -lm -lc -framework AGL -framework OpenGL -framework Cocoa
endif
ifeq ($(OS), Linux)
	CC     = /usr/bin/gcc
	CFLAGS = -std=c99 -g -Wall 
	LFLAGS = -lglfw -lm -lc
endif

gol: gol.c
	$(CC) $(CFLAGS) gol.c gol_frontend.c gol_backend.c $(LFLAGS) -o gol

clean:
	rm -Rf gol
	rm -Rf gol.dSYM	
