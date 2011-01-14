/* 
 * Copyright (c) 2010 Peter Joensson <peter.joensson@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the right
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *  
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glfw.h>
#include <assert.h>

#include "gol_backend.h"

#define TITLE   "Game of Life - the ressurection"
#define VERSION "0.1"
#define AUTHOR  "(c) Peter Jönsson (peter.joensson@gmail.com)"
#define LICENSE "Licensed under the MIT License"
#define MAXLEN 256

void renderSquare(int, int, float);
void printUsage(char *);

int main(int argc, char **argv)
{
	int running = GL_TRUE;
	int boardSize = 0;
	float sleepTime = 0.0f;
	float scaleFactor = 0.0f;
	char windowTitle[MAXLEN];

	if (argc < 4) {
		printUsage(argv[0]);

		return 0;
	} else {
		boardSize = atoi(argv[1]);
		scaleFactor = atof(argv[2]);
		sleepTime = atof(argv[3]);
	}

	// make sure that the input values are somewhat sane.
	assert(boardSize > 0);
	assert(scaleFactor > 0.0f);
	assert(sleepTime > 0);

	int windowSize = boardSize * (int)scaleFactor * 2.0f;
	t_lifeBoard *board = createLifeBoard(boardSize);

	if(!board) {
		exit(EXIT_FAILURE);
	}

	if(!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	if(!glfwOpenWindow(windowSize, windowSize, 0,0,0,0,0,0, GLFW_WINDOW)) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	// set default window title
	snprintf(windowTitle, MAXLEN, TITLE);

	// move (0,0) to lower left corner to make rendering easier.
	glTranslatef(-1.0f, -1.0f, 0.0f);
	float s = scaleFactor / (float)board->boardSize;	
	int generation = 0;
	while (running) {
		
		glfwSetWindowTitle(windowTitle);

		glClear(GL_COLOR_BUFFER_BIT);

		for(int x=0; x<board->boardSize; x++) {
			for(int y=0; y<board->boardSize; y++) {
				if(getCell(board, x, y) == true) {
					renderSquare(x, y, s);
				} 
				
			}			
		}

		glfwSwapBuffers();

		running = !glfwGetKey(GLFW_KEY_ESC) &&
			glfwGetWindowParam(GLFW_OPENED);

		calculateLifeTorus(board);
		glfwSleep(sleepTime);

		snprintf(windowTitle, MAXLEN, "%s (%d generation)", TITLE,
			 generation);
		generation++;
	}

	// Cleanup before we leave.
	glfwTerminate();
	destroyLifeBoard(board);

	return 0;
}

/**
 *
 *
 */
void renderSquare(int x, int y, float s)
{
	float z = 0.0f;
	float f_x = (float)x * (s*1.0f);
	float f_y = (float)y * (s*1.0f);

#ifdef DEBUG
	printf("[(%f, %f),",   f_x,   f_y);
	printf( "(%f, %f),",   f_x+s, f_y);
	printf( "(%f, %f),",   f_x+s, f_y-s);
	printf( "(%f, %f)]\n", f_x,   f_y-s);
#endif
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(f_x,   f_y,   z); 
	glVertex3f(f_x+s, f_y,   z);   
	glVertex3f(f_x+s, f_y-s, z); 
	glVertex3f(f_x,   f_y-s, z); 
	glEnd();

	return;
}

/**
 *
 *
 */
void printUsage(char *name)
{
	printf("%s - %s\n", TITLE, VERSION);
	printf("%s - %s\n", LICENSE, AUTHOR);
	printf("%s <board size> <scale factor> <update interval>\n", name);
}
