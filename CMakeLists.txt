# cmake最低版本号要求
cmake_minimum_required(VERSION 3.10.2)

# 项目名
project(TinyWebServer)

set(CMAKE_CXX_COMPILER "/usr/local/g++")

# 指定生成目标
add_executable(server.exe ./Code/main.cpp)