#include "Thread.hpp"

#include <stdexcept>

Thread::Thread(std::function<void()> fn)
    : _fn(std::move(fn)), _started(false), _joined(false) {
}

Thread::~Thread() {
    if (_started && !_joined)
        pthread_detach(_thread);
}

void Thread::start() {
    if (_started)
        throw std::runtime_error("Thread: already started");
    if (pthread_create(&_thread, nullptr, _trampoline, this) != 0)
        throw std::runtime_error("Thread: failed to create thread");
    _started = true;
}

void Thread::join() {
    if (!_started)
        throw std::runtime_error("Thread: not started");
    if (_joined)
        throw std::runtime_error("Thread: already joined");
    if (pthread_join(_thread, nullptr) != 0)
        throw std::runtime_error("Thread: join failed");
    _joined = true;
}
