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
				"scarab/libscarab.gyp:scarab"
			],
			"libraries": [
				"-Wl,-rpath,<!(pwd)/build/Release/",
				"-L./Release/libscarab.a",
				"-lflint",
				"-lgmp",
				"-lmpfr"
			]
		}
	]
}
