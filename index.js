/*
 * node-simc - Node.Js wrapper for SimulationCraft C/C++
 *
 * Copyright (c) 2016 Stephen Parente
 * All rights reserved.
 */

var binding = require('bindings')('node-simc');

module.exports = binding;

var sim = new binding.Simulation();

var r = sim.run([
	'armory=us,illidan,john',
	'calculate_scale_factors=1'
], function(err, response) {
	console.log('done');
});

console.log(r);
