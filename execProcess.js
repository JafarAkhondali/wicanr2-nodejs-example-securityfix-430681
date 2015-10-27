var exec=require('child_process').exec;
var proc = process.argv[2];

console.log("process.argv[2] " + process.argv[2]);
var child = exec(proc, function(err,stdout,stderr) {

    console.log("stdout:" + stdout);
    console.log(stderr);
    if ( err ) {
    } else {
    }
});
