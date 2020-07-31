TCP Client-Server applications
==============================

Description
-----------
Clients connect to a running server. Each second, each client sends a message. The server broadcasts this message to all connected clients.

Compilation
-----------
- Under Windows
  - Open CMakeLists.txt with VisualStudio
  - Menu Generate > Generate All
  - Executable files are located in "ClientServerSFML\out\build\x64-Debug (par défaut)" directory
- Under Linux (machine ssh.imtbs-tsp.eu)
  - Open a terminal and go to directory where CMakeLists.txt is.
  - mkdir build
  - cd build
  - /mci/inf/simatic/Software/cmake-3.17.3/cmake-build/bin/cmake .. # Use this cmake because installed cmake version is only 3.14 ==> Not recent enough
  - make
  - Executable files are located in the build directory

Usage
-----
tcp_server.exe [port]
       port: Port on which to listen (4096 by default)
tcp_clientMonoThread.exe [nbMessages [host [port]]]
        nbMessages: Number of messages to send to server for broadcast (60 by default)
        host: Host where server is located (localhost by default)
        port: Port on which to contact server (4096 by default)
tcp_clientMultiThread.exe [nbMessages [host [port]]]
        nbMessages: Number of messages to send to server for broadcast (60 by default)
        host: Host where server is located (localhost by default)
        port: Port on which to contact server (4096 by default)
udp_clientMonoThread.exe [localPort [nbMessages [host [remotePort]]]]
        localPort: Port on which client waits fo UDP messages (4096 by default)
	    nbMessages: Number of messages to send to server for broadcast (60 by default)
	    host: Host where server is located (localhost by default)
	    remotePort: Port on which to contact server (4096 by default)
udp_clientMultiThread.exe [localPort [nbMessages [host [remotePort]]]]
        localPort: Port on which client waits fo UDP messages (4096 by default)
	    nbMessages: Number of messages to send to server for broadcast (60 by default)
	    host: Host where server is located (localhost by default)
	    remotePort: Port on which to contact server (4096 by default)
udp_server.exe [port]
	      port: Port on which to listen (4096 by default)

Remark
------
The only difference between [tcp|udp]_clientMonoThread.exe and [tcp|udp]_clientMultiThread.exe is in the way the work, the former
with a single thread and the latter with two threads (one thread for sending messages and the other one for
receiving them). The roundtrip times are more precise with udp_clientMultiThread.exe as we have no wait 
before trying to see if we received a message from the server.

TODO
----
- TODO udp_clientMonoThread and udp_server are incomplete conecerning timeouts. For instance
  - If udp_server is not here, udp_clientMonoThread goes on sending messages as though udp_server was here.
  - If a udp_clientMonoThread does not send a message to udp_server for a long time, udp_server keeps this client in vecClient.