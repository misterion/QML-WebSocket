var io = require('socket.io').listen(8080);

io.sockets.on('connection', function (socket) {
    socket.on('request', function (data) {
        socket.emit('response', data);
    });
});

