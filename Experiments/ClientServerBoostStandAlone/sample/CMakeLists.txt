add_executable(blocking_tcp_echo_client
  blocking_tcp_echo_client.cpp common.h
)
if (UNIX)
  target_link_options(blocking_tcp_echo_client PUBLIC -pthread)
endif()
add_executable(blocking_tcp_echo_server
  blocking_tcp_echo_server.cpp common.h
)
if (UNIX)
  target_link_options(blocking_tcp_echo_server PUBLIC -pthread)
endif()

add_executable(blocking_udp_echo_client
  blocking_udp_echo_client.cpp common.h
)
if (UNIX)
  target_link_options(blocking_udp_echo_client PUBLIC -pthread)
endif()
add_executable(blocking_udp_echo_server
  blocking_udp_echo_server.cpp common.h
)
if (UNIX)
  target_link_options(blocking_udp_echo_server PUBLIC -pthread)
endif()
