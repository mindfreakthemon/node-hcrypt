{
	"targets": [
		{
			"target_name": "addon",
			"sources": [
				"src/addon.cc",
				"src/scarab_object.cc"
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
