#include "sparsearray.h"
#include <stddef.h>
#include <node.h>

/**
 * Constructor for a new sparse array
 */
Sparsearray::Sparsearray(int w, int h, unsigned int *dta) {
  width = w;
  height = h;
  data = dta;
}

/**
 * Constructor for a new sparse array
 */
Sparsearray::Sparsearray() {
  width = 0;
  height = 0;
  data = NULL;
}

/**
 * Destructor for a new sparse array
 */
Sparsearray::~Sparsearray() {
  delete [] data;
  data = NULL;
}

/**
 * JavaScript Nan constructor
 */
Nan::Persistent<v8::Function> Sparsearray::constructor;

/**
 * JavaScript Init
 */
void Sparsearray::Init() {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Sparsearray").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(tpl->GetFunction());
}

/**
 * Get the area of the array
 */
unsigned int Sparsearray::area() {
  return width * height;
}

/**
 * New handler
 */
void Sparsearray::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Sparsearray* obj = new Sparsearray();
  obj->width = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
  obj->height = info[1]->IsUndefined() ? 0 : info[1]->NumberValue();
  if (!info[2]->IsUndefined()) {
    unsigned int*buf = (unsigned int*) node::Buffer::Data(info[2]->ToObject());
    obj->data = buf;
  }
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

/**
 * Create new instance
 */
v8::Local<v8::Object> Sparsearray::NewInstance(v8::Local<v8::Value> arg) {
  Nan::EscapableHandleScope scope;

  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { arg };
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

  return scope.Escape(instance);
}