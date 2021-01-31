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

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("Include" "Include/Common" "Include/Common/Components" "Include/Common/Managers" "Include/Common/Modules" "Include/Common/Systems")

file(GLOB SOURCES CONFIGURE_DEPENDS
        Include/*.h
        Include/*.inl
        Include/Application/*.h
        Include/Application/States/*.h
        Include/Common/*.h
        Include/Common/GUI/*.h
        Include/Common/Components/*.h
        Include/Common/Managers/*.h
        Include/Common/Modules/*.h
        Include/Common/Network/*.h
        Include/Common/Prefabs/*.h
        Include/Common/Resources/*.h
        Include/Common/Resources/*.inl
        Include/Common/States/*.h
        Include/Common/Systems/*.h
        Include/Common/Utility/*.h
        Include/Common/Utility/*.inl
        Include/Client/*.h
        Include/Client/Managers/*.h
        Include/Client/Modules/*.h
        Include/Client/Systems/*.h
        Include/Client/States/*.h
        Include/Server/*.h
        Include/Server/Managers/*.h
        Include/Server/Modules/*.h
        Include/Server/Systems/*.h

        Source/*.cpp
        Source/Application/*.cpp
        Source/Application/States/*.cpp
        Source/Common/*.cpp
        Source/Common/GUI/*.cpp
        Source/Common/Components/*.cpp
        Source/Common/Managers/*.cpp
        Source/Common/Modules/*.cpp
        Source/Common/Network/*.cpp
        Source/Common/Prefabs/*.cpp
        Source/Common/Resources/*.cpp
        Source/Common/States/*.cpp
        Source/Common/Systems/*.cpp
        Source/Common/Utility/*.cpp
        Source/Client/*.cpp
        Source/Client/Managers/*.cpp
        Source/Client/Modules/*.cpp
        Source/Client/Systems/*.cpp
        Source/Client/States/*.cpp
        Source/Server/*.cpp
        Source/Server/Managers/*.cpp
        Source/Server/Modules/*.cpp
        Source/Server/Systems/*.cpp
        )

set(SOURCE_STANDALONE ${SOURCES})
set(SOURCE_FULL ${SOURCES})
set(SOURCE_TEST_CLIENT ${SOURCES})
list(REMOVE_ITEM SOURCE_STANDALONE ${CMAKE_CURRENT_SOURCE_DIR}/Source/Source.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Source/Client/ClientTestMain.cpp)
list(REMOVE_ITEM SOURCE_FULL ${CMAKE_CURRENT_SOURCE_DIR}/Source/Server/ServerMain.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Source/Client/ClientTestMain.cpp)
list(REMOVE_ITEM SOURCE_TEST_CLIENT  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Source.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Source/Server/ServerMain.cpp)

file(GLOB IMPLOT_SOURCES thirdparty/implot/*.h thirdparty/implot/*.cpp)

if (MSVC)
    # warning level 4 (see https://docs.microsoft.com/fr-fr/cpp/build/reference/compiler-option-warning-level?view=vs-2019)
    # We do not put /WX to consider all warnings as errors
    add_compile_options(/W4)
else()
    # lots of warnings 
    # We do not put -Werror to consider all warnings as errors
    add_compile_options(-g -Wall -Wextra)
endif()

add_subdirectory(thirdparty/imgui-sfml)
include_directories(thirdparty/implot)
add_executable(2M3
        ${SOURCE_FULL}
        ${IMPLOT_SOURCES}
)

add_executable(2M3-Server-Standalone
        ${SOURCE_STANDALONE}
        ${IMPLOT_SOURCES}
)

add_executable(2M3-Test-Client
        ${SOURCE_TEST_CLIENT}
        ${IMPLOT_SOURCES}
)

set(COMMON_LIBS
        sfml-system sfml-window sfml-graphics sfml-audio sfml-network imgui-sfml
        )

set(CLIENT_LIBS
        sfml-audio
        )

target_link_libraries(2M3 PUBLIC ${COMMON_LIBS} ${CLIENT_LIBS})
target_link_libraries(2M3-Server-Standalone PUBLIC ${COMMON_LIBS})
target_link_libraries(2M3-Test-Client PUBLIC ${COMMON_LIBS})


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

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        add_executable(2M3-WithTracy
                thirdparty/tracy/TracyClient.cpp
                ${SOURCE_FULL}
                ${IMPLOT_SOURCES}
        )
        target_include_directories(2M3-WithTracy PUBLIC thirdparty/tracy/)

        target_compile_definitions(2M3-WithTracy PRIVATE TRACY_ENABLE=1 TRACY_ON_DEMAND=1)
        target_link_libraries(2M3-WithTracy PUBLIC ${COMMON_LIBS} ${CLIENT_LIBS})
        add_dependencies(2M3-WithTracy copy-Media)
endif()