# cmake 最低版本号要求
cmake_minimum_required(VERSION 3.10.2)

# 项目信息
project(TinyWebServer)

set(LIB_DIR ${PROJECT_SOURCE_DIR}/Code)
set(
    SRC_LIST
    ${LIB_DIR}/locker/locker.h
    ${LIB_DIR}/threadpool/threadpool.h
    ${LIB_DIR}/threadpool/taskQueue.h
    ${LIB_DIR}/toys/tcpsocket.h
    ${LIB_DIR}/toys/tcpsocket.cpp
    ${LIB_DIR}/toys/etEpoll.h
    ${LIB_DIR}/toys/etEpoll.cpp
    ${LIB_DIR}/toys/buffer.h
    ${LIB_DIR}/toys/buffer.cpp
    ${LIB_DIR}/webserver/webserver.h
    ${LIB_DIR}/webserver/webserver.cpp
    ${LIB_DIR}/http/httpcon.h
    ${LIB_DIR}/http/httpcon.cpp
    ${LIB_DIR}/http/httprequest.h
    ${LIB_DIR}/http/httprequest.cpp
    ${LIB_DIR}/http/httpresponse.h
    ${LIB_DIR}/http/httpresponse.cpp
    ${LIB_DIR}/config.h
    ${LIB_DIR}/main.cpp
)

INCLUDE_DIRECTORIES(
    ${LIB_DIR}/locker
    ${LIB_DIR}/threadpool
    ${LIB_DIR}/toys
    ${LIB_DIR}/webserver
    ${LIB_DIR}/http
    ${LIB_DIR}
)

find_package(Threads)
add_definitions("-Wall -g")

# 指定生成目标 
add_executable(
    ${PROJECT_NAME}
    ${SRC_LIST}
)

target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})