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
        port: Host where is server located (localhost by default)
        port: Port on which to contact server (4096 by default)
tcp_clientMultiThread.exe [nbMessages [host [port]]]
        nbMessages: Number of messages to send to server for broadcast (60 by default)
        port: Host where is server located (localhost by default)
        port: Port on which to contact server (4096 by default)

Remark
------
The only difference between tcp_clientMonoThread.exe and tcp_clientMultiThread.exe is in the way the work, the former
witrh a single thread and the latter with two threads (one thread for sending messages and the other one for
receiving them).