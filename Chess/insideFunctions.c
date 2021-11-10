#include <stdlib.h>
#include "map.h"

MapKeyElement copyId(MapKeyElement id)
{
    if (id == NULL)
    {
        return NULL;
    }
    int* copy = malloc(sizeof(int));
    if (copy != NULL)
    {
        *copy = *(int*)id;
    }
    return (MapKeyElement)copy;
}

void freeId(MapKeyElement id)
{
    free(id);
}

int compareIds(MapKeyElement id1, MapKeyElement id2)
{
    return *(int*)id1 - *(int*)id2;
}

void swapForInt(int* x, int* y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void swapForDouble(double* x, double* y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

int findMaxIdx(double a[], int b[], int n)
{
    int idxmax = 0;
    for (int i = 1; i < n; i++)
    {
        if (a[i] > a[idxmax] || (a[i] == a[idxmax] && b[i] < b[idxmax]))
            idxmax = i;
    }
    return idxmax;
}

void sort(double a[], int b[], int n)
{
    int idxmax;
    while (n > 1)
    {
        idxmax = findMaxIdx(a, b, n);
        swapForDouble(&a[n - 1], &a[idxmax]);
        swapForInt(&b[n - 1], &b[idxmax]);
        n--;
    }
    return;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}