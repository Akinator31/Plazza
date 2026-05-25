#pragma once
#include <memory>
#include "Thread/Thread.hpp"

class Kitchen;

class Cooker {
    Kitchen *_kitchen;
    std::unique_ptr<Thread> _thread;

public:
    Cooker(Kitchen *kitchen);

    void start();
    void run();
};
