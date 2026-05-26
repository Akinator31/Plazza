#pragma once

#include <semaphore.h>

namespace Internal {
    class Semaphore {
        sem_t _semaphore;

    public:
        explicit Semaphore(unsigned int value = 0);
        ~Semaphore();

        Semaphore(const Semaphore &) = delete;
        Semaphore &operator=(const Semaphore &) = delete;

        void wait();
        void post();
    };
}
