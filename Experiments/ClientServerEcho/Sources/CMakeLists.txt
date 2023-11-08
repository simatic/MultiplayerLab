#
# UDP perf applications
#
add_executable(udp_client
  udp_client.cpp common.h udp_communication.cpp udp_communication.h options.cpp options.h OptParserExtended.cpp OptParserExtended.h
        Measures.cpp Measures.h)
if (WIN32)
  target_link_libraries(udp_client PUBLIC ws2_32.lib winmm.lib)
else() # We are under UNIX
  target_link_options(udp_client PUBLIC -pthread)
endif()
add_executable(udp_server
  udp_server.cpp common.h udp_communication.cpp udp_communication.h options.cpp options.h OptParserExtended.cpp OptParserExtended.h
        Measures.cpp Measures.h)
if (WIN32)
  target_link_libraries(udp_server PUBLIC ws2_32.lib winmm.lib)
else() # We are under UNIX
  target_link_options(udp_server PUBLIC -pthread)
endif()
