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
  colors = ccount;
  data = new char[ccount * 4];
  cindex = 0;
}

/**
 * Constructor for a new color engine
 */
Colorengine::~Colorengine() {
  delete[] data;
  data = NULL;
}

/**
 * Add a color to the color engine
 */
void Colorengine::add_color(char r, char g, char b, char a) {
  if (cindex < colors) {
    data[cindex] = r;
    data[cindex + 1] = g;
    data[cindex + 2] = b;
    data[cindex + 3] = a;
    cindex += 4;
  }
}