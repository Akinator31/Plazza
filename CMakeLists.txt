cmake_minimum_required(VERSION 3.20)
project(plazza LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(src include)

add_executable(plazza src/main.cpp
        src/Reception/Reception.hpp
        src/Reception/Reception.cpp
        src/Errors/PlazzaErrors.hpp
        src/Errors/PlazzaErrors.cpp
        src/Kitchen/Kitchen.hpp
        src/Kitchen/Kitchen.cpp
        src/Internal/Process/Process.cpp
        src/Internal/Process/Process.hpp
        src/Internal/Poller/Poller.cpp
        src/Internal/Poller/Poller.hpp
        src/Internal/Socket/Socket.cpp
        src/Internal/Socket/Socket.hpp
        src/Reception/Commands.cpp
        src/Reception/Commands.hpp
        src/Utils/Utils.cpp
        src/Utils/Utils.hpp
        src/Internal/IPC/IPC.cpp
        src/Internal/IPC/IPC.hpp
        include/Pizzas.hpp)