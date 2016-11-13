var download = require('download-tarball');
var fs = require('fs');

download({
	url: 'https://github.com/mindfreakthemon/scarab/archive/master.tar.gz',
	dir: 'deps'
})
	.then(function () {
		fs.renameSync('deps/scarab-master', 'deps/scarab');
		console.log('scarab was downloaded');
	})
	.catch(function (err) {
		console.log(err);
	});


download({
	url: 'https://github.com/mindfreakthemon-contributes/flint2/archive/trunk.tar.gz',
	dir: 'deps'
})
	.then(function () {
		fs.renameSync('deps/flint2-trunk', 'deps/flint');
		console.log('flint2 was downloaded');
	})
	.catch(function (err) {
		console.log(err);
	});

