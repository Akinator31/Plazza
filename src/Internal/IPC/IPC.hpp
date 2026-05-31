/**
 * @file IPC.hpp
 * @brief Header file for the IPC class.
 */

#pragma once
#include "Internal/Socket/Socket.hpp"
#include "Pizzas.hpp"
#include "Internal/Poller/Poller.hpp"
#include "Errors/PlazzaErrors.hpp"

/**
 * @enum IPCStatus
 * @brief Represents the status of an IPC operation.
 */
enum IPCStatus {
    TIMEOUT, ///< The operation timed out.
    OK       ///< The operation succeeded.
};

namespace Internal {
    /**
     * @class IPC
     * @brief A class for Inter-Process Communication using Unix sockets.
     */
    class IPC {
        Socket _socket;
        Poller _poller;

    public:
        /**
         * @brief Construct a new IPC object from an accepted socket.
         * 
         * @param accepted The socket to wrap.
         */
        explicit IPC(Socket &&accepted);

        /**
         * @brief Construct a new IPC object and connect to a socket path.
         * 
         * @param socketPath The path to the Unix socket.
         */
        explicit IPC(const std::string &socketPath);

        /**
         * @brief Get the file descriptor of the underlying socket.
         * 
         * @return int The file descriptor.
         */
        [[nodiscard]] int fd() const;

        /**
         * @brief Send data through the IPC.
         * 
         * @tparam T The type of data to send.
         * @param data The data to send.
         * @return IPC& A reference to this IPC object.
         */
        template<typename T>
        IPC& operator<<(const T& data) {
            this->_socket.write(reinterpret_cast<const char*>(&data), sizeof(data));
            return *this;
        }

        /**
         * @brief Receive data through the IPC.
         * 
         * @tparam T The type of data to receive.
         * @param data The variable where the received data will be stored.
         * @return IPC& A reference to this IPC object.
         * @throw PlazzaException if the read fails.
         */
        template<typename T>
        IPC& operator>>(T& data) {
            ssize_t total = 0;
            const ssize_t expected = static_cast<ssize_t>(sizeof(data));
            auto buf = reinterpret_cast<char*>(&data);
            while (total < expected) {
                const ssize_t n = this->_socket.read(buf + total, expected - total);
                if (n <= 0)
                    throw PlazzaException(IPCReadError);
                total += n;
            }
            return *this;
        }

        /**
         * @brief Wait for data to be available for reading.
         * 
         * @param timeout The timeout in milliseconds.
         * @return IPCStatus The status of the wait operation.
         */
        IPCStatus wait(int timeout);
    };
}