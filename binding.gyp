{
	"targets": [
		{
			"target_name": "addon",
			"sources": [
				"src/addon.cc",
				"src/crypto_object.cc",
				"src/operator_object.cc"
			],
			"include_dirs": [
			    "<!(node -e \"require('nan')\")",
				"include"
			],
			"dependencies": [
				"deps/scarab/libscarab.gyp:scarab"
			],
            "libraries": [
                "-Wl,-rpath,<!(pwd)/build/Release/",
                "./Release/libscarab.so",
                "-lflint",
                "-lgmp",
                "-lmpfr"
			]
		}
	]
}
