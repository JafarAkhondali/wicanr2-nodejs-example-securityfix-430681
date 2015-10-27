const util = require('util');
const EventEmitter = require('events');

//var MyEventEmitter = function() {
function MyEventEmitter() {
    EventEmitter.call(this);
    console.log("init MyEventEmitter");
}

util.inherits(MyEventEmitter, EventEmitter);

MyEventEmitter.prototype.init = function() {
    console.log("fooBar");
};

module.exports = MyEventEmitter;
