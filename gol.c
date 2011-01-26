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
#define VERSION "0.2"
#define AUTHOR  "(c) Peter Jönsson (peter.joensson@gmail.com)"
#define LICENSE "Licensed under the MIT License"
#define MAXLEN 256

// Uncomment and recompile to get debug traces.
//#define _DEBUG_ 

static void printUsage(char *);
static void renderSquare(int, int, float);
static void processKeyPress(int, int);
static void processMouseClick(int, int);
static void renderBoard(LifeBoard *, float);

// Globals to be updated by callback functions from key and mouse presses.
int running = GL_TRUE;
int simulation = GL_TRUE;
LifeBoard *board = NULL;

int main(int argc, char **argv)
{
	int boardSize = 0.0;
	float scaleFactor = 0.0f;
	float sleepTime = 0.0f;

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
	if (scaleFactor < 2.0f) {
		scaleFactor = 2.0f;
		printf("Scale factor too low, resetting to 2.0\n");
		(void)fflush(NULL);
	}

	assert(boardSize > 0);
	assert(scaleFactor > 0.0f);
	assert(sleepTime > 0);

	// we scale with 2 since we will move (0, 0) to the bottom
	// left corner later.
	int windowSize = boardSize * (int)scaleFactor * 2;
	board = createLifeBoard(boardSize);

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
	float s = scaleFactor / (float)board->boardSize;	
	(void)glTranslatef(-1.0f,-1.0f,0.0f);
	
	//setup callback functions for keyboard and mouse.
	(void)glfwSetKeyCallback(&processKeyPress);
	(void)glfwSetMouseButtonCallback(&processMouseClick);

	int generation = 0;

	while (running) {
		
		glfwPollEvents();
		if (!glfwGetWindowParam(GLFW_OPENED)) {
			return 0;
		}
		
		(void)glClear(GL_COLOR_BUFFER_BIT);
		renderBoard(board, s);
		glfwSwapBuffers();
			
		// sleep and calculate next generation.
		if (simulation) {
			glfwSleep(sleepTime);
			calculateLifeTorus(board);
			snprintf(windowTitle, MAXLEN, "%s (%d generation)",
				 TITLE, generation);
			glfwSetWindowTitle(windowTitle);
			generation++;
		}
		
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
	float fX = (float)x * (s*1.0f);
	float fY = (float)y * (s*1.0f);

#ifdef _DEBUG_
	printf("[(%f, %f),",   fX,   fY);
	printf( "(%f, %f),",   fX+s, fY);
	printf( "(%f, %f),",   fX+s, fY-s);
	printf( "(%f, %f)]\n", fX,   fY-s);
	fflush(NULL);
#endif
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(fX,   fY,   z); 
	glVertex3f(fX+s, fY,   z);   
	glVertex3f(fX+s, fY-s, z); 
	glVertex3f(fX,   fY-s, z); 
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

/**
 *
 *
 */
void processKeyPress(int key, int action)
{
	// only process on key down
	if (action == GLFW_RELEASE) {
		return;
	}

#ifdef _DEBUG_
	printf("key %d, with action %d\n", key, action);
	(void)fflush(NULL);
#endif

	switch(key) {
	case GLFW_KEY_ESC:
	case 'Q':
	case 'q':
		running = GL_FALSE;
		break;
	case 'S':
	case 's':
		// start/stop the simulation.
		simulation = simulation == GL_TRUE ? GL_FALSE : GL_TRUE;
		break;
	case 'N':
	case 'n':
		// step one generation forwards.
		break;
	case 'P':
	case 'p':
		// step one generation backwards.
		break;
	default:
		break;
	}
	
}

/**
 *
 */
void processMouseClick(int button, int action)
{
	int x = 0;
	int y = 0;

	// only process on mouse click down.
	if (action == GLFW_RELEASE) {
		return;
	} 

	(void)glfwGetMousePos(&x, &y);

//#ifdef _DEBUG_
	printf("Button %d, with action %d on ", button, action);
	printf("(%d, %d)\n", x, y);
	(void)fflush(NULL);
//#endif

	boolean currentState = getCell(board, x, y);
	boolean newState = currentState ? false : true;
	printf("currentState: %d, setting newState: %d\n", currentState, newState);
	fflush(NULL);
	(void)setCell(board, x, y, newState);
}

/**
 *
 */
static void renderBoard(LifeBoard *board, float scale)
{
	// quick sanity check.
	assert(board != NULL);
	assert(scale > 0.0f);

	for(int x=0; x<board->boardSize; x++) {
		for(int y=0; y<board->boardSize; y++) {
			if(getCell(board, x, y) == true) {
				renderSquare(x, y, scale);
			}
		}
	}
}
