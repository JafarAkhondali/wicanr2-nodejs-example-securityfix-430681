var ws = require("../websocket/index.js");

function getDateTime() {

    var date = new Date();

    var hour = date.getHours();
    hour = (hour < 10 ? "0" : "") + hour;

    var min  = date.getMinutes();
    min = (min < 10 ? "0" : "") + min;

    var sec  = date.getSeconds();
    sec = (sec < 10 ? "0" : "") + sec;

    var year = date.getFullYear();

    var month = date.getMonth() + 1;
    month = (month < 10 ? "0" : "") + month;

    var day  = date.getDate();
    day = (day < 10 ? "0" : "") + day;

    return year + ":" + month + ":" + day + ":" + hour + ":" + min + ":" + sec;

}

var connect = ws.connect("ws://localhost:9091",function(){
    console.log("done ");
    console.log("connect to ws://localhost:9091");
});

connect.on('connect',function(dat) {
    console.log( getDateTime() + " websocket connected, send Hello World");
    connect.sendText(getDateTime() + " Hello World");
});
connect.on('text',function (str) {
    console.log(getDateTime() + " Received Text " + str); 
});


