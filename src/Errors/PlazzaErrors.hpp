//
// Created by pavel on 06/05/2026.
//

#pragma once
#include <exception>
#include <map>
#include <string>

enum PlazzaErrorTypes {
    IncorrectArgs,
    ForkError,
    PollError,
    SocketCreationError,
    BindError,
    ListenError,
    AcceptError,
};

class PlazzaException : public std::exception {
    std::map<PlazzaErrorTypes, std::string> _error_map {
        {IncorrectArgs, "Incorrect arguments!"},
        {ForkError, "Fail to fork!"},
        {PollError, "An error occurred while polling reception socket!"},
        {SocketCreationError, "An error occurred while creating a socket!"},
        {BindError, "An error occurred while binding the reception socket!"},
        {ListenError, "An error occurred while listening the reception socket!"},
        {AcceptError, "An error occurred while accepting connection on the reception socket!"}
    };

    std::string _error;

public:
    explicit PlazzaException(PlazzaErrorTypes type);
    [[nodiscard]] const char* what() const noexcept override;
};