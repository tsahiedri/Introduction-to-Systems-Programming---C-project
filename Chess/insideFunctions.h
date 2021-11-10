#ifndef _INSIDE_FUNCTIONS_H
#define _INSIDE_FUNCTIONS_H
#include "map.h"


MapKeyElement copyId(MapKeyElement id);

/**
 * freeId: Frees an id element (int) from memory.
 */
void freeId(MapKeyElement id);

/**
 * compareIds: Compares 2 ids for the map ADT.
 */
int compareIds(MapKeyElement id1, MapKeyElement id2);

/**
 * swapForInt: A swap function for ints.
 */
void swapForInt(int* x, int* y);

/**
 * swapForDouble: A swap function for doubles.
 */
void swapForDouble(double* x, double* y);

/**
 * findMaxIdx: Finds the index i for which a[i] is the greatest.
 * If there are multiple indexs with the same a[i] it will choose the one with lower b[i].
 * This function is used for sorting.
 */
int findMaxIdx(double a[], int b[], int n);

/**
 * sort: This function is used to sort the player by their level in order to print them.
 * It sorts both a (the players levels) and b (the player id's) in the same order.
 */
void sort(double a[], int b[], int n);

/**
 * max: Return the max of a and b.
 */
int max(int a, int b);

/**
 * min: Returns the min of a and b.
 */
int min(int a, int b);


#endif // _INSIDE_FUNCTIONS_H