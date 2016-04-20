{
  "targets": [
    {
      "target_name": "node-simc",
      "sources": [
	  	"src/simulation.cc",
        "src/binding.cc"
      ],
      "dependencies": [
        "<(module_root_dir)/deps/simc.gyp:simc"
      ],
      "include_dirs": [
        "<(module_root_dir)/deps/simc/engine",
        "<!(node -e \"require('nan')\")"
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
