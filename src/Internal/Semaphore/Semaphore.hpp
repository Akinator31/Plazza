/**
 * @file Semaphore.hpp
 * @brief Header file for the Semaphore class.
 */

#pragma once

#include <semaphore.h>

namespace Internal {
    /**
     * @class Semaphore
     * @brief A wrapper class for the POSIX semaphore.
     */
    class Semaphore {
        sem_t _semaphore;

    public:
        /**
         * @brief Construct a new Semaphore object.
         * 
         * @param value Initial value of the semaphore. Defaults to 0.
         */
        explicit Semaphore(unsigned int value = 0);

        /**
         * @brief Destroy the Semaphore object.
         */
        ~Semaphore();

        Semaphore(const Semaphore &) = delete;
        Semaphore &operator=(const Semaphore &) = delete;

        /**
         * @brief Wait for the semaphore (decrement).
         * 
         * This operation will block if the semaphore value is 0.
         */
        void wait();

        /**
         * @brief Post the semaphore (increment).
         */
        void post();
    };
}
