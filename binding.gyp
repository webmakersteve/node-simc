{
  "targets": [
    {
      "target_name": "node-simc",
      "sources": [
        "src/binding.cc"
      ],
      "dependencies": [
        "<(module_root_dir)/deps/simc.gyp:simc"
      ],
      "include_dirs": [
        "<(module_root_dir)/deps/simc/engine",
        "<!(node -e \"require('nan')\")"
      ],
      'conditions': [
        [
          'OS=="linux"',
          {

          }
        ]
      ]
    }
  ]
}
