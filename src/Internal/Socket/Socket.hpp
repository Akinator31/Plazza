/**
 * @file Socket.hpp
 * @brief Header file for the Socket class.
 */

#pragma once
#include <optional>
#include <string>
#include <netinet/in.h>
#include <sys/un.h>

/**
 * @brief Prefix for the Unix socket path.
 */
#define SOCK_PATH_PREFIX "/tmp/plazza_socket_"

namespace Internal {
    /**
     * @class Socket
     * @brief A wrapper around POSIX sockets that handles the lifecycle automatically (RAII).
     *
     * This class is responsible for creating, binding, listening, and accepting connections
     * on a Unix socket. It also handles reading and writing data through the socket.
     * The socket is automatically closed when the object is destroyed.
     *
     * @note Copy is disabled, only move is allowed.
     */
    class Socket {
        int _fd; ///< The file descriptor of the socket (-1 if closed/invalid).
        sockaddr_un _socketConfig{}; ///< The socket address configuration (Unix domain).

    public:
        /**
         * @brief Creates a new Unix socket.
         * @throw PlazzaException if the socket creation failed.
         */
        Socket();

        /**
         * @brief Wraps an existing file descriptor into a Socket object.
         * @param socket The file descriptor of the socket you want to wrap.
         */
        explicit Socket(int socket);

        /**
         * @brief Destroys the socket and closes the file descriptor if it's still open.
         */
        ~Socket();

        /**
         * @brief Move constructor. Takes the ownership of the other socket.
         * @param other The socket you're moving from. It will be invalidated after this.
         */
        Socket(Socket&& other) noexcept;

        /**
         * @brief Move assignment operator. Closes the current socket and takes the other one.
         * @param other The socket you're moving from.
         * @return A reference to this socket.
         */
        Socket& operator=(Socket&& other) noexcept;

        /**
         * @brief Binds the socket to a specific path.
         * @param socketConfigOpt The socket config if you want to use your own.
         * @throw PlazzaException if the bind failed.
         */
        void bind(const std::optional<sockaddr_un>& socketConfigOpt = std::nullopt);

        /**
         * @brief Puts the socket in a listening state so it can accept incoming connections.
         * @throw PlazzaException if listen failed.
         */
        void listen() const;

        /**
         * @brief Establishes a connection to a remote socket path.
         * @param socket_path The path to the Unix socket.
         * @return 0 on success, -1 on failure.
         */
        int connect(const std::string& socket_path) const;

        /**
         * @brief Accepts a new incoming connection and returns a new Socket for it.
         * @return A new Socket object that represents the client connection.
         * @throw PlazzaException if accept failed.
         */
        [[nodiscard]] Socket accept() const;

        /**
         * @brief Reads data from the socket into a buffer.
         * @param buffer Where the read data will be stored.
         * @param size The maximum number of bytes to read.
         * @return The number of bytes read, 0 if the connection was closed, or -1 on error.
         */
        [[nodiscard]] ssize_t read(void* buffer, size_t size) const;

        /**
         * @brief Writes data from a buffer to the socket.
         * @param buffer The data you want to send.
         * @param size The number of bytes to write.
         * @return The number of bytes written, or -1 on error.
         */
        ssize_t write(const char* buffer, size_t size) const;

        /**
         * @brief Closes the socket if it was not already closed.
         */
        void close();

        /**
         * @brief Gets the file descriptor of the socket.
         * @return The file descriptor, or -1 if the socket is closed.
         */
        [[nodiscard]] int fd() const;

        /**
         * @brief Gets the sockaddr_un struct of the socket.
         * @return The sockaddr_un.
         */
        [[nodiscard]] sockaddr_un& getSun();

        /**
         * @brief Gets the path of the Unix socket.
         * @return The path of the socket.
         */
        [[nodiscard]] std::string getPath() const;
    };
}