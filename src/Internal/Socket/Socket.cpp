//
// Created by pavel on 12/05/2026.
//

#include "Socket.hpp"

#include <ostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "Errors/PlazzaErrors.hpp"

namespace Internal {
    Socket::Socket() {
        this->_fd = socket(AF_UNIX, SOCK_STREAM, 0);

        if (this->_fd == -1)
            throw PlazzaException(SocketCreationError);
    }

    Socket::Socket(const int socket) {
        this->_fd = socket;
    }

    Socket::~Socket() {
        close();
    }

    Socket::Socket(Socket&& other) noexcept : _fd(other._fd), _socketConfig(other._socketConfig) {
        other._fd = -1;
    }

    Socket& Socket::operator=(Socket&& other) noexcept {
        if (this != &other) {
            this->close();
            this->_fd = other._fd;
            this->_socketConfig = other._socketConfig;
            other._fd = -1;
        }
        return *this;
    }

    void Socket::close() {
        if (this->fd() != -1) {
            ::close(this->_fd);
            this->_fd = -1;
        }
    }

    void Socket::bind(const std::optional<sockaddr_un>& socketConfigOpt) {
        sockaddr_un socketConfig = {};

        if (socketConfigOpt.has_value()) {
            socketConfig = socketConfigOpt.value();
        } else {
            const std::string sun_path = SOCK_PATH_PREFIX + std::to_string(this->_fd);

            socketConfig.sun_family = AF_UNIX;
            strcpy(socketConfig.sun_path, sun_path.c_str());
        }

        const auto* castSocketConfig = reinterpret_cast<sockaddr*>(&socketConfig);
        constexpr socklen_t castSocketConfigSize = sizeof(socketConfig);

        if (::bind(this->_fd, castSocketConfig, castSocketConfigSize) == -1)
            throw PlazzaException(BindError);

        this->_socketConfig = socketConfig;
    }

    void Socket::listen() const {
        if (::listen(this->_fd, SOMAXCONN) == -1) {
            throw PlazzaException(ListenError);
        }
    }

    int Socket::connect(const std::string& socket_path) const {
        sockaddr_un socketConfig{};

        socketConfig.sun_family = AF_UNIX;
        strcpy(socketConfig.sun_path, socket_path.c_str());

        return ::connect(this->_fd, reinterpret_cast<sockaddr*>(&socketConfig), sizeof(socketConfig));
    }

    Socket Socket::accept() const {
        sockaddr_un newSocketConfig{};
        socklen_t newSocketConfigSize = sizeof(newSocketConfig);
        const int newSocket = ::accept(this->_fd, reinterpret_cast<sockaddr*>(&newSocketConfig), &newSocketConfigSize);

        if (newSocket == -1)
            throw PlazzaException(AcceptError);

        Socket newS(newSocket);
        newS._socketConfig = newSocketConfig;

        return newS;
    }

    ssize_t Socket::read(void* buffer, const size_t size) const {
        return ::read(this->fd(), buffer, size);
    }

    ssize_t Socket::write(const char* buffer, const size_t size) const {
        return ::write(this->fd(), buffer, size);
    }

    int Socket::fd() const {
        return this->_fd;
    }

    sockaddr_un& Socket::getSun() {
        return this->_socketConfig;
    }

    std::string Socket::getPath() const {
        return this->_socketConfig.sun_path;
    }
}