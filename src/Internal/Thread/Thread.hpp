/**
 * @file Thread.hpp
 * @brief Header file for the Thread class.
 */

#pragma once

#include <functional>
#include <pthread.h>

namespace Internal {
    /**
     * @class Thread
     * @brief A wrapper class for the POSIX thread.
     */
    class Thread {
        pthread_t _thread;
        std::function<void()> _fn;
        bool _started;
        bool _joined;

        /**
         * @brief Trampoline function to bridge between C pthread and C++ std::function.
         * 
         * @param arg Pointer to the Thread object.
         * @return void* Always returns nullptr.
         */
        static void *_trampoline(void *arg);

    public:
        /**
         * @brief Construct a new Thread object.
         * 
         * @param fn The function to be executed in the thread.
         */
        Thread(std::function<void()> fn);

        /**
         * @brief Destroy the Thread object.
         * 
         * If the thread was started but not joined, it will be detached or joined depending on implementation (usually join in destructor is safer if we want to ensure it finished).
         */
        ~Thread();

        Thread(const Thread &) = delete;
        Thread &operator=(const Thread &) = delete;

        /**
         * @brief Start the thread execution.
         * 
         * @throw std::runtime_error if the thread fails to start.
         */
        void start();

        /**
         * @brief Join the thread execution.
         * 
         * Blocks until the thread finishes its execution.
         */
        void join();
    };
}
