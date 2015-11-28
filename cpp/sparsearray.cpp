#include "sparsearray.h"
#include <stddef.h>
#include <node.h>

/**
 * Constructor for a new sparse array
 */
Sparsearray::Sparsearray(int w, int h, int dl, unsigned int *dta) {
  width = w;
  height = h;
  datalen = dl;
  data = dta;
}

/**
 * Constructor for a new sparse array
 */
Sparsearray::Sparsearray() {
  width = 0;
  height = 0;
  datalen = 0;
  data = NULL;
}

/**
 * Destructor for a new sparse array
 */
Sparsearray::~Sparsearray() {
  delete [] data;
  data = NULL;
}

/**
 * Get the area of the array
 */
unsigned int Sparsearray::area() {
  return width * height;
}
