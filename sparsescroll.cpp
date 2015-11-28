#include "sparsescroll.h"
#include <stddef.h>
#include <node.h>

/**
 * Constructor for a new sparse matrix
 */
Sparsescroll::Sparsescroll(int h) {
  set_size(h);
}

/**
 * Set the size of the matrix
 */
void Sparsescroll::set_size(int h) {
  height = h;
  max = 0;
  data = new unsigned long[height];
  for (int d = 0; d < height; d++) {
    data[d] = 0;
  }
};

/**
 * Destructor
 */
Sparsescroll::~Sparsescroll() {
  delete[] data;
  data = NULL;
}

/**
 * Integrate a sparse array
 */
void Sparsescroll::integrate_sparsearray(Sparsearray *sp) {

}