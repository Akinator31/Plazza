/**
 * @file Process.hpp
 * @brief Header file for the Process class.
 */

#pragma once
#include <sched.h>
#include <sys/types.h>

namespace Internal {
    /**
     * @class Process
     * @brief A wrapper class for the fork() system call and process management.
     */
    class Process {
        pid_t _pid;

    public:
        /**
         * @brief Construct a new Process object by forking the current process.
         */
        explicit Process();

        /**
         * @brief Check if the current instance is the child process.
         * 
         * @return true if this is the child process.
         * @return false if this is the parent process.
         */
        [[nodiscard]] bool isChild() const;

        /**
         * @brief Check if the current instance is the parent process.
         * 
         * @return true if this is the parent process.
         * @return false if this is the child process.
         */
        [[nodiscard]] bool isParent() const;

        /**
         * @brief Get the process ID.
         * 
         * @return pid_t The process ID.
         */
        [[nodiscard]] pid_t pid() const;

        /**
         * @brief Terminate the process.
         */
        void kill() const;

        /**
         * @brief Wait for the process to terminate.
         */
        void wait() const;
    };
}