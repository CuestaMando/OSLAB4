cmake_minimum_required(VERSION 3.15)
project(untitled2 C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled2 main.c)
add_executable(untitled2_memory memory_mgmt.c memory_mgmt.h)

add_executable(untitled2_process process_mgmt.c process_mgmt.h)

add_executable(untitled2_queue queue.c queue.h)
add_executable(untitled2_util util.c util.h)
