var path = require('path');
var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
app.use(express.static(path.join(__dirname)));
app.get('/', function(req, res){
  res.sendFile('index.html', {root: __dirname});
});


io.on('connection', function(socket) {
  console.log("Got a connection!");
  socket.on('message', function(data) {
    // Broadcast to everyone
    socket.broadcast.emit('message', data);
    // Also send to the emitter because we're making an echo bot after all ;)
    socket.emit('message', data);
  });
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});
