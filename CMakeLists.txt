cmake_minimum_required(VERSION 3.24)
project(DbLab1)

set(CMAKE_CXX_STANDARD 20)

add_executable(DbLab1 main.cpp DataBase.cpp DataBase.h Publications.h Reviews.h GarbageCollector.h)
