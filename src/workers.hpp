/*
 * node-simc - Node.Js wrapper for SimulationCraft C/C++
 *
 * Copyright (c) 2016 Stephen Parente
 * All rights reserved.
 */

#ifndef SRC_WORKERS_H_
#define SRC_WORKERS_H_

#include <nan.h>
#include "simulation.hpp"

namespace NodeSimc {
namespace Workers {

class RunSimulation : public Nan::AsyncProgressWorker {

public:
	RunSimulation(Nan::Callback *, NodeSimc::Simulation*, std::vector<std::string>);
	~RunSimulation();

	void Execute(const ExecutionProgress&);
  void HandleOKCallback();
  void HandleErrorCallback();
	void HandleProgressCallback(const char *, size_t);

private:
	Simulation * _sim;
	NodeSimc::SimulationResponse * _response;
	std::vector<std::string> _args;
};

}
}

#endif
