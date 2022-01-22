cmake_minimum_required(VERSION 2.4.3)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)
cmake_policy(SET CMP0017 NEW)
SET(MSVC_INCREMENTAL_DEFAULT ON)

PROJECT(shared_memory_example)

add_executable(shared_memory_client shared_memory_client.cpp win32_shared_memory.cpp win32_shared_memory.h)

add_executable(shared_memory_server shared_memory_server.cpp win32_shared_memory.cpp win32_shared_memory.h)