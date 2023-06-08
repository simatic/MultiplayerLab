cmake_minimum_required(VERSION 3.15)
cmake_policy(VERSION 3.15)

include(FetchContent)
project(SampleBoost VERSION 1.0.0 LANGUAGES CXX)

if(WIN32)
  # SonarLint support for CMake requires the following flag to be set on.
  #(see https://community.sonarsource.com/t/sonarlint-for-visual-studio-v4-38-support-cmake-projects/50127)
  set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

set (BUILD_SHARED_LIBS FALSE)

# We include the subset of Boost we will be using.
include_directories(boost_1_82_0_subset)
if(WIN32)
    # Compilation of some of the boost sources requires the following definition
    # (which is appropriate for Windows 7 target)
    add_compile_definitions(_WIN32_WINNT=0x0601)
endif()
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(sample)

