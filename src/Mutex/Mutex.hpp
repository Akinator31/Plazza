#pragma once

#include <pthread.h>

class Mutex {
    pthread_mutex_t _mutex;

public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();
};
