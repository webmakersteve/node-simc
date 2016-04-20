/*
 * node-simc - Node.Js wrapper for SimulationCraft C/C++
 *
 * Copyright (c) 2016 Stephen Parente
 * All rights reserved.
 */

#include "binding.hpp"

void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  exports->Set(Nan::New("version").ToLocalChecked(),
      Nan::New(SC_VERSION).ToLocalChecked());
}

NODE_MODULE(simc, Init)
