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

#include <stdlib.h>
#include <stdio.h>

#include "list.h"

/**
 *
 *
 */
CircularList *createCircularList(int size)
{
	CircularList *list = NULL;

//#ifdef _DEBUG_
	printf("Creating a new circular list.\n");
	fflush(NULL);
//#endif
	
	list = (CircularList *)malloc(sizeof(CircularList));
	list->nodes = (Node *)malloc(sizeof(Node) * size);
	list->size = size;
	list->next = 0;

	return list;
}

/**
 *
 *
 */
void destroyCircularList(CircularList *list)
{
//#ifdef _DEBUG_
	printf("Deleting circular list 0x%d.\n", (int)list);
	fflush(NULL);
//#endif

	free(list->nodes);
	free(list);
	
	return;
}

void addToCircularList(CircularList *list, void *data)
{
//#ifdef _DEBUG_
	printf("Adding 0x%d\tto list 0x%d.\n", (int)data, (int)list);
	fflush(NULL);
//#endif
	list->nodes[list->next].value = data;
	// remove parenthesis for fun effects ;)
	list->next = (list->next + 1) % list->size;
}
