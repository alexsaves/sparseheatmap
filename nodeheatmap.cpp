#include <nan.h>
#include <node.h>
#include "sparsematrix.h"
#include "sparsearray.h"

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Array;

/**
 * Compile a series of sparse arrays to a single sparse matrix
 */
void Compile(const Nan::FunctionCallbackInfo <v8::Value> &info) {

    if (info.Length() < 4) {
        Nan::ThrowTypeError("Wrong number of arguments: expected 4.");
        return;
    }

    if (!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber() || !info[3]->IsArray()) {
        Nan::ThrowTypeError("Wrong arguments: Expected width (Number), height (Number), layout (Number), data (Array).");
        return;
    }

    double width = info[0]->NumberValue();
    double height = info[1]->NumberValue();
    int layout = info[2]->NumberValue();
    v8::Local<v8::Array> arr = v8::Local<v8::Array>::Cast(info[3]);

    Sparsematrix matrix(width, height);

    v8::Local <v8::Number> num1 = Nan::New(layout);
    v8::Local <v8::Number> num = Nan::New(arr->Length());

    info.GetReturnValue().Set(num);
}

/**
 * Interface with JavaScript and expose our API
 */
void Init(v8::Local <v8::Object> exports) {
    exports->Set(Nan::New("compile").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Compile)->GetFunction());
}

// Let node know about this
NODE_MODULE(sparsematrix, Init)