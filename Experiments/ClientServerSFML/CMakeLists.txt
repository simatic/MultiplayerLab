cmake_minimum_required(VERSION 3.26)

include(FetchContent)
project(client_server VERSION 1.0.0 LANGUAGES CXX)

if(WIN32)
  # SonarLint support for CMake requires the following flag to be set on.
  #(see https://community.sonarsource.com/t/sonarlint-for-visual-studio-v4-38-support-cmake-projects/50127)
  set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

set (BUILD_SHARED_LIBS FALSE)

FetchContent_Declare( sfml
  URL https://github.com/SFML/SFML/archive/refs/tags/3.0.1.tar.gz
)
option(SFML_BUILD_AUDIO "Build audio" OFF)
FetchContent_MakeAvailable(sfml)

set(CMAKE_CXX_STANDARD 23)

if(UNIX)
  add_compile_definitions(UNIX)
endif()

include_directories("src")

add_executable(tcp_clientMonoThread src/tcp_clientMonoThread.cpp)
target_link_libraries(tcp_clientMonoThread PUBLIC sfml-system sfml-network)

add_executable(tcp_clientMultiThread src/tcp_clientMultiThread.cpp)
target_link_libraries(tcp_clientMultiThread PUBLIC sfml-system sfml-network)

add_executable(tcp_serverMonoThread src/tcp_serverMonoThread.cpp)
target_link_libraries(tcp_serverMonoThread PUBLIC sfml-system sfml-network)

add_executable(tcp_serverMultiThread src/tcp_serverMultiThread.cpp)
target_link_libraries(tcp_serverMultiThread PUBLIC sfml-system sfml-network)

add_executable(udp_clientMonoThread src/udp_clientMonoThread.cpp)
target_link_libraries(udp_clientMonoThread PUBLIC sfml-system sfml-network)

add_executable(udp_clientMultiThread src/udp_clientMultiThread.cpp)
target_link_libraries(udp_clientMultiThread PUBLIC sfml-system sfml-network)

add_executable(udp_server src/Client.cpp src/udp_server.cpp)
target_link_libraries(udp_server PUBLIC sfml-system sfml-network)
