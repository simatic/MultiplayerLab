#
# TCP perf applications
#
add_executable(tcp_client
  tcp_client.cpp common.h tcp_communication.cpp tcp_communication.h options.cpp options.h OptParserExtended.cpp OptParserExtended.h
        Measures.cpp Measures.h)
if (UNIX)
  target_link_options(tcp_client PUBLIC -pthread)
endif()
add_executable(tcp_server
  tcp_server.cpp common.h tcp_communication.cpp tcp_communication.h options.cpp options.h OptParserExtended.cpp OptParserExtended.h
        Measures.cpp Measures.h)
if (UNIX)
  target_link_options(tcp_server PUBLIC -pthread)
endif()

#
# UDP perf applications
#
add_executable(udp_client
  udp_client.cpp common.h udp_communication.cpp udp_communication.h options.cpp options.h OptParserExtended.cpp OptParserExtended.h
        Measures.cpp Measures.h)
if (UNIX)
  target_link_options(udp_client PUBLIC -pthread)
endif()
add_executable(udp_server
  udp_server.cpp common.h udp_communication.cpp udp_communication.h options.cpp options.h OptParserExtended.cpp OptParserExtended.h
        Measures.cpp Measures.h)
if (UNIX)
  target_link_options(udp_server PUBLIC -pthread)
endif()

#
# ENet perf applications
#
add_executable(enet_client
    enet_client.cpp common.h enet_communication.cpp enet_communication.h options.cpp options.h OptParserExtended.cpp OptParserExtended.h
        Measures.cpp Measures.h)
target_link_libraries(enet_client PUBLIC enet)
if (WIN32)
  target_link_libraries(enet_client PUBLIC ws2_32.lib winmm.lib)
else() # We are under UNIX
  target_link_options(enet_client PUBLIC -pthread)
endif()
add_executable(enet_server
        enet_server.cpp common.h enet_communication.cpp enet_communication.h options.cpp options.h OptParserExtended.cpp OptParserExtended.h
        Measures.cpp Measures.h)
target_link_libraries(enet_server PUBLIC enet)
if (WIN32)
  target_link_libraries(enet_server PUBLIC ws2_32.lib winmm.lib)
else() # We are under UNIX
  target_link_options(enet_server PUBLIC -pthread)
endif()