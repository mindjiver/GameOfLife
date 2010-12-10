#ifndef __BOL_BACKEND_H_
#define __BOL_BACKEND_H_

/**
 *
 *
 */
typedef enum boolean { true = 1, false = 0 } boolean;

/**
 *
 *
 */
typedef struct t_lifeBoard
{
	boolean **matrix;
	int boardSize;
} t_lifeBoard;

t_lifeBoard *createLifeBoard(int);

void destroyLifeBoard(t_lifeBoard *);

boolean getCell(t_lifeBoard *, int, int);

void calculateLife(t_lifeBoard *);

void calculateLifeSphere(t_lifeBoard *);

#endif
