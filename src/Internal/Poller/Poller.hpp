/**
 * @file Poller.hpp
 * @brief Header file for the Poller class.
 */

#pragma once
#include <map>
#include <vector>
#include <sys/poll.h>

namespace Internal {
    /**
     * @class Poller
     * @brief A wrapper class for the poll() system call.
     */
    class Poller {
        std::vector<pollfd> _pfds;
        std::map<int, size_t> _fdIndex;

        /**
         * @brief Get the revents of a file descriptor.
         * 
         * @param fd The file descriptor to check.
         * @return short The revents field from the pollfd struct.
         */
        short _reventOf(int fd);

    public:
        /**
         * @brief Construct a new Poller object.
         */
        Poller() = default;

        /**
         * @brief Add a file descriptor to the poller.
         * 
         * @param fd The file descriptor to add.
         * @param events The events to poll for (e.g., POLLIN, POLLOUT).
         */
        void add(int fd, short events);

        /**
         * @brief Remove a file descriptor from the poller.
         * 
         * @param fd The file descriptor to remove.
         */
        void remove(int fd);

        /**
         * @brief Wait for events on the added file descriptors.
         * 
         * @param timeout The timeout in milliseconds. Defaults to -1 (infinite).
         * @return int The number of file descriptors with events, or -1 on error.
         */
        int wait(int timeout = -1);

        /**
         * @brief Check if a file descriptor is readable.
         * 
         * @param fd The file descriptor to check.
         * @return true if the file descriptor is readable.
         * @return false otherwise.
         */
        [[nodiscard]] bool isReadable(int fd);

        /**
         * @brief Check if a file descriptor is writable.
         * 
         * @param fd The file descriptor to check.
         * @return true if the file descriptor is writable.
         * @return false otherwise.
         */
        [[nodiscard]] bool isWritable(int fd);

        /**
         * @brief Check if a file descriptor is invalid.
         * 
         * @param fd The file descriptor to check.
         * @return true if the file descriptor is invalid.
         * @return false otherwise.
         */
        [[nodiscard]] bool isInvalid(int fd);

        /**
         * @brief Check if a file descriptor has an error.
         * 
         * @param fd The file descriptor to check.
         * @return true if the file descriptor has an error.
         * @return false otherwise.
         */
        [[nodiscard]] bool hasError(int fd);

        /**
         * @brief Check if a file descriptor has a hangup.
         * 
         * @param fd The file descriptor to check.
         * @return true if the file descriptor has a hangup.
         * @return false otherwise.
         */
        [[nodiscard]] bool hasHangup(int fd);
    };
}