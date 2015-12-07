#include "sparsematrix.h"
#include "colorengine.h"
#include "lodepng.h"
#include <stddef.h>
#include <node.h>
#include <math.h>
#include <iostream>

/**
 * Constructor for a new sparse matrix
 */
Sparsematrix::Sparsematrix(int w, int h, int bw, int bh, int lt, unsigned int *bdata, int db, int fl, int tpl, int tpt, int tpr, int tpb) {
  trimPixelsLeft = tpl;
  trimPixelsTop = tpt;
  trimPixelsRight = tpr;
  trimPixelsBottom = tpb;
  debugMode = db;
  filter = fl;
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
unsigned char *Sparsematrix::get_intensity_map(int w, Colorengine *ce) {
  int h = (int) (((float) w / (float) width) * (float) height);
  lastIntensityHeight = h;
  int sz = w * h;
  lastIntensitySize = sz;
  int osz = width * height;
  for (int idx = 0; idx < osz; idx++) {
    max = (max < data[idx]) ? data[idx] : max;
  }
  long double fmax = (long double) max;
  unsigned long total = 0;
  int dcount = 0;
  for (int idx = 0; idx < osz; idx++) {
    long double val = (long double)data[idx];
    long double newval = (unsigned long)((val / fmax) * 255);
    data[idx] = newval;
    total += (unsigned long)newval;
    if (newval > 0) {
        dcount++;
    }
  }
  max = 255;

  if (filter == 1) {
    // lowpass
    double avg = (double)total / (double) dcount;
    if (debugMode == 1) {
        std::cout << "SparseHeatmap DEBUG: Average intensity: " << avg << " Max: " << max << "\n";
        std::cout << "SparseHeatmap DEBUG: Implementing low-pass filter..\n";
    }
    fmax = 0;
    long double downsamplerate = 0.5;
    if (avg < 45) {
        downsamplerate = 0.46;
    }
    if (avg < 35) {
        downsamplerate = 0.36;
    }
    if (avg < 25) {
        downsamplerate = 0.26;
    }
    if (avg < 15) {
        downsamplerate = 0.16;
    }
    if (avg < 5) {
        downsamplerate = 0.06;
    }
    for (int idx = 0; idx < osz; idx++) {
        long double val = (long double)data[idx];
        if (val > (long double)avg) {
            long double newval = ((val - avg) * downsamplerate) + avg;
            data[idx] = newval;
            if (newval > fmax) {
                fmax = newval;
            }
        }
      }
      dcount = 0;
      total = 0;
      for (int idx = 0; idx < osz; idx++) {
          long double val = (long double)data[idx];
          long double newval = (unsigned long)((val / fmax) * 255);
          data[idx] = newval;
          total += (unsigned long)newval;
          if (newval > 0) {
              dcount++;
          }
        }
        avg = (double)total / (double) dcount;
        if (debugMode == 1) {
                std::cout << "SparseHeatmap DEBUG: Average intensity is now: " << avg << " Max: " << max << "\n";
            }
  }

  unsigned char *targ = new unsigned char[sz * 4];
  double hf = (double) h - (double) 1;
  double wf = (double) w - (double) 1;
  double myheight = (double) height - (double) 1;
  double mywidth = (double) width - (double) 1;
  int index = 0;
  for (int y = 0; y < h; y++) {
    double yp = (double) y / hf;
    double srcy = yp * myheight;
    int floory = (int) floor(srcy);
    int ceily = (int) ceil(srcy);
    int ybase = floory * width;
    int yceil = ceily * width;
    double yprog = srcy - (double) floory;
    for (int x = 0; x < w; x++) {
      double xp = (double) x / wf;
      double srcx = xp * mywidth;
      int floorx = (int) floor(srcx);
      int ceilx = (int) ceil(srcx);
      double xprog = srcx - (double) floorx;
      double tl = (double) data[ybase + floorx];
      double tr = (double) data[ybase + ceilx];
      double bl = (double) data[yceil + floorx];
      double br = (double) data[yceil + ceilx];
      double topavr = (tl * (1.0 - xprog)) + (tr * xprog);
      double botavr = (bl * (1.0 - xprog)) + (br * xprog);
      double tots = ((topavr * (1.0 - yprog)) + (botavr * yprog));
      Clr intensity_color = ce->get_color((int)tots);
      targ[index++] = intensity_color.r;
      targ[index++] = intensity_color.g;
      targ[index++] = intensity_color.b;
      targ[index++] = intensity_color.a;
    }
  }
  lastIntensityIndex = index - 1;
  /*
   * unsigned encode(std::vector<unsigned char>& out,
                    const unsigned char* in, unsigned w, unsigned h,
                    LodePNGColorType colortype = LCT_RGBA, unsigned bitdepth = 8);
   */
  std::vector<unsigned char> outVect;
  lodepng::encode(outVect, targ, (unsigned)w, (unsigned)h);
  lastIntensityIndex = outVect.size();
  /*std::cout << "Outvect size:";
  std::cout << x;
  std::cout << "targ size:";
  std::cout << lastIntensityIndex;
  std::cout << "\n";*/
  //std::vector<unsigned char> v(10);
  unsigned char* myArr = new unsigned char[outVect.size()];
  std::copy(outVect.begin(), outVect.end(), myArr);
  delete [] targ;
  /*
   * unsigned lodepng_encode32(unsigned char** out, size_t* outsize,
                          const unsigned char* image, unsigned w, unsigned h);
   */
  //unsigned char* outPNG;
  //lodepng::lodepng_encode32(outPNG)
  return myArr;
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
  int opw = (int)sp->width;
  int oph = (int)sp->height;
  float ow = (float) width;
  int dlen = sp->datalen;
  if (layout == 0) {
    // CENTERFIXEDWIDTH
    int xofs = (int) ((ow - spw) / 2.0);
    for (int idx = 0; idx < dlen; idx += 3) {
      int ox = sp->data[idx + 1];
      int ny = (int) sp->data[idx + 2];
      if (ox > trimPixelsLeft && ox < (opw - trimPixelsRight) && ny > trimPixelsTop && ny < (oph - trimPixelsBottom)) {
        int nx = (int) ((float) ox + xofs);
        if (ny <= height + blobhalfheight) {
          set_blob(nx, ny);
        }
      }
    }
  } else if (layout == 1) {
    // STRETCH
    for (int idx = 0; idx < dlen; idx += 3) {
      int ox = sp->data[idx + 1];
      int nx = (int) (((float) ox / spw) * ow);
      int ny = (int) sp->data[idx + 2];
      if (ox > trimPixelsLeft && ox < (opw - trimPixelsRight) && ny > trimPixelsTop && ny < (oph - trimPixelsBottom)) {
        if (ny <= (int) height + (int) blobhalfheight) {
          set_blob(nx, ny);
        }
      }
    }
  } else if (layout == 2) {
    // LEFTFIXEDWIDTH
    for (int idx = 0; idx < dlen; idx += 3) {
      int nx = (int) sp->data[idx + 1];
      int ny = (int) sp->data[idx + 2];
      if (ny <= ((int) height + (int) blobhalfheight) && (nx > trimPixelsLeft && nx < (opw - trimPixelsRight) && ny > trimPixelsTop && ny < (oph - trimPixelsBottom))) {
        set_blob(nx, ny);
      }
    }
  }
}