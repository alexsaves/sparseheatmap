//
// Created by Alexei White on 2015-11-26.
//

#ifndef NODE_HEATMAP_SPARSEARRAY_H
#define NODE_HEATMAP_SPARSEARRAY_H

#include <nan.h>

class Sparsearray {
public:
    Sparsearray();
    Sparsearray(int, int, int, unsigned int *);
    ~Sparsearray();

    unsigned int width, height;
    unsigned int* data;
    unsigned int datalen;

    unsigned int area();
};

#endif //NODE_HEATMAP_SPARSEARRAY_H
