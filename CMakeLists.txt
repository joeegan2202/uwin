cmake_minimum_required(VERSION 3.25)
project(uwin)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBEVDEV REQUIRED libevdev)

add_executable(uwin-server Geometry.cpp Launcher.cpp LauncherHelper.cpp Font.cpp Panel.cpp EventManager.cpp Mouse.cpp Framebuffer.cpp VideoDevice.cpp Image.cpp WindowServer.cpp Window.cpp main.cpp)
target_include_directories(uwin-server PRIVATE ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(uwin-server PRIVATE ${LIBEVDEV_LIBRARIES})

add_library(uwin WindowClient.cpp Framebuffer.cpp Window.cpp)
target_include_directories(uwin PRIVATE ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(uwin PRIVATE ${LIBEVDEV_LIBRARIES})

add_executable(dumb-client Framebuffer.cpp WindowClient.cpp Image.cpp test/test.cpp)
target_include_directories(dumb-client PRIVATE ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(dumb-client PRIVATE ${LIBEVDEV_LIBRARIES})
