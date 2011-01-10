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
#include "gol_backend.h"

/**
 *
 *
 */
boolean **createMatrix(int size)
{
	if (size <= 0) {
		return NULL;
	}

	boolean **matrix = (boolean **)malloc(sizeof(boolean *) * size);
	for (int i = 0; i < size; i++) {
		matrix[i] = (boolean *)malloc(sizeof(boolean) * size);
	}

	return matrix;
}
	
/**
 *
 *
 */
void destroyMatrix(boolean **matrix, int size)
{
	for(int i = 0; i < size; i++) {
		free(matrix[i]);
	}

	free(matrix);
}

/**
 *
 *
 */
t_lifeBoard *createLifeBoard(int boardSize)
{
	if (boardSize <= 0) {
		return NULL;
	}

	t_lifeBoard *lifeBoard = (t_lifeBoard *)malloc( sizeof(t_lifeBoard) );
	lifeBoard->matrix = createMatrix(boardSize);
	lifeBoard->boardSize = boardSize;

	srandom(42);

	// Create a random initialisated board.
	for (int x = 0; x < boardSize; x++) {
		for (int y = 0; y < boardSize; y++) {
			lifeBoard->matrix[x][y] = random() % 2;
		}
	}

	return lifeBoard;
}
	
/**
 *
 *
 */
void destroyLifeBoard(t_lifeBoard *lifeBoard)
{
	if(lifeBoard == NULL) {
		return;
	}

	destroyMatrix(lifeBoard->matrix, lifeBoard->boardSize);
	free(lifeBoard);
}
	
/**
 * Get the value at (x, y) in the game matrix
 * 
 * @Return value of cell (x, y) in game matrix
 */
boolean getCell(t_lifeBoard *lifeBoard, int x, int y)
{
	if (lifeBoard == NULL) {
		return false;
	}

	/* check that we stay inside the game matrix */
	boolean xOk = (x - 1 >= 0) && (x + 1 < lifeBoard->boardSize) ? true : false;
	boolean yOk = (y - 1 >= 0) && (y + 1 < lifeBoard->boardSize) ? true : false;

	if (xOk && yOk) {
		return lifeBoard->matrix[x][y];
	}

	else {
		return false;
	}
}
	
/*		
 * The coordinates of the cells that live around the cell at (x,y)
 * 		
 *	_____________________________________
 *	|           |           |           |
 *	|           |           |           |
 *	| (x-1,y+1) |  (x,y+1)  | (x+1,y+1) |
 *	|           |           |           |
 *	|___________|___________|___________|
 *	|           |           |           |
 *	|           |           |           |
 *	|  (x-1,y)  |   (x,y)   |  (x+1,y)  |     
 *	|           |           |           |
 *	|___________|___________|___________|
 *	|           |           |           |
 *	|           |           |           |
 *	| (x-1,y-1) |  (x,y-1)  | (x+1,y-1) |
 *	|           |           |           |
 *	|___________|___________|___________|
 *						
 */	

/**
 * Calcuate the next life cycle for all the cells
 */
