//
// Created by Alexei White on 2015-11-29.
//

#include "colorengine.h"
#include <math.h>
#include <stddef.h>

/**
 * Constructor for a new color engine
 */
Colorengine::Colorengine(int ccount) {
  colorcount = ccount;
  data = new int[ccount * 4];
  finalcolors = new int[4 * 256];
  cindex = 0;
}

/**
 * Constructor for a new color engine
 */
Colorengine::~Colorengine() {
  delete[] data;
  delete[] finalcolors;
  finalcolors = NULL;
  data = NULL;
}

/**
 * Generate intermediary colors
 */
void Colorengine::prepare() {
  for (int i = 0; i < 256; i++) {
    float prog = (float)i / 255.0;
    float rprog = prog * (float)(colorcount - 1);
    int basement = (int)floor(rprog);
    int roof = (int)ceil(rprog);
    float brprog = rprog - (float)basement;
    int stidx = i * 4;
    float fr = (float)data[(basement * 4)];
    float rr = (float)data[(roof * 4)];
    float fg = (float)data[(basement * 4) + 1];
    float rg = (float)data[(roof * 4) + 1];
    float fb = (float)data[(basement * 4) + 2];
    float rb = (float)data[(roof * 4) + 2];
    float fa = (float)data[(basement * 4) + 3];
    float ra = (float)data[(roof * 4) + 3];

    float finr = (fr * (1.0 - brprog)) + (rr * brprog);
    float fing = (fg * (1.0 - brprog)) + (rg * brprog);
    float finb = (fb * (1.0 - brprog)) + (rb * brprog);
    float fina = (fa * (1.0 - brprog)) + (ra * brprog);
    finalcolors[stidx] = (int)finr;
    finalcolors[stidx + 1] = (int)fing;
    finalcolors[stidx + 2] = (int)finb;
    finalcolors[stidx + 3] = (int)fina;
  }
}

/**
 * Add a color to the color engine
 */
void Colorengine::add_color(int r, int g, int b, int a) {
  if (cindex < (colorcount * 4)) {
    data[cindex] = r;
    data[cindex + 1] = g;
    data[cindex + 2] = b;
    data[cindex + 3] = a;
    cindex += 4;
  }
}

/**
 * Get a color for an intensity
 */
Clr Colorengine::get_color(int intensity) {
  Clr res;
  int rin = (int)intensity * 4;
  res.r = (char)finalcolors[rin];
  res.g = (char)finalcolors[rin + 1];
  res.b = (char)finalcolors[rin + 2];
  res.a = (char)finalcolors[rin + 3];
  return res;
}