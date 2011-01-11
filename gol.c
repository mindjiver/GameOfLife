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
#include <GL/glfw.h>
#include "gol_backend.h"

void renderSquare(int x, int y, float s);

int main(int argc, char **argv)
{
	//int running = GL_TRUE;
	int boardSize = 400;

	// TODO validate command line arguments:
	//  - size of board
	//  - simulation speed
	//  ...
	//  - Profit!
	t_lifeBoard *board = createLifeBoard(boardSize);

	if(!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	if(!glfwOpenWindow(boardSize, boardSize, 0,0,0,0,0,0, GLFW_WINDOW)) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowTitle("Game of Life - the ressurection");

	int turns = 10;
	int i = 0;

	float s = 1.0f / (float)board->boardSize;

	while (i < turns) {
		glClear(GL_COLOR_BUFFER_BIT);

		for(int x=0; x<board->boardSize; x++) {
			for(int y=0; y<board->boardSize; y++) {
				if(board->matrix[x][y] == true) {
					renderSquare(x, y, s);
				} 
				
			}			
		}

		glfwSwapBuffers();

		//running = !glfwGetKey(GLFW_KEY_ESC) &&
		//	glfwGetWindowParam(GLFW_OPENED);

		calculateLifeSphere(board);
		glfwSleep(1.0);
		// render sphere in window.
		i++;
	}

	// Cleanup before we leave.
	glfwTerminate();
	destroyLifeBoard(board);

	return 0;
}


void renderSquare(int x, int y, float s)
{
	float z = 0.0f;

	float f_x = (float)x * s;
	float f_y = (float)y * s;

	// this should be the coordinates if we render a
	// correctly scaled quad from (0, 0).
//	printf("rendering (%f, %f),", f_x,   f_y);
//	printf("(%f, %f),",           f_x+s, f_y);
//	printf("(%f, %f),",           f_x+s, f_y-s);
//	printf("(%f, %f)\n",          f_x,   f_y-s);
	
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(f_x,
		   f_y,
	 	   z); 
	glVertex3f(f_x+s,
		   f_y,
	 	   z);   
	glVertex3f(f_x+s,
		   f_y-s,
	 	   z); 
	glVertex3f(f_x,
		   f_y-s,
	 	   z); 
	glEnd();
	
	return;
}
