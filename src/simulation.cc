/*
 * node-simc - Node.Js wrapper for SimulationCraft C/C++
 *
 * Copyright (c) 2016 Stephen Parente
 * All rights reserved.
 */

#include <iostream>
#include "simulation.hpp"

using Nan::FunctionCallbackInfo;


namespace NodeSimc {

Simulation::Simulation() :
	sim() {
	dbc::init();
}
Simulation::~Simulation() {
	dbc::de_init();
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

int Simulation::Run(const std::vector<std::string>& args) {
	module_t::init();
	unique_gear::register_hotfixes();
	unique_gear::register_special_effects();

	sim_control_t control;

	try
	{
		control.options.parse_args(args);
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR! Incorrect option format: " << e.what() << std::endl;
		return 1;
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
		return 0;
	}

	if ( sim.display_bonus_ids )
	{
		std::cout << dbc::bonus_ids_str( sim.dbc );
		return 0;
	}

	if ( ! setup_success )
	{
		std::cerr <<  "ERROR! Setup failure: " << errmsg << std::endl;
		return 1;
	}

	if ( sim.canceled ) return 1;

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
			return 1;
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

	std::cout << std::endl;

	return sim.canceled;
}

void Simulation::NodeRun(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	Nan::HandleScope scope;

	Simulation* sim = ObjectWrap::Unwrap<Simulation>(info.This());

	std::vector<std::string> args;

	sim->Run(args);

	info.GetReturnValue().Set(Nan::Null());
}

}
