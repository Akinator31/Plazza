#pragma once

#include <functional>
#include <pthread.h>

namespace Internal {
    class Thread {
        pthread_t _thread;
        std::function<void()> _fn;
        bool _started;
        bool _joined;

        static void *_trampoline(void *arg);

    public:
        Thread(std::function<void()> fn);
        ~Thread();

        Thread(const Thread &) = delete;
        Thread &operator=(const Thread &) = delete;

        void start();
        void join();
    };
}
