#ifndef __SPARSEMATRIX_DEFINED__
#define __SPARSEMATRIX_DEFINED__

#include <nan.h>
#include "sparsearray.h"
#include "colorengine.h"

class Sparsematrix {
public:
    Sparsematrix(int, int, int, int, int, unsigned int *, int, int, int, int, int, int);

    ~Sparsematrix();

    int width, height, filter;
    unsigned int layout;
    unsigned long max;
    unsigned long *data;
    unsigned int *blob;
    int debugMode;
    int blobwidth, blobheight, lastIntensityHeight, lastIntensitySize, lastIntensityIndex;
    int trimPixelsLeft, trimPixelsTop, trimPixelsRight, trimPixelsBottom;

    void set_size(int, int);

    void integrate_sparsearray(Sparsearray *);

    void set_blob(int x, int y);

    unsigned char *get_intensity_map(int, Colorengine *);

    unsigned int area();

private:
    int blobhalfwidth, blobhalfheight;
};

#endif