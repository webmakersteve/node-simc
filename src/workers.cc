/*
 * node-simc - Node.Js wrapper for SimulationCraft C/C++
 *
 * Copyright (c) 2016 Stephen Parente
 * All rights reserved.
 */

#include "workers.hpp"

using NodeSimc::Simulation;

namespace NodeSimc {
namespace Workers {

RunSimulation::RunSimulation(Nan::Callback *callback, Simulation* sim, std::vector<std::string> args):
	Nan::AsyncProgressWorker(callback),
	_sim(sim),
	_args(args) {}

RunSimulation::~RunSimulation() {}

void RunSimulation::Execute(const ExecutionProgress& progress) {

	_response = _sim->Run(_args);

	if (_response->canceled > 0) {
		SetErrorMessage("Simulation canceled");
	}

}

void RunSimulation::HandleOKCallback() {
  Nan::HandleScope scope;

	const unsigned int argc = 2;
  v8::Local<v8::Value> argv[argc] = { Nan::Null(), _response->AsObject() };

  callback->Call(argc, argv);

}

void RunSimulation::HandleErrorCallback() {
  Nan::HandleScope scope;

  const unsigned int argc = 2;
  v8::Local<v8::Value> argv[argc] = { Nan::Error(ErrorMessage()), Nan::Null() };

  callback->Call(argc, argv);
}

void RunSimulation::HandleProgressCallback(const char* progress, size_t size) {

}

}  // end Workers
}  // end NodeSimc
