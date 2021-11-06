cmake_minimum_required(VERSION 3.20)
project(OrderCache)

set(CMAKE_CXX_STANDARD 17)

add_executable(OrderCache main.cpp OrderBook.h OrderBook.cpp OrderCache.h OrderCache.cpp)
