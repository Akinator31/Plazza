#include "Mutex.hpp"

#include <stdexcept>

Mutex::Mutex() {
    if (pthread_mutex_init(&_mutex, nullptr) != 0)
        throw std::runtime_error("Mutex: failed to initialize");
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&_mutex);
}

void Mutex::lock() {
    if (pthread_mutex_lock(&_mutex) != 0)
        throw std::runtime_error("Mutex: failed to lock");
}

void Mutex::unlock() {
    if (pthread_mutex_unlock(&_mutex) != 0)
        throw std::runtime_error("Mutex: failed to unlock");
}
