'use strict'

var net = require('net');

try {
var client = new net.Socket();

client.connect(5566, '192.168.31.133', function() {
    console.log('Connected');
    client.write("Hello Arduion Yun");
});

client.on('data', function(data){
    console.log('Received data ' + data );
    
});

client.on('close', function() {
    console.log("socket close");
});

} catch ( err ) {
    console.log('err ' + err );
}
