# Client-Server App
## How to Install and Run the Project
Open console (Bash, CMD, PowerShell etc.) in project's directory and type:
```console
$ cmake .
$ cmake --build .
```
After successful builing you should open terminal windows in 'server' directory and type following (\<port\> - is desired server's listening socket port number):
```console
./server <port>
```
And now you can run the client by opening another one terminal in 'client' directory and typing following (\<addr\> - is host's IPv4 address you are connecting to, \<port\> - its port number):
```console
./client <addr> <port>
```
## How to Use the Project
On the client side you can input a string and if it contains only numbers and is less than 64 characters, then string will be sorted descending, all even number will be replaced with characters 'KB' and sum of remaining numbers will be sended to server.
On the server side this message will be received and if the number has more than 2 digits and multiple of 32, then it will be printed to the console.
