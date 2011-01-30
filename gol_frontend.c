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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glfw.h>

#include "gol_frontend.h"

extern int running;
extern int step;
extern int simulation;
extern float sleepTime;
extern float sleepFactor;
extern LifeBoard *board;

/**
 *
 *
 */
static void renderSquare(int x, int y, float s)
{
	float z = 0.0f;
	float fX = (float)x * (s*1.0f);
	float fY = (float)y * (s*1.0f);
	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;

#ifdef _DEBUG_
	printf("[(%f, %f),",   fX,   fY);
	printf( "(%f, %f),",   fX+s, fY);
	printf( "(%f, %f),",   fX+s, fY-s);
	printf( "(%f, %f)]\n", fX,   fY-s);
	(void)fflush(NULL);
#endif
	glBegin(GL_QUADS);

	// perhaps make this more dynamic.
	red =   (float)rand()/RAND_MAX;
	green = (float)rand()/RAND_MAX;
	blue =  (float)rand()/RAND_MAX;
	glColor3f(red, green, blue);

	glVertex3f(fX,   fY,   z); 
	glVertex3f(fX+s, fY,   z);   
	glVertex3f(fX+s, fY-s, z); 
	glVertex3f(fX,   fY-s, z); 
	glEnd();

	return;
}

/**
 *
 */
void renderBoard(LifeBoard *board, float scale)
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
		step = GL_FALSE;
		break;
	case GLFW_KEY_RIGHT:
	case 'N':
	case 'n':
		// step one generation forwards.
		simulation = GL_TRUE;
		step = GL_TRUE;
		break;
	case 'P':
	case 'p':
		// step one generation backwards.
		break;
	case GLFW_KEY_UP:
		// increase the simulation speed
		sleepTime -= sleepFactor;
		break;
	case GLFW_KEY_DOWN:
		// decrease the simulation speed
		sleepTime += sleepFactor;
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
	(void)fflush(NULL);
	(void)setCell(board, x, y, newState);
}