void calculateLife(t_lifeBoard *lifeBoard)
{		
	if (lifeBoard == NULL) {
		return;
	}

	int boardSize = lifeBoard->boardSize;
	boolean **newBoard = (boolean **)malloc(sizeof(boolean *) * boardSize);
	
	for (int i = 0; i < boardSize; i++) {
		newBoard[i] = (boolean *)malloc(sizeof(boolean) * boardSize);
	}
	
	for (int x = 0; x < boardSize; x++) {
		for (int y = 0; y < boardSize; y++) {
			
			int count = 0;		
				
			/* check that we stay inside the game matrix */
			boolean xOk = 
				(x - 1 >= 0) && (x + 1 < boardSize) ? true : false;
			boolean yOk = 
				(y - 1 >= 0) && (y + 1 < boardSize) ? true : false;

			if (xOk && yOk && lifeBoard->matrix[x - 1][y + 1]) {
				count++;
			}
			
			if (yOk && lifeBoard->matrix[x][y + 1]) {
				    count++;
			}
			
			if (xOk && yOk && lifeBoard->matrix[x + 1][y + 1]) {
				count++;
			}
			
			if (xOk && lifeBoard->matrix[x - 1][y]) {
				count++;
			}
			
			if (xOk && lifeBoard->matrix[x + 1][y]) {
				count++;
			}
			
			if (xOk && yOk && lifeBoard->matrix[x - 1][y - 1]) {
				count++;
			}
			
			if (yOk && lifeBoard->matrix[x][y - 1]) {
				count++;
			}
			
			if (xOk && yOk && lifeBoard->matrix[x + 1][y - 1]) {
				count++;
			}

			/*
			  The rules for the game of life are :
			  
			  Any live cell with fewer than two neighbors dies of loneliness.
			  Any live cell with more than three neighbors dies of crowding.
			  Any dead cell with exactly three neighbors comes to life.
			  Any live cell with two or three neighbors lives, unchanged, to the
			  next generation.
			*/					
			
			if (lifeBoard->matrix[x][y]) {
				
				if (count < 2) {
					newBoard[x][y] = false;
				}
				
				else if (count > 3) {
					newBoard[x][y] = false;
				}
				
				else {
					newBoard[x][y] = lifeBoard->matrix[x][y];
				}
				
			}
			
			else {
				
				if (count == 3) {
					newBoard[x][y] = true;
				}
				
				else {
					newBoard[x][y] = false;
				}
					
			}
		}
	}
	
	destroyMatrix(lifeBoard->matrix, lifeBoard->boardSize);
	lifeBoard->matrix = newBoard;

}


/**
 * Calcuate the next life cycle for all the cells with the board projected onto a spehere
 */
void calculateLifeSphere(t_lifeBoard *lifeBoard)
{		

	if (lifeBoard == NULL) {
		return;
	}

	int boardSize = lifeBoard->boardSize;
	boolean **newBoard = (boolean **)malloc(sizeof(boolean *) * boardSize);
	
	for (int i = 0; i < boardSize; i++) {
		newBoard[i] = (boolean *)malloc(sizeof(boolean) * boardSize);
	}

	for (int x = 0; x < boardSize; x++) {
		for (int y = 0; y < boardSize; y++) {
			
			int count = 0;		
			
			/* We need to map boardSize + 1 to 0 and -1 to boardSize */
			int maxBoardSize = boardSize - 1;
			
			int xPlusOne =  (x + 1) > maxBoardSize ? 0 : (x + 1);
			int xMinusOne = (x - 1) < 0 ? maxBoardSize : (x - 1);
			int yPlusOne =  (y + 1) > maxBoardSize ? 0 : (y + 1);
			int yMinusOne = (y - 1) < 0 ? maxBoardSize : (y - 1);
			
			if (lifeBoard->matrix[xMinusOne][yPlusOne]) {
				count++;
			}
			
			if (lifeBoard->matrix[x][yPlusOne]) {
				count++;
			}

			if (lifeBoard->matrix[xPlusOne][yPlusOne]) {
				count++;
			}

			if (lifeBoard->matrix[xMinusOne][y]) {
				count++;
			}
			
			if (lifeBoard->matrix[xPlusOne][y]) {
				count++;
			}
			
			if (lifeBoard->matrix[xMinusOne][yMinusOne]) {
				count++;
			}
			
			if (lifeBoard->matrix[x][yMinusOne]) {
				count++;
			}

			if (lifeBoard->matrix[xPlusOne][yMinusOne]) {
				count++;
			}

			/*
			  The rules for the game of life are :
			  
			  Any live cell with fewer than two neighbors dies of loneliness.
			  Any live cell with more than three neighbors dies of crowding.
			  Any dead cell with exactly three neighbors comes to life.
			  Any live cell with two or three neighbors lives, unchanged, to the
			  next generation.
			*/					
			
			if (lifeBoard->matrix[x][y]) {
				
				if (count < 2) {
					newBoard[x][y] = false;
				}
				
				else if (count > 3) {
					newBoard[x][y] = false;
				}
				
				else {
						newBoard[x][y] = lifeBoard->matrix[x][y];
				}
				
			}
				
			else {
				
				if (count == 3) {
					newBoard[x][y] = true;
				}
				
				else {
					newBoard[x][y] = false;
				}
										
			}
		}
	}

	destroyMatrix(lifeBoard->matrix, lifeBoard->boardSize);
	lifeBoard->matrix = newBoard;
}
	
