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
#include "gol_backend.h"

#define TITLE   "Game of Life - the ressurection"
#define VERSION "0.1"
#define AUTHOR  "Peter Joensson (peter.joensson@gmail.com)"

#define MAXLEN 256

static void renderSquare(int, int, float);
static void processKeyPress(int, int);
static void processMouseClick(int ,int);

// Globals to be updated by callback functions from key and mouse presses.
int running = GL_TRUE;
int rendering = GL_TRUE;

int main(int argc, char **argv)
{
	// TODO validate command line arguments:
	//  - size of board
	//  - simulation speed
	//  ...
	//  - Profit!	

	int boardSize = 500;
	float sleepTime = 0.1f;
	char windowTitle[MAXLEN];
	t_lifeBoard *board = createLifeBoard(boardSize);

	if(!board) {
		exit(EXIT_FAILURE);
	}

	if(!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	if(!glfwOpenWindow(boardSize, boardSize, 0,0,0,0,0,0, GLFW_WINDOW)) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	// set default window title
	snprintf(windowTitle, MAXLEN, TITLE);
	// Scale up using 2.0 to render in all four quandrants of screen.
	float s = 2.0f / (float)board->boardSize;
	// move (0,0) to lower left corner to make rendering easier.
	(void)glTranslatef(-1.0f,-1.0f,0.0f);
	
	//setup callback functions for keyboard and mouse.
	(void)glfwSetKeyCallback(&processKeyPress);
	(void)glfwSetMouseButtonCallback(&processMouseClick);

	int generation = 0;

	while (running) {
		
		glfwPollEvents();

		if (rendering) {

			(void)glClear(GL_COLOR_BUFFER_BIT);

			for(int x=0; x<board->boardSize; x++) {
				for(int y=0; y<board->boardSize; y++) {
					if(board->matrix[x][y] == true) {
						renderSquare(x, y, s);
					}
				}
			}

			glfwSwapBuffers();

			// sleep and calculate next generation.
			glfwSleep(sleepTime);
			calculateLifeSphere(board);
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
 */
void processKeyPress(int key, int action)
{
#ifdef _DEBUG_
	printf("key %d, with action %d\n", key, action);
	(void)fflush(NULL);
#endif

	// only process on key down
	if (action == 0) {
		return;
	}

	switch(key) {
	case GLFW_KEY_ESC:
	case 'Q':
	case 'q':
		running = GL_FALSE;
		break;
	case 'S':
	case 's':
		// start/stop the simulation.
		rendering = rendering == GL_TRUE ? GL_FALSE : GL_TRUE;
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
#ifdef _DEBUG_
	printf("button %d, with action %d\n", button, action);
	(void)fflush(NULL);
#endif
}
