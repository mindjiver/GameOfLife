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
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <GL/glfw.h>
	
#include "gol_backend.h"
	
#define MAX_PAYLOAD_LENGTH (1024 * 1024)
	
#define TITLE   "Game of Life - the ressurection"
#define VERSION "0.2"
#define AUTHOR  "(c) Peter Jönsson (peter.joensson@gmail.com)"
#define LICENSE "Licensed under the MIT License"
#define MAXLEN 256
#define ARGUMENTS 4
	
typedef struct message
{
	unsigned int length;
	char payLoad[MAX_PAYLOAD_LENGTH];
} message;

static void printUsage(char *);
static void renderSquare(int, int, float);
static void processKeyPress(int, int);
static void processMouseClick(int, int);
static void childProcess(int *, int *);

// rewrite me!
static int sendLifeBoard(int, t_lifeBoard *);
static int receiveLifeBoard(int, t_lifeBoard *);

// Globals to be updated by callback functions from key and mouse presses.
int running = GL_TRUE;
int rendering = GL_TRUE;

int main(int argc, char **argv)
{
	int serverToClient[2];
	int clientToServer[2];
	int pid = 0;
	int rc = 0;

	int boardSize = 0;
	float scaleFactor = 0.0f;
	float sleepTime = 0.0f;
	char windowTitle[MAXLEN];
	
	if (argc < ARGUMENTS) {
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

	rc = pipe(serverToClient);
	if(rc == -1) {
		perror("IPC server<->client");
		return -1;
	}

	rc = pipe(clientToServer);
	if(rc == -1) {
		perror("IPC client<->server");
		return -1;
	}

	pid = fork();

	switch(pid) {
	case -1:
		perror("Fork failed.");
		exit(-1);
	case 0:
		childProcess(clientToServer, serverToClient);
		// not reached.
	case 1:
		// in parent, so continue.
		break;
			
	}

	// Close unneccessary client handles.
	close(serverToClient[0]);
	close(clientToServer[1]);

//	int windowSize = boardSize * (int)scaleFactor * 2.0f;
	t_lifeBoard *board = createLifeBoard(boardSize);

	if(!board) {
		exit(EXIT_FAILURE);
	}

//	if(!glfwInit()) {
//		exit(EXIT_FAILURE);
//	}

	/* if(!glfwOpenWindow(windowSize, windowSize, 0,0,0,0,0,0, GLFW_WINDOW)) { */
	/* 	glfwTerminate(); */
	/* 	exit(EXIT_FAILURE); */
	/* } */
	
	// set default window title
	snprintf(windowTitle, MAXLEN, TITLE);

	// move (0,0) to lower left corner to make rendering easier.
	//float s = scaleFactor / (float)board->boardSize;	

//	(void)glTranslatef(-1.0f,-1.0f,0.0f);
	
	//setup callback functions for keyboard and mouse.
	(void)glfwSetKeyCallback(&processKeyPress);
	(void)glfwSetMouseButtonCallback(&processMouseClick);

	int generation = 0;

	while (running) {
		
		//glfwPollEvents();

		if (rendering) {

			//(void)glClear(GL_COLOR_BUFFER_BIT);

			for(unsigned int x=0; x<board->boardSize; x++) {
				for(unsigned int y=0; y<board->boardSize; y++) {
					if(board->matrix[x][y] == true) {
						//renderSquare(x, y, s);
					}
				}
			}

			//glfwSwapBuffers();

			(void)sendLifeBoard(clientToServer[0], board);
			(void)receiveLifeBoard(serverToClient[1], board);

			glfwSleep(sleepTime);			

			snprintf(windowTitle, MAXLEN, "%s (%d generation)",
				 TITLE, generation);
			//glfwSetWindowTitle(windowTitle);
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

#ifdef DEBUG
	printf("[(%f, %f),",   fX,   fY);
	printf( "(%f, %f),",   fX+s, fY);
	printf( "(%f, %f),",   fX+s, fY-s);
	printf( "(%f, %f)]\n", fX,   fY-s);
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
#ifdef _DEBUG_
	printf("key %d, with action %d\n", key, action);
	(void)fflush(NULL);
#endif

	// only process on key down
	if (action == GLFW_RELEASE) {
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
	int xPos = 0;
	int yPos = 0;

	// only process on mouse click down.
	if (action == GLFW_RELEASE) {
		return;
	} 

	(void)glfwGetMousePos(&xPos, &yPos);

//#ifdef _DEBUG_
	printf("Button %d, with action %d on ", button, action);
	printf("(%d, %d)\n", xPos, yPos);
	(void)fflush(NULL);
//#endif

}

/**
 *
 */
static void childProcess(int *clientToServer, int* serverToClient)
{
	int rc = 0;
	t_lifeBoard *board = NULL;
	// Close unneccessary client handles.
	close(clientToServer[0]);
	close(serverToClient[1]);
	
	rc = receiveLifeBoard(clientToServer[1], board);
	calculateLifeTorus(board);
	sendLifeBoard(serverToClient[0], board);
}

/**
 *
 */
static int sendLifeBoard(int pipe, t_lifeBoard *board)
{
	printf("Trying to send board to pipe %d ...\n", pipe);
	(void)fflush(NULL);

	message *m = (message *)malloc(sizeof(message));
	m->length = sizeof(unsigned int) + (sizeof(enum) *
					    (board->boardSize * board->boardSize));

	assert(m->length < MAX_PAYLOAD_LENGTH);
        memcpy(m->payLoad, board, m->length);
	(void)write(pipe, m, sizeof(m));
	free(m);
	
	return 0;
}

/**
 *
 */
static int receiveLifeBoard(int pipe, t_lifeBoard *board)
{
	unsigned int i = 0;
	unsigned int length = 0;
	int ret = 0;
	char c;
	char temp[1000];

	printf("Trying to receive board from pipe %d ...\n", pipe);
	(void)fflush(NULL);

	// this should hopefully(?) retrieve the size of what we want to read.
	ret = read(pipe, &i, sizeof(unsigned int));

	do {
		ret = read(pipe, &c, 1);
		temp[i++] = c;
	} while ((ret > 0) && (i < length));
		    
	return -1;
}
