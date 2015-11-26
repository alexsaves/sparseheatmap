#include "sparsematrix.h"
#include <stddef.h>
#include <node.h>

/**
 * Constructor for a new sparse matrix
 */
Sparsematrix::Sparsematrix (int w, int h) {
    set_size(w, h);
}

/**
 * Set the size of the matrix
 */
void Sparsematrix::set_size(int x, int y) {
    width = x;
    height = y;
    max = 0;
    data = new unsigned long[width * height];
};

/**
 * Get the area of the matrix
 */
unsigned int Sparsematrix::area() {
    return width * height;
}

/**
 * Destructor
 */
Sparsematrix::~Sparsematrix () {
    delete [] data;
    data = NULL;
}