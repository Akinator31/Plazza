cmake_minimum_required(VERSION 3.20)
project(plazza LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(plazza
        src/main.cpp
        src/Reception/Reception.cpp
        src/Reception/Commands.cpp
        src/Errors/PlazzaErrors.cpp
        src/Kitchen/Kitchen.cpp
        src/Kitchen/Cooker/Cooker.cpp
        src/Internal/Process/Process.cpp
        src/Internal/Poller/Poller.cpp
        src/Internal/Socket/Socket.cpp
        src/Utils/Utils.cpp
        src/Internal/IPC/IPC.cpp
        src/Internal/Thread/Thread.cpp
        src/Internal/Mutex/Mutex.cpp
        src/Internal/Semaphore/Semaphore.cpp
        include/Pizzas.hpp)

target_include_directories(plazza PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)