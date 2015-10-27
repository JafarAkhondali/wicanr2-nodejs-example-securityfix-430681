var exec=require('child_process').exec;
var spawn=require('child_process').spawn;
var execPath = process.argv[2];
var argv = process.argv;
argv.shift();
argv.shift();
argv.shift();

console.log("execPath = " + execPath + ", argv=" + argv);

proc = spawn(execPath,argv);

proc.stdout.on('data', function(data) {
    console.log("stdout: " + data );
});

proc.stderr.on('data', function(data) {
    console.log("stdout: " + data );
});

proc.on('exit', function(code) {
    console.log("chiled process exited with code " + code);
});
