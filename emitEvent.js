var EventEmitter = require("events").EventEmitter;
var bomb = new EventEmitter();

console.log("create EventEmitter bomb");
bomb.on('boom',function(arg1) {
    console.log("booom!!!! arg1 = " + arg1 );
});

console.log("emit boom");
bomb.emit("boom","test");


