#include <node.h>
#include <v8.h>
#include <nan.h>
#include <stdio.h>
#include <math.h>
#include "sparsescroll.h"
#include "sparsematrix.h"
#include "sparsearray.h"
#include "colorengine.h"

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
void CompileCanvas(const Nan::FunctionCallbackInfo <v8::Value> &info) {

  if (info.Length() < 9) {
    Nan::ThrowTypeError("Wrong number of arguments: expected 9.");
    return;
  }

  if (!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsNumber() || !info[3]->IsArray() ||
      !info[4]->IsNumber() || !info[5]->IsNumber() || !info[6]->IsArray() || !info[7]->IsNumber() ||
      !info[8]->IsArray()) {
    Nan::ThrowTypeError(
            "Wrong arguments: Expected width (Number), height (Number), layout (Number), data (Array), blob width (Number), blob height (Number), blob intensity data (Array), imageWidth (Number), colorArray (Array).");
    return;
  }

  double width = info[0]->NumberValue();
  double height = info[1]->NumberValue();
  int layout = info[2]->NumberValue();
  v8::Local <v8::Array> dataarr = v8::Local<v8::Array>::Cast(info[3]);
  double blobwidth = info[4]->NumberValue();
  double blobheight = info[5]->NumberValue();
  int destImageWidth = (int) info[7]->NumberValue();
  v8::Local <v8::Array> blobArr = v8::Local<v8::Array>::Cast(info[6]);
  v8::Local <v8::Array> colorArr = v8::Local<v8::Array>::Cast(info[8]);

  Colorengine c(colorArr->Length());
  for (unsigned int p = 0; p < colorArr->Length(); p += 4) {
    c.add_color((char) colorArr->Get(p)->Uint32Value(), (char) colorArr->Get(p + 1)->Uint32Value(),
                (char) colorArr->Get(p + 2)->Uint32Value(), (char) colorArr->Get(p + 3)->Uint32Value());
  }

  unsigned int *blobVals = new unsigned int[blobArr->Length()];
  // Iterate through the blob array, adding each element to our list
  for (unsigned int i = 0; i < blobArr->Length(); i++) {
    unsigned int intensityval = blobArr->Get(i)->Uint32Value();
    blobVals[i] = intensityval;
  }

  Sparsearray **sparrs = new Sparsearray *[dataarr->Length()];

  // Do the matrix!
  Sparsematrix matrix(width, height, blobwidth, blobheight, layout, blobVals);

  for (unsigned int d = 0; d < dataarr->Length(); d++) {
    Sparsearray *myNewSP = new Sparsearray();
    v8::Local <v8::Object> myObj = v8::Local<v8::Object>::Cast(dataarr->Get(d));
    myNewSP->width = (unsigned int) myObj->GetInternalField(0)->NumberValue();
    myNewSP->height = (unsigned int) myObj->GetInternalField(1)->NumberValue();
    v8::Local <v8::Array> sparseArr = v8::Local<v8::Array>::Cast(myObj->GetInternalField(2));
    myNewSP->datalen = sparseArr->Length();
    myNewSP->data = new unsigned int[sparseArr->Length()];
    for (unsigned int t = 0; t < sparseArr->Length(); t++) {
      unsigned int scoreval = sparseArr->Get(t)->Uint32Value();
      myNewSP->data[t] = scoreval;
    }
    sparrs[d] = myNewSP;

    matrix.integrate_sparsearray(myNewSP);
  }

  char *finalImageIntensities = matrix.get_intensity_map(destImageWidth);

  for (unsigned int s = 0; s < dataarr->Length(); s++) {
    delete sparrs[s];
  }
  delete[] sparrs;
  delete[] blobVals;

  info.GetReturnValue().Set(Nan::NewBuffer(finalImageIntensities, matrix.lastIntensitySize).ToLocalChecked());
}

/**
 * Compile a series of sparse arrays to a single sparse matrix
 */
void CompileVScroll(const Nan::FunctionCallbackInfo <v8::Value> &info) {

  if (info.Length() < 6) {
    Nan::ThrowTypeError("Wrong number of arguments: expected 6.");
    return;
  }

  if (!info[0]->IsNumber() || !info[1]->IsNumber() || !info[2]->IsArray() || !info[3]->IsNumber() ||
      !info[4]->IsNumber() || !info[5]->IsArray()) {
    Nan::ThrowTypeError(
            "Wrong arguments: Expected height (Number), data (Array), Output image width (Number), Y-axis pixel multiplier (Number), Color array (Number).");
    return;
  }

  double width = info[0]->NumberValue();
  double height = info[1]->NumberValue();
  double destImageWidth = info[3]->NumberValue();
  int yAxisMultiplier = (int) round((double) info[4]->NumberValue());
  if (yAxisMultiplier < 1) {
    yAxisMultiplier = 1;
  }
  v8::Local <v8::Array> dataarr = v8::Local<v8::Array>::Cast(info[2]);
  v8::Local <v8::Array> colorArr = v8::Local<v8::Array>::Cast(info[5]);

  Colorengine c(colorArr->Length());
  for (unsigned int p = 0; p < colorArr->Length(); p += 4) {
    c.add_color((char) colorArr->Get(p)->Uint32Value(), (char) colorArr->Get(p + 1)->Uint32Value(),
                (char) colorArr->Get(p + 2)->Uint32Value(), (char) colorArr->Get(p + 3)->Uint32Value());
  }

  Sparsearray **sparrs = new Sparsearray *[dataarr->Length()];

  // Do the matrix!
  Sparsescroll matrix(width, height, yAxisMultiplier);

  for (unsigned int d = 0; d < dataarr->Length(); d++) {
    Sparsearray *myNewSP = new Sparsearray();
    v8::Local <v8::Object> myObj = v8::Local<v8::Object>::Cast(dataarr->Get(d));
    myNewSP->width = (unsigned int) myObj->GetInternalField(0)->NumberValue();
    myNewSP->height = (unsigned int) myObj->GetInternalField(1)->NumberValue();
    v8::Local <v8::Array> sparseArr = v8::Local<v8::Array>::Cast(myObj->GetInternalField(2));
    myNewSP->datalen = sparseArr->Length();
    myNewSP->data = new unsigned int[sparseArr->Length()];
    for (unsigned int t = 0; t < sparseArr->Length(); t++) {
      unsigned int scoreval = sparseArr->Get(t)->Uint32Value();
      myNewSP->data[t] = scoreval;
    }
    sparrs[d] = myNewSP;

    matrix.integrate_sparsearray(myNewSP);
  }

  char *finalImageIntensities = matrix.get_intensity_map(destImageWidth);

  /*Local <Array> v8Array = Nan::New<Array>();
  unsigned int matlen = matrix.height;
  for (unsigned int s = 0; s < matlen; s++) {
    v8Array->Set(s, Nan::New((double) matrix.data[s]));
  }*/

  for (unsigned int s = 0; s < dataarr->Length(); s++) {
    delete sparrs[s];
  }
  delete[] sparrs;

  //info.GetReturnValue().Set(v8Array);
  info.GetReturnValue().Set(Nan::NewBuffer(finalImageIntensities, matrix.lastIntensitySize).ToLocalChecked());
}

/**
 * Interface with JavaScript and expose our API
 */
void Init(v8::Local <v8::Object> exports) {
  exports->Set(Nan::New("compile_canvas").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(CompileCanvas)->GetFunction());
  exports->Set(Nan::New("compile_vertical_scroll").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(CompileVScroll)->GetFunction());
}

// Let node know about this
NODE_MODULE(sparsematrix, Init
)