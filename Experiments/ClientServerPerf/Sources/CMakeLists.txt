add_executable(tcp_client
  tcp_client.cpp common.h tcp_communication.cpp tcp_communication.h options.cpp options.h OptParserExtended.cpp OptParserExtended.h
)
if (UNIX)
  target_link_options(tcp_client PUBLIC -pthread)
endif()
add_executable(tcp_server
  tcp_server.cpp common.h tcp_communication.cpp tcp_communication.h options.cpp options.h OptParserExtended.cpp OptParserExtended.h
)
if (UNIX)
  target_link_options(tcp_server PUBLIC -pthread)
endif()

add_executable(udp_client
  udp_client.cpp common.h
)
if (UNIX)
  target_link_options(udp_client PUBLIC -pthread)
endif()
add_executable(udp_server
  udp_server.cpp common.h
)
if (UNIX)
  target_link_options(udp_server PUBLIC -pthread)
endif()
