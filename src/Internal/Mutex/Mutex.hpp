#pragma once

#include <pthread.h>

namespace Internal {
    class Mutex {
        pthread_mutex_t _mutex;

    public:
        Mutex();
        ~Mutex();

        Mutex(const Mutex &) = delete;
        Mutex &operator=(const Mutex &) = delete;

        void lock();
        void unlock();
    };
}
