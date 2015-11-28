#ifndef __SPARSEMATRIX_DEFINED__
#define __SPARSEMATRIX_DEFINED__

#include <nan.h>
#include "sparsearray.h"

class Sparsematrix {
public:
    Sparsematrix(int, int, int, int, int, unsigned int *);

    ~Sparsematrix();

    int width, height;
    unsigned int layout;
    unsigned long max;
    unsigned long *data;
    unsigned int *blob;
    int blobwidth, blobheight;

    void set_size(int, int);

    void integrate_sparsearray(Sparsearray *);

    void set_blob(int x, int y);

    unsigned int area();

private:
    int blobhalfwidth, blobhalfheight;
};

#endif