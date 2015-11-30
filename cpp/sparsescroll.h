#ifndef __SPARSESCROLL_DEFINED__
#define __SPARSESCROLL_DEFINED__

#include <nan.h>
#include "sparsearray.h"
#include "colorengine.h"

class Sparsescroll {
public:
    Sparsescroll(int, int, int, int, int);

    ~Sparsescroll();

    int width, height, lastIntensityHeight, lastIntensitySize, lastIntensityIndex, yMult;
    unsigned long max;
    unsigned long *data;
    int debugMode, filter;

    void set_size(int, int);

    unsigned char *get_intensity_map(int, Colorengine *);

    void integrate_sparsearray(Sparsearray *);

};

#endif