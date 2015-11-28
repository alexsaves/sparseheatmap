#include "sparsematrix.h"
#include <stddef.h>
#include <node.h>
#include <math.h>

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
 * Get an intensity map
 */
int *Sparsematrix::get_intensity_map(int w) {
  int h = (int) ((float) w / (float) width) * (float) height;
  lastIntensityHeight = h;
  int sz = w * h;
  int *targ = new int[sz];
  lastIntensitySize = sz;
  /*for (int idx = 0; idx < sz; idx++) {
    max = (max < data[idx]) ? data[idx] : max;
    targ[idx] = 92;
  }*/

  /*
  float fmax = (float)max;

  float hf = (float) h - (float) 1;
  float wf = (float) w - (float) 1;
  float myheight = (float) height - (float) 1;
  float mywidth = (float) width - (float) 1;
  int index = 0;
  for (int y = 0; y < h; y++) {
    float yp = (float) y / hf;
    float srcy = yp * myheight;
    int floory = (int) floor(srcy);
    int ceily = (int) ceil(srcy);
    int ybase = floory * width;
    int yceil = ceily * width;
    float yprog = yp - (float) floory;
    for (int x = 0; x < w; x++) {
      float xp = (float) x / wf;
      float srcx = xp * mywidth;
      int floorx = (int) floor(srcx);
      int ceilx = (int) ceil(srcx);
      float xprog = xp - (float) floorx;
      float tl = (float) data[ybase + floorx];
      float tr = (float) data[ybase + ceilx];
      float bl = (float) data[yceil + floorx];
      float br = (float) data[yceil + ceilx];
      float topavr = (tl * (1.0 - xprog)) + (tr * xprog);
      float botavr = (bl * (1.0 - xprog)) + (br * xprog);
      float tots = ((topavr * (1.0 - yprog)) + (botavr * yprog)) / fmax;
      targ[index++] = (int)(tots * 100.0);
    }
  }*/
  return targ;
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