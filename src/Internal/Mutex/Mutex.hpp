/**
 * @file Mutex.hpp
 * @brief Header file for the Mutex class.
 */

#pragma once

#include <pthread.h>

namespace Internal {
    /**
     * @class Mutex
     * @brief A wrapper class for the POSIX mutex.
     */
    class Mutex {
        pthread_mutex_t _mutex;

    public:
        /**
         * @brief Construct a new Mutex object.
         */
        Mutex();

        /**
         * @brief Destroy the Mutex object.
         */
        ~Mutex();

        Mutex(const Mutex &) = delete;
        Mutex &operator=(const Mutex &) = delete;

        /**
         * @brief Lock the mutex.
         * 
         * Blocks if the mutex is already locked by another thread.
         */
        void lock();

        /**
         * @brief Unlock the mutex.
         */
        void unlock();
    };
}
