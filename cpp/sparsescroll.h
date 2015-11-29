#ifndef __SPARSESCROLL_DEFINED__
#define __SPARSESCROLL_DEFINED__

#include <nan.h>
#include "sparsearray.h"

class Sparsescroll {
public:
    Sparsescroll(int, int);

    ~Sparsescroll();

    int width, height, lastIntensityHeight, lastIntensitySize, lastIntensityIndex;
    unsigned long max;
    unsigned long *data;

    void set_size(int, int);

    char *get_intensity_map(int w);

    void integrate_sparsearray(Sparsearray *);

};

#endif