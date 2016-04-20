{
	"targets": [
		{
			"target_name": "simc",
			'type': "static_library",
			"sources": [
				"simc/engine/util/xml.cpp",
				"simc/engine/util/str.cpp",
				"simc/engine/util/stopwatch.cpp",
				"simc/engine/util/rng.cpp",
				"simc/engine/util/io.cpp",
				"simc/engine/util/concurrency.cpp",
				"simc/engine/sim/sc_sim.cpp",
				"simc/engine/sim/sc_scaling.cpp",
				"simc/engine/sim/sc_reforge_plot.cpp",
				"simc/engine/sim/sc_raid_event.cpp",
				"simc/engine/sim/sc_progress_bar.cpp",
				"simc/engine/sim/sc_plot.cpp",
				"simc/engine/sim/sc_option.cpp",
				"simc/engine/sim/sc_gear_stats.cpp",
				"simc/engine/sim/sc_expressions.cpp",
				"simc/engine/sim/sc_event.cpp",
				"simc/engine/sim/sc_core_sim.cpp",
				"simc/engine/sim/sc_cooldown.cpp",
				"simc/engine/report/sc_report_xml.cpp",
				"simc/engine/report/sc_report_text.cpp",
				"simc/engine/report/sc_report_json.cpp",
				"simc/engine/report/sc_report_html_sim.cpp",
				"simc/engine/report/sc_report_html_player.cpp",
				"simc/engine/report/sc_report.cpp",
				"simc/engine/report/sc_highchart.cpp",
				"simc/engine/report/sc_chart.cpp",
				"simc/engine/player/sc_unique_gear.cpp",
				"simc/engine/player/sc_set_bonus.cpp",
				"simc/engine/player/sc_proc.cpp",
				"simc/engine/player/sc_player.cpp",
				"simc/engine/player/sc_pet.cpp",
				"simc/engine/player/sc_item.cpp",
				"simc/engine/player/sc_enchant.cpp",
				"simc/engine/player/sc_consumable.cpp",
				"simc/engine/interfaces/sc_wowhead.cpp",
				"simc/engine/interfaces/sc_js.cpp",
				"simc/engine/interfaces/sc_http.cpp",
				"simc/engine/interfaces/sc_bcp_api.cpp",
				"simc/engine/dbc/sc_spell_info.cpp",
				"simc/engine/dbc/sc_spell_data.cpp",
				"simc/engine/dbc/sc_item_data_import_ptr.cpp",
				"simc/engine/dbc/sc_item_data_import_noptr.cpp",
				"simc/engine/dbc/sc_item_data.cpp",
				"simc/engine/dbc/sc_data.cpp",
				"simc/engine/dbc/sc_const_data.cpp",
				"simc/engine/class_modules/sc_warrior.cpp",
				"simc/engine/class_modules/sc_warlock.cpp",
				"simc/engine/class_modules/sc_shaman.cpp",
				"simc/engine/class_modules/sc_rogue.cpp",
				"simc/engine/class_modules/sc_priest.cpp",
				"simc/engine/class_modules/sc_paladin.cpp",
				"simc/engine/class_modules/sc_monk.cpp",
				"simc/engine/class_modules/sc_mage.cpp",
				"simc/engine/class_modules/sc_hunter.cpp",
				"simc/engine/class_modules/sc_enemy.cpp",
				"simc/engine/class_modules/sc_druid.cpp",
				"simc/engine/class_modules/sc_death_knight.cpp",
				"simc/engine/buff/sc_buff.cpp",
				"simc/engine/action/sc_stats.cpp",
				"simc/engine/action/sc_spell.cpp",
				"simc/engine/action/sc_sequence.cpp",
				"simc/engine/action/sc_dot.cpp",
				"simc/engine/action/sc_distance_targeting.cpp",
				"simc/engine/action/sc_attack.cpp",
				"simc/engine/action/sc_action_state.cpp",
				"simc/engine/action/sc_action.cpp",
				"simc/engine/sc_util.cpp"
			],
			"dependencies": [],
			"cflags": [
				"-std=c++11",
				"-stdlib=libc++"
			],
			"include_dirs": [
				"<(module_root_dir)/deps/simc/engine",
			],
			'cflags!': [
				'-fno-exceptions',
				'-fno-rtti'
			],
			'cflags_cc!': [
				'-fno-exceptions',
				'-fno-rtti'
			],
			'conditions': [
				[
					'OS=="linux"',
					{

					}
				],
				[
					'OS=="mac"',
					{
						'xcode_settings': {
							'OTHER_CFLAGS': [
								'-ObjC'
							],
							'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
							'GCC_ENABLE_CPP_RTTI': 'YES',
							'MACOSX_DEPLOYMENT_TARGET': '10.7',
							'OTHER_CPLUSPLUSFLAGS': [
								'-std=c++11',
								'-stdlib=libc++'
							]
						},
					}
				]
			]
		}
	]
}
