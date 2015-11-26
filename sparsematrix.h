#ifndef __X_H_INCLUDED__   // if x.h hasn't been included yet...
#define __X_H_INCLUDED__   //   #define this so the compiler knows it has been included

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