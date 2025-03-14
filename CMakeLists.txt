# CMake minimum version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(AuthNinja)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Path to the local Crypto++ headers
set(CRYPTOPP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/cryptopp")

# Add Crypto++ headers to the include path
include_directories(${CRYPTOPP_INCLUDE_DIR})

# Add executable
add_executable(AuthNinja main.cpp)
