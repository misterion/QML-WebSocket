You can test Example.qml with node.js + socket.io.
Server code:

var io = require('socket.io').listen(12345);

io.sockets.on('connection', function (socket) {
    socket.emit('news', { hello: 'world' }, {world: {at: 'war'}});
    socket.on('my other event', function (data) {
        console.log(data);
    });
});


 