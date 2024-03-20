#pragma once

#include "libqhull_r/qhull_ra.h"

int get_hull(coordT* points, int dim, int numpoints, int** simplices_ptr, int* numsimplices, double* volume, double* area);
int get_delaunay(coordT* points, int dim, int numpoints, int** simplices_ptr, int* numsimplices);
void free_ptr(size_t* simplices_ptr);