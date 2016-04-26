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

class SimulationResponse {
	public:
		SimulationResponse();
		~SimulationResponse();

		void Load(sim_t*);
		v8::Local<v8::Object> AsObject();

		int canceled;
	protected:
		double player_dps;
};

class Simulation : public Nan::ObjectWrap {
	public:
		static void Init(v8::Local<v8::Object>);

		SimulationResponse * Run(const std::vector<std::string>&);

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
		static std::vector<std::string> GetArgs(v8::Local<v8::Array>);

		void StartBuffering();
		std::string StopBuffering();

		std::stringstream _buffer;
		std::streambuf * _old_stream_buffer;

};

}

#endif  // SRC_SIMULATION_H_
