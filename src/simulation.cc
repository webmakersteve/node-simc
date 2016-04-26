/*
 * node-simc - Node.Js wrapper for SimulationCraft C/C++
 *
 * Copyright (c) 2016 Stephen Parente
 * All rights reserved.
 */

#include "simulation.hpp"
#include "workers.hpp"

using Nan::FunctionCallbackInfo;
using NodeSimc::Workers::RunSimulation;

namespace NodeSimc {

SimulationResponse::SimulationResponse() {
	canceled = 1;
	player_dps = 0;
}

SimulationResponse::~SimulationResponse() {

}

void SimulationResponse::Load(sim_t * sim) {
	canceled = sim->canceled;

	if (canceled < 1) {
		player_dps = sim->active_player->collected_data.dps.mean();
	}
}

v8::Local<v8::Object> SimulationResponse::AsObject() {
	v8::Local<v8::Object> object = Nan::New<v8::Object>();

	return object;
}

Simulation::Simulation() :
	sim() {
	dbc::init();
}
Simulation::~Simulation() {
	dbc::de_init();
	delete _old_stream_buffer;
}

Nan::Persistent<v8::Function> Simulation::constructor;

void Simulation::New(const FunctionCallbackInfo<v8::Value>& info) {
	if (!info.IsConstructCall()) {
		return Nan::ThrowError("non-constructor invocation not supported");
	}

	Simulation* sim = new Simulation();

	if (info.Length() >= 1 && info[0]->IsObject()) {

	}

	sim->Wrap(info.This());

	info.GetReturnValue().Set(info.This());

}

void Simulation::Init(v8::Local<v8::Object> exports) {
	Nan::HandleScope scope;

	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Simulation").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	tpl->PrototypeTemplate()->Set(Nan::New("run").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(NodeRun)->GetFunction());

	constructor.Reset(tpl->GetFunction());

	exports->Set(Nan::New("Simulation").ToLocalChecked(), tpl->GetFunction());

}

bool Simulation::need_to_save_profiles() {
	if ( sim.save_profiles ) return true;

  for ( size_t i = 0; i < sim.player_list.size(); ++i )
  {
    player_t* p = sim.player_list[ i ];
    if ( ! p -> report_information.save_str.empty() )
      return true;
  }

  return false;
}

void Simulation::StartBuffering() {
	_old_stream_buffer = std::cout.rdbuf(_buffer.rdbuf());
}

std::string Simulation::StopBuffering() {
	return _buffer.str();
}

SimulationResponse * Simulation::Run(const std::vector<std::string>& args) {
	module_t::init();
	unique_gear::register_hotfixes();
	unique_gear::register_special_effects();

	sim_control_t control;
	SimulationResponse * response = new SimulationResponse();

	try
	{
		control.options.parse_args(args);
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR! Incorrect option format: " << e.what() << std::endl;
		return response;
	}

	// Hotfixes are applies right before the sim context (control) is created, and simulator setup
	// begins
	hotfix::apply();

	bool setup_success = true;
	std::string errmsg;
	try
	{
		sim.setup( &control );
	}
	catch( const std::exception& e ){
		errmsg = e.what();
		setup_success = false;
	}

	if ( sim.display_hotfixes )
	{
		std::cout << hotfix::to_str( sim.dbc.ptr );
		return response;
	}

	if ( sim.display_bonus_ids )
	{
		std::cout << dbc::bonus_ids_str( sim.dbc );
		return response;
	}

	if ( ! setup_success )
	{
		std::cerr <<  "ERROR! Setup failure: " << errmsg << std::endl;
		return response;
	}

	if ( sim.canceled ) return response;

	std::cout << std::endl;

	if ( sim.spell_query )
	{
		try
		{
			sim.spell_query -> evaluate();
			// print_spell_query();
		}
		catch( const std::exception& e ){
			std::cerr <<  "ERROR! Spell Query failure: " << e.what() << std::endl;
			return response;
		}
	}
	else if ( need_to_save_profiles() )
	{
		sim.init();
		std::cout << "\nGenerating profiles... \n";
		// report::print_profiles( sim );
	}
	else
	{
		// util::printf( "\nSimulating... ( iterations=%d, threads=%d, target_error=%.3f,  max_time=%.0f, vary_combat_length=%0.2f, optimal_raid=%d, fight_style=%s )\n\n",
		//	iterations, threads, target_error, max_time.total_seconds(), vary_combat_length, optimal_raid, fight_style.c_str() );

		if ( sim.execute() )
		{
			sim.scaling      -> analyze();
			sim.plot         -> analyze();
			sim.reforge_plot -> start();
		}
		else
			sim.canceled = 1;
	}

	response->Load(&sim);

	return response;
}

std::vector<std::string> Simulation::GetArgs(v8::Local<v8::Array> parameter) {
	std::vector<std::string> newItem;

	if (parameter->Length() >= 1) {

    for (unsigned int i = 0; i < parameter->Length(); i++) {
      Nan::MaybeLocal<v8::String> p =
        Nan::To<v8::String>(parameter->Get(i));
      if (p.IsEmpty()) {
        continue;
      }

      Nan::Utf8String pVal(p.ToLocalChecked());
      std::string pString(*pVal);

      newItem.push_back(pString);
    }
	}

	return newItem;
}

void Simulation::NodeRun(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	Nan::HandleScope scope;

	Simulation* sim = ObjectWrap::Unwrap<Simulation>(info.This());

	if (!info[0]->IsArray()) {
		Nan::ThrowError("Must provide a configuration array");
		return;
	}

	std::vector<std::string> args = GetArgs(info[0].As<v8::Array>());

	if (info[1]->IsFunction()) {
		Nan::Callback * callback = new Nan::Callback(info[1].As<v8::Function>());

		Nan::AsyncQueueWorker(
			new RunSimulation(callback, sim, args));
		info.GetReturnValue().Set(Nan::Null());
	} else {
		SimulationResponse * response = sim->Run(args);
		info.GetReturnValue().Set(response->AsObject());
		delete response;
	}

}

}
