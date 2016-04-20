/*
 * node-simc - Node.Js wrapper for SimulationCraft C/C++
 *
 * Copyright (c) 2016 Stephen Parente
 * All rights reserved.
 */

#ifndef SRC_SIMULATION_H_
#define SRC_SIMULATION_H_

#include <nan.h>
#include "simulationcraft.hpp"

namespace NodeSimc {

class Simulation : public Nan::ObjectWrap {
	public:
		static void Init(v8::Local<v8::Object>);

		int Run(const std::vector<std::string>&);

	protected:
		Simulation();
  	~Simulation();

		static Nan::Persistent<v8::Function> constructor;
  	static void New(const Nan::FunctionCallbackInfo<v8::Value>&);

		// Node exposed methods
		static void NodeRun(const Nan::FunctionCallbackInfo<v8::Value>&);

	private:
		sim_t sim;

		bool need_to_save_profiles();

};

}

#endif  // SRC_SIMULATION_H_
