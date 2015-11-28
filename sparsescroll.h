#ifndef __SPARSESCROLL_DEFINED__
#define __SPARSESCROLL_DEFINED__

#include <nan.h>
#include "sparsearray.h"

class Sparsescroll {
public:
    Sparsescroll(int);

    ~Sparsescroll();

    int height;
    unsigned long max;
    unsigned long *data;

    void set_size(int);

    void integrate_sparsearray(Sparsearray *);

};

#endif