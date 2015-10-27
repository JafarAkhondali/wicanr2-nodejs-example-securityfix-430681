var ws = require("../websocket/index.js");

function sleep(milliSeconds) {
    var startTime = new Date().getTime();
    while (new Date().getTime() < startTime + milliSeconds);
}

var server = ws.createServer(function(connection) {
    connection.on("text", function(str) {
        console.log("received " + str);
        sleep(1000);
        console.log("response Hello Too Back");
        connection.sendText("Hello Too");
        connection.close();
    });
    connection.on("close", function() {
        console.log("socket close");
    });
});

server.listen(9091);
console.log("open server ws://localhost:9091");
