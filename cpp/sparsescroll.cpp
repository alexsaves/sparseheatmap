#include "sparsescroll.h"
#include "colorengine.h"
#include "lodepng.h"
#include <stddef.h>
#include <node.h>
#include <math.h>
#include <iostream>

/**
 * Constructor for a new sparse matrix
 */
Sparsescroll::Sparsescroll(int w, int h, int yaxismultiplier, int db, int fl) {
  debugMode = db;
  filter = fl;
  yMult = yaxismultiplier;
  set_size(w, h);
}

/**
 * Set the size of the matrix
 */
void Sparsescroll::set_size(int w, int h) {
  width = w;
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
  int dlen = sp->datalen;
  int tidx = 0;
  for (int idx = 0; idx < dlen; idx++) {
    unsigned int srcVal = sp->data[idx];
    for (int k = 0; k < yMult; k++) {
      if (tidx < height - 1) {
        data[tidx++] += srcVal;
      }
    }
  }
}

/**
 * Get an intensity map
 */
unsigned char *Sparsescroll::get_intensity_map(int w, Colorengine *ce) {
  int h = (int) (((float) w / (float) width) * (float) height);
  lastIntensityHeight = h;
  int sz = w * h;
  lastIntensitySize = sz;
  int osz = height;
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
  double myheight = (double) height - (double) 1;
  int index = 0;
  for (int y = 0; y < h; y++) {
    double yp = (double) y / hf;
    double srcy = yp * myheight;
    int floory = (int) floor(srcy);
    int ceily = (int) ceil(srcy);
    double tt = (double) data[floory];
    double bb = (double) data[ceily];
    double yprog = srcy - (double) floory;
    double tots = ((tt * (1.0 - yprog)) + (bb * yprog));
    Clr intensity_color = ce->get_color((int)tots);
    for (int x = 0; x < w; x++) {
      targ[index++] = intensity_color.r;
      targ[index++] = intensity_color.g;
      targ[index++] = intensity_color.b;
      targ[index++] = intensity_color.a;
    }
  }
  lastIntensityIndex = index - 1;

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