#include <nan.h>
#include <node.h>
#include "sparsematrix.h"

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void Compile(const Nan::FunctionCallbackInfo <v8::Value> &info) {

    if (info.Length() < 2) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!info[0]->IsNumber() || !info[1]->IsNumber()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }
    Sparsematrix rect(5, 8);
    rect.set_size(3, 4);

    double arg0 = info[0]->NumberValue();
    double arg1 = info[1]->NumberValue();
    v8::Local <v8::Number> num = Nan::New(arg0 + arg1 + arg0 + rect.area());

    info.GetReturnValue().Set(num);
}

void Init(v8::Local <v8::Object> exports) {
    exports->Set(Nan::New("compile").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Compile)->GetFunction());
}

NODE_MODULE(sparsematrix, Init)