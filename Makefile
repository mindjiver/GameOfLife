# 
# Copyright (c) 2010 Peter Joensson <peter.joensson@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the right
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#  
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE
#

OS := $(shell uname)

ifeq ($(OS), Darwin)
	SB     = /Developer/usr/clang+llvm-2.8-x86_64-apple-darwin10/scan-build
	CC     = /Developer/usr/clang+llvm-2.8-x86_64-apple-darwin10/bin/clang
	CFLAGS = -std=c99 -g -Wall -I/usr/local/include -L/usr/local/lib
	LFLAGS = -lglfw -lm -lc -framework AGL -framework OpenGL -framework Cocoa
	IFLAGS = 
endif
ifeq ($(OS), Linux)
	CC     = /usr/bin/gcc
	CFLAGS = -std=c99 -g -Wall 
	LFLAGS = -lglfw -lm -lc
	EXTFLAGS = -I../glfw-2.7/include -L../glfw-2.7/lib/x11
endif

gol: gol.c
	$(CC) $(CFLAGS) $(EXTFLAGS) -D ARUN -D gol.c gol_frontend.c gol_backend.c $(LFLAGS) -o gol

scan:
	$(SB) $(CC) $(CFLAGS) gol.c gol_frontend.c gol_backend.c list.c $(LFLAGS) -o gol

clean:
	rm -Rf gol
	rm -Rf gol.dSYM	
