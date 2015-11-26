#ifndef __SPARSEMATRIX_DEFINED__
#define __SPARSEMATRIX_DEFINED__

#include <nan.h>

class Sparsematrix {
public:
    Sparsematrix(int, int);
    ~Sparsematrix();

    unsigned int width, height;
    unsigned long max;
    unsigned long* data;

    void set_size(int, int);

    unsigned int area();
};

#endif