cmake_minimum_required(VERSION 3.15)
cmake_policy(VERSION 3.15)

include(FetchContent)
project(client_server VERSION 1.0.0 LANGUAGES CXX)

set (BUILD_SHARED_LIBS FALSE)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.1
)

FetchContent_MakeAvailable(sfml)

set(CMAKE_CXX_STANDARD 17)

if(UNIX)
	add_compile_definitions(UNIX)
endif()

include_directories("Sources")

add_executable(tcp_clientMonoThread Sources/tcp_clientMonoThread.cpp)
target_link_libraries(tcp_clientMonoThread PUBLIC sfml-system sfml-network)

add_executable(tcp_clientMultiThread Sources/tcp_clientMultiThread.cpp)
target_link_libraries(tcp_clientMultiThread PUBLIC sfml-system sfml-network)

add_executable(tcp_server Sources/tcp_server.cpp)
target_link_libraries(tcp_server PUBLIC sfml-system sfml-network)

add_executable(udp_clientMonoThread Sources/udp_clientMonoThread.cpp)
target_link_libraries(udp_clientMonoThread PUBLIC sfml-system sfml-network)

add_executable(udp_server Sources/Client.cpp Sources/udp_server.cpp)
target_link_libraries(udp_server PUBLIC sfml-system sfml-network)
