cmake_minimum_required(VERSION 3.1.0)

project(qt-pong-server)
set(qt-pong-server VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Network REQUIRED)

add_executable(qt-pong-server
    src/main.cpp
    src/lobby_server.cpp
    src/pong_server.cpp
    include/lobby_server.h
    include/pong_server.h
)

target_link_libraries(qt-pong-server
    PUBLIC
    Qt::Core
    Qt::Network
)
