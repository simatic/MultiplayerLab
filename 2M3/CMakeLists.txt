cmake_minimum_required(VERSION 3.15)
cmake_policy(VERSION 3.15)

include(FetchContent)
project(2M3 VERSION 1.0.0 LANGUAGES CXX)

set (BUILD_SHARED_LIBS FALSE)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.1
)

FetchContent_MakeAvailable(sfml)

include_directories("Include")
file(GLOB SOURCES CONFIGURE_DEPENDS Include/*.h Include/*.inl Source/*.cpp)

if (MSVC)
    # warning level 4 (see https://docs.microsoft.com/fr-fr/cpp/build/reference/compiler-option-warning-level?view=vs-2019)
    # We do not put /WX to consider all warnings as errors
    add_compile_options(/W4)
else()
    # lots of warnings 
    # We do not put -Werror to consider all warnings as errors
    add_compile_options(-Wall -Wextra)
endif()

add_executable(2M3
  ${SOURCES}
   "Include/Client.h" "Source/Client.cpp")

target_link_libraries(2M3 PUBLIC sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

add_custom_target(copy-Media ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Media)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Media
                   DEPENDS ${CMAKE_SOURCE_DIR}/Media
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                           ${CMAKE_SOURCE_DIR}/Media
                           ${CMAKE_CURRENT_BINARY_DIR}/Media
                           )
add_dependencies(2M3 copy-Media)

file(GLOB_RECURSE DYNAMIC_LIBS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/lib/${ARCH}/*.dll)
file(COPY ${DYNAMIC_LIBS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
