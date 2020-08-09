# cmake 最低版本号要求
cmake_minimum_required(VERSION 3.10.2)

# 项目信息
project(unit_test)

set(LIB_DIR ${PROJECT_SOURCE_DIR}/../Code)
set(
    SRC_LIST
    ${LIB_DIR}/locker/locker.h
    ${LIB_DIR}/threadpool/threadpool.h
    ${LIB_DIR}/threadpool/threadpool.cpp
    ${LIB_DIR}/threadpool/taskQueue.h
    ${LIB_DIR}/threadpool/taskQueue.cpp
    ${LIB_DIR}/config.h
    main.cpp
)

INCLUDE_DIRECTORIES(
    ${LIB_DIR}/locker
    ${LIB_DIR}/threadpool
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