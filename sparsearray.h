//
// Created by Alexei White on 2015-11-26.
//

#ifndef NODE_HEATMAP_SPARSEARRAY_H
#define NODE_HEATMAP_SPARSEARRAY_H

#include <nan.h>

class Sparsearray : public Nan::ObjectWrap {
public:
    static void Init();
    static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);

    unsigned int width, height;
    unsigned int* data;

    unsigned int area();
private:
    Sparsearray();
    Sparsearray(int, int, unsigned int *);
    ~Sparsearray();

    static Nan::Persistent<v8::Function> constructor;
    static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
};

#endif //NODE_HEATMAP_SPARSEARRAY_H
