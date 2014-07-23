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
				"include"
			],
			"dependencies": [
				"deps/scarab/libscarab.gyp:scarab"
			]
		}
	]
}
