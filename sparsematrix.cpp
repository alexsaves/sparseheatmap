#include "sparsematrix.h"
#include <stddef.h>
#include <node.h>

/**
 * Constructor for a new sparse matrix
 */
Sparsematrix::Sparsematrix(int w, int h, int bw, int bh, int lt, unsigned int *bdata) {
  blobwidth = bw;
  blobheight = bh;
  layout = lt;
  blob = bdata;
  blobhalfwidth = (int) bw / 2;
  blobhalfheight = (int) bh / 2;
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
  for (int d = 0; d < width * height; d++) {
    data[d] = 0;
  }
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
Sparsematrix::~Sparsematrix() {
  delete[] data;
  data = NULL;
}

/**
 * Set a blob at a position
 */
void Sparsematrix::set_blob(int x, int y) {
  int ydesto = y - blobhalfheight + blobheight;
  int xdesto = x - blobhalfwidth + blobwidth;
  int blobidx = 0;
  if (xdesto < width && x - blobhalfwidth >= 0 && ydesto < height &&
      (int) y - (int) blobhalfheight >= 0) {
    for (int yp = y - blobhalfheight; yp < ydesto; yp++) {
      int ydpos = yp * width;
      for (int xp = x - blobhalfwidth; xp < xdesto; xp++) {
        data[ydpos + xp] += blob[blobidx++];
      }
    }
  } else {
    int nx = x;
    int ny = y;
    for (int yp = ny - blobhalfheight; yp < ydesto; yp++) {
      int ydpos = yp * width;
      if (yp >= 0 && yp < (int) height) {
        for (int xp = nx - blobhalfwidth; xp < xdesto; xp++) {
          if (xp >= 0 && xp < (int) width) {
            blobidx = ((yp - (ny - blobhalfheight)) * blobwidth) + ((xp - (nx - blobhalfwidth)));
            data[ydpos + xp] += blob[blobidx];
          }
        }
      }
    }
  }
}

/**
 * Integrate a sparse array
 */
void Sparsematrix::integrate_sparsearray(Sparsearray *sp) {
  float spw = (float) sp->width;
  float ow = (float) width;
  int dlen = sp->datalen;
  if (layout == 0) {
    // CENTERFIXEDWIDTH
    int xofs = (int) ((ow - spw) / 2.0);
    for (int idx = 0; idx < dlen; idx += 3) {
      int nx = (int) ((float) sp->data[idx + 1] + xofs);
      int ny = (int) sp->data[idx + 2];
      if (ny <= height + blobhalfheight) {
        set_blob(nx, ny);
      }
    }
  } else if (layout == 1) {
    // STRETCH
    for (int idx = 0; idx < dlen; idx += 3) {
      int nx = (int) (((float) sp->data[idx + 1] / spw) * ow);
      int ny = (int) sp->data[idx + 2];
      if (ny <= (int) height + (int) blobhalfheight) {
        set_blob(nx, ny);
      }
    }
  } else if (layout == 2) {
    // LEFTFIXEDWIDTH
    for (int idx = 0; idx < dlen; idx += 3) {
      int nx = (int) sp->data[idx + 1];
      int ny = (int) sp->data[idx + 2];
      if (ny <= (int) height + (int) blobhalfheight) {
        set_blob(nx, ny);
      }
    }
  }
}