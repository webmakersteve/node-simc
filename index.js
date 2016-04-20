/*
 * node-simc - Node.Js wrapper for SimulationCraft C/C++
 *
 * Copyright (c) 2016 Stephen Parente
 * All rights reserved.
 */

var binding = require('bindings')('node-simc');

module.exports = binding;

var sim = new binding.Simulation();

console.log(sim);

sim.run();
