#pragma once
#include <memory>
#include "Internal/Thread/Thread.hpp"

class Kitchen;

class Cooker {
    Kitchen *_kitchen;
    std::unique_ptr<Internal::Thread> _thread;

public:
    Cooker(Kitchen *kitchen);

    void start();
    void run();
};
