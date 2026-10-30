cmake_minimum_required(VERSION 3.16)

project(autoscaler_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(autoscaler-server
  src/main.cpp
  src/Socket.cpp
  src/Processor.cpp
)
