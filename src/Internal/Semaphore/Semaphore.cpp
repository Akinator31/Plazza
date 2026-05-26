#include "Semaphore.hpp"

#include <stdexcept>

namespace Internal {
    Semaphore::Semaphore(unsigned int value) {
        if (sem_init(&_semaphore, 0, value) != 0)
            throw std::runtime_error("Semaphore: failed to initialize");
    }

    Semaphore::~Semaphore() {
        sem_destroy(&_semaphore);
    }

    void Semaphore::wait() {
        if (sem_wait(&_semaphore) != 0)
            throw std::runtime_error("Semaphore: wait failed");
    }

    void Semaphore::post() {
        if (sem_post(&_semaphore) != 0)
            throw std::runtime_error("Semaphore: post failed");
    }
}
