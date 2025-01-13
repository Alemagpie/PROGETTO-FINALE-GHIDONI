#Minimum CMake version required
cmake_minimum_required(VERSION 3.1)

#Project name
project(ProgettoFinale)

#C++ minimum version require
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_RELEASE "-O2")

#Include directory
include_directories(include)

#Source files
set(SOURCES
 src/main.cpp
 src/AutomaticDevice.cpp
 src/ManualDevice.cpp
 src/Device.cpp 
 src/DeviceManager.cpp
 src/CustomTime.cpp
 src/Utility.cpp
 src/OutputManager.hpp
)

# Add the executable target
add_executable(ProgettoFinale ${SOURCES})