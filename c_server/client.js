var net = require('net');
var client = net.Socket();

client.connect(5566,"127.0.0.1", function() {
    console.log("Connected, send Hello");
    client.write("Hello World\n");
});

client.on('data',function(data){
});

client.on('close',function(){
    console.log("Disconnected");
});
