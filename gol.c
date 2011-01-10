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

int main(int argc, char **argv)
{
	int running = GL_TRUE;
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

	while (running) {
		glClear( GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers();

		running = !glfwGetKey(GLFW_KEY_ESC) &&
			glfwGetWindowParam(GLFW_OPENED);

		calculateLifeSphere(board);
//		glfwSleep(1.0);
		// render sphere in window.
	}

	// Cleanup before we leave.
	glfwTerminate();
	destroyLifeBoard(board);

	return 0;
}
