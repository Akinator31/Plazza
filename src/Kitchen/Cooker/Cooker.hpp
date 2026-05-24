#pragma once
#include "Thread/Thread.hpp"
#include "Semaphore/Semaphore.hpp"
#include "Mutex/Mutex.hpp"

class Kitchen;

class Cooker {
    Thread _thread;
    Kitchen *_kitchen;
public:
    Cooker();
    ~Cooker();
    static void *run(void *arg);
};
