# Using Network Sockets to Communicate on different computers

## Project Description
In This project, I wrote C code to communicate via Network Sockets.
The programs were developed in Windows, but by using a UNIX-like environment: Cygwin, I'm able to run these programs on Linux and MacOS.
Here's how it works:

## Demo
In this demo, I have used the client to connect to localhost, which is itself, meaning both the server and client are running on the same computer. If you want to use the server and client on different computers, make sure the client code has the updated IP address of the computer thats running the server. 
Also ensure that the ports being used in the code are not being used by other processes on your computers.


https://github.com/ZafeerAbbasi/My-Projects/assets/86879362/3a782fa9-899f-43ab-b25d-2a4d0f1c5132


Sockets are used for one computer to communicate with another, one is usually called the server, and the other, client.

1. The server starts by creating a socket for communication.
2. The server attaches a local address and specifies a port to its socket.
    - The address that the server attaches, is the only address that it can recieve data from. You can attach multiple, or even use macros that allow data to be received from any IP Address.
    - This action of a server attaching an address to its socket is called: Binding.
3.  The server then announces its willingness to accept new connections.
    - This action is called: Listen.
4. The server now blocks and waits until a connection is received.
    - This action is called: Accept.
    - After calling this function, the server code halts and does not continue    until it has received a connection.
5. At this point, the server is ready for any connections, so a client can create a socket. (Could have been done earlier but a server is usually set up before client because the client cant function without a server, but a server can wait and handle multiple clients) So now the client creates it's socket for communication.
6. The client now actively attempts to establish a connection and it must connect to the exact IP Address and specified port of the server.
    - This action is called: Connect.
    - If the connection is successful then the server will accept its connection and then continue on with its code since it will be stepping out of blocking mode.
7. Now either the client or server can send data over the connection.
8. Likewise, either the client or server can also receive data over the connection.
9. The last step is to release the connection.

### Possible Improvements
Adding in a feature that allows the client or server to send multiple messages without recieving anything after the first message sent.

Develop multi-threaded communication by allocating one thread for sending data, and one for recieving.

Develop a queue structure for the server in which, the server can finish communicating with one client and then move on to the next one in the queue.

Develop multi-threading request handling for the server such that for every new connection request, open a new thread to communicate with it.





