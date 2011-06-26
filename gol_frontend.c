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

#define ARC4RANDOM_MAX      0x100000000

extern int running;
extern int step;
extern int simulation;
extern float sleepTime;
extern float sleepFactor;
extern LifeBoard *board;
extern float scaleFactor;

/**
 *
 *
 */
static void renderSquare(int x, int y, float s, Colour c)
{
	float z = 0.0f;
	float fX = (float)x * (s*1.0f);
	float fY = (float)y * (s*1.0f);

#ifdef _DEBUG_
	printf("[(%f, %f),",   fX,   fY);
	printf( "(%f, %f),",   fX+s, fY);
	printf( "(%f, %f),",   fX+s, fY-s);
	printf( "(%f, %f)]\n", fX,   fY-s);
	(void)fflush(NULL);
#endif
	glBegin(GL_QUADS);
	glColor3f(c.red, c.green, c.blue);
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
	Colour c = {0.0f, 0.0f, 0.0f};

	// quick sanity check.
	assert(board != NULL);
	assert(scale > 0.0f);

	for(int x=0; x<board->boardSize; x++) {
		for(int y=0; y<board->boardSize; y++) {
			if(getCell(board, x, y) == true) {
#ifdef BSD
				c.red =   (float)arc4random()/ARC4RANDOM_MAX;
				c.green = (float)arc4random()/ARC4RANDOM_MAX;
				c.blue =  (float)arc4random()/ARC4RANDOM_MAX;
#else
				c.red =   (float)random()/ARC4RANDOM_MAX;
				c.green = (float)random()/ARC4RANDOM_MAX;
				c.blue =  (float)random()/ARC4RANDOM_MAX;
#endif
				renderSquare(x, y, scale, c);
			}
		}
	}

	return;
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

	return;
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

	// we need to scale down the (x, y) coordinats to match the size of
	// the internal board data structure.
	(void)glfwGetMousePos(&x, &y);
	x = x / (int)scaleFactor;
	y = y / (int)scaleFactor;

	boolean currentState = getCell(board, x, y);
	boolean newState = currentState ? false : true;
	(void)setCell(board, x, y, newState);

//#ifdef _DEBUG_
	printf("Button %d, with action %d on ", button, action);
	printf("(%d, %d)\n", x, y);
	printf("currentState: %d, setting newState: %d\n", currentState,
	       newState);
	(void)fflush(NULL);
//#endif

	return;
}
