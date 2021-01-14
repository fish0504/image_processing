cmake_minimum_required(VERSION 3.0)
project(trafic-light-detection)

# Check C++11
include(CheckCXXCompilerFlag)
enable_language(CXX)
check_cxx_compiler_flag("-std=gnu++11" COMPILER_SUPPORTS_CXX11)
if(NOT ${COMPILER_SUPPORTS_CXX11})
  message(FATAL_ERROR "${CMAKE_CXX_COMPILER} doesn't support C++11\n")
endif()
set(CMAKE_CXX_STANDARD 11) # C++17
message("Compiler:\n\t${CMAKE_CXX_COMPILER} (using C++11)")

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RELEASE")
endif()
string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
message("Build Type:\n\t${CMAKE_BUILD_TYPE}")


# clear CMAKE_CXX_FLAGS
set(CMAKE_CXX_FLAGS "")

# CMAKE_CXX_FLAGS
message("CMAKE_CXX_FLAGS:\n\t${CMAKE_CXX_FLAGS}")

# Set warning flags
set(CXX_WARNING_FLAGS
    -Wall
    -Wextra
    -Wconversion
    -Wswitch-default
    -Wdisabled-optimization
    -Wformat
    -Winit-self
    -Woverloaded-virtual
    -Wfloat-equal
    -Wno-old-style-cast
    -Wno-pragmas)
foreach(FLAG IN LISTS CXX_WARNING_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}")
endforeach()

# OpenCV
$OpenCV_DIR="/root/Workspace/opencv"
find_package(OpenCV REQUIRED)
message("OpenCV version:\n\t${OpenCV_VERSION}")

# Boost
find_package(Boost REQUIRED QUIET)
message("Boost version:\n\t${Boost_VERSION}")

# Eigen3
find_package(Eigen3 REQUIRED)
message("Eigen3 version:\n\t${EIGEN3_VERSION_STRING}")
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

# header & source
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
file(GLOB SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**/*.cpp)

# =======================================
add_library(TLD ${SOURCES})
target_link_libraries(TLD ${OpenCV_LIBS} pthread)

# main
add_executable(main main.cpp)
target_link_libraries(main ${OpenCV_LIBS} TLD pthread)
