cmake_minimum_required(VERSION 3.10)
project(WebServer)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(webserver src/main.cpp)

