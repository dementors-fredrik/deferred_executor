cmake_minimum_required(VERSION 3.21)
project(deferred_executor)

set(CMAKE_CXX_STANDARD 14)

add_executable(deferred_executor main.cpp DeferredExecutor.cpp DeferredExecutor.h)
