cmake_minimum_required(VERSION 3.20)
project(plazza LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(plazza src/main.cpp
        src/Reception/Reception.hpp
        src/Reception/Reception.cpp
        src/Errors/PlazzaErrors.hpp
        src/Errors/PlazzaErrors.cpp)