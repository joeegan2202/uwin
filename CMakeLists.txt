cmake_minimum_required(VERSION 3.25)
project(uwin)

set(CMAKE_CXX_STANDARD 17)

add_executable(uwin-server Framebuffer.cpp VideoDevice.cpp Image.cpp WindowServer.cpp Window.cpp main.cpp)

add_library(uwin WindowClient.cpp Framebuffer.cpp Window.cpp)
