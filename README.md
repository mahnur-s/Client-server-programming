# Client-server-programming
This code snippet is for a server application in a client-server architecture, specifically designed to handle file upload, download, and connection termination operations. The server is implemented in C and uses sockets for network communication. Here's a breakdown of the key functionalities:

Server Socket Initialization: The server creates a socket and binds it to a specified port number received from the command line arguments. It then listens for incoming client connections.

Client Connection Handling: The server accepts connections from clients. Each client connection is handled in a continuous loop, waiting for client requests.

Processing Client Requests: The server can handle three types of commands from clients: UPLOAD, DOWNLOAD, and TERMINATE.

UPLOAD: The server receives a file name, file size, and then the file content from the client. It saves the received file in the "server_files" directory.
DOWNLOAD: The server receives a file name from the client, checks if the file exists, and if so, sends the file size followed by the file content back to the client.
TERMINATE: Closes the client and server sockets, effectively terminating the connection.
Error Handling: The server includes basic error handling for socket operations (creation, binding, listening, and accepting connections) and file operations (opening files for reading/writing).

Data Transfer: File data is transferred in chunks using buffers. The server reads/writes data in blocks, handling large files efficiently.

Closing Sockets: After handling a TERMINATE command or completing a series of client requests, the server closes the client socket and its own server socket.

This server application demonstrates fundamental concepts of network programming in C, including socket programming, handling client-server communication, and basic file operations in a network context.
