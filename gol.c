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
#include "gol_frontend.h"
#include "gol.h"

#include "list.h"

#define TITLE   "Game of Life - the ressurection"
#define VERSION "0.3.1"
#define AUTHOR  "(c) Peter Jönsson (peter.joensson@gmail.com)"
#define LICENSE "Licensed under the MIT License"
#define MAXLEN 256

// Uncomment and recompile to get debug traces.
//#define _DEBUG_ 

extern int running;
extern int step;
extern int simulation;
extern float sleepTime;
extern float sleepFactor;
extern LifeBoard *board;
extern float scaleFactor;
extern CircularList *clist;

static void printUsage(char *);

int main(int argc, char **argv)
{
	int boardSize = 0;
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

	// get the random juice flowing.
#ifndef ARUN
	sranddev();
#endif
	// we scale with 2 since we will move (0, 0) to the bottom
	// left corner later.
	scaleFactor = scaleFactor * 2.0f;
	int windowSize = boardSize * (int)scaleFactor;
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

	clist = createCircularList(10);

	while (running) {
		
		glfwPollEvents();
		if (!glfwGetWindowParam(GLFW_OPENED)) {
			return 0;
		}

		// sleep and calculate next generation.
		if (simulation) {
			(void)glClear(GL_COLOR_BUFFER_BIT);
			renderBoard(board, s);
			glfwSwapBuffers();
#ifdef _DEBUG_
			printf("Sleeping %f seconds.\n", sleepTime);
			(void)fflush(NULL);
#endif
			glfwSleep(sleepTime);
			// store old board in list.
			addToCircularList(clist, board->matrix);
			// calculate the new board.
			calculateLifeTorus(board);
			
			snprintf(windowTitle, MAXLEN, "%s (%d generation)",
				 TITLE, generation);
			glfwSetWindowTitle(windowTitle);
			generation++;
			if(step) {
				simulation = GL_FALSE;
			}
		}
		
	}

	// Cleanup before we leave.
	glfwTerminate();
	destroyCircularList(clist);
//	destroyLifeBoard(board);

	return 0;
}


/**
 *
 *
 */
static void printUsage(char *name)
{
	printf("%s - %s\n", TITLE, VERSION);
	printf("%s - %s\n", LICENSE, AUTHOR);
	printf("%s <board size> <scale factor> <update interval>\n", name);
}

