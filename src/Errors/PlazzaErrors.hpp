/**
 * @file PlazzaErrors.hpp
 * @brief Header file for error handling in the Plazza project.
 */

#pragma once
#include <exception>
#include <map>
#include <string>

/**
 * @enum PlazzaErrorTypes
 * @brief Enum representing different types of errors in the Plazza project.
 */
enum PlazzaErrorTypes {
    IncorrectArgs,      ///< Incorrect command line arguments.
    ForkError,          ///< Failure to fork a new process.
    PollError,          ///< Failure during polling.
    SocketCreationError, ///< Failure to create a socket.
    BindError,          ///< Failure to bind a socket.
    ListenError,        ///< Failure to listen on a socket.
    AcceptError,        ///< Failure to accept a connection on a socket.
    ConnectError,       ///< Failure to connect to a socket.
    IPCReadError,       ///< Failure to read from IPC.
    PizzaOrderError,    ///< Incorrect pizza order format.
};

/**
 * @class PlazzaException
 * @brief Custom exception class for the Plazza project.
 */
class PlazzaException : public std::exception {
    /**
     * @brief Map of error types to their corresponding error messages.
     */
    std::map<PlazzaErrorTypes, std::string> _error_map {
        {IncorrectArgs, "Incorrect arguments!"},
        {ForkError, "Fail to fork!"},
        {PollError, "An error occurred while polling reception socket!"},
        {SocketCreationError, "An error occurred while creating a socket!"},
        {BindError, "An error occurred while binding the reception socket!"},
        {ListenError, "An error occurred while listening the reception socket!"},
        {AcceptError, "An error occurred while accepting connection on the reception socket!"},
        {ConnectError, "An error occurred while connecting to the reception socket!"},
        {IPCReadError, "An error occurred while reading an IPC!"},
        {PizzaOrderError, "Incorrect pizza order!"}
    };

    std::string _error; ///< The stored error message.

public:
    /**
     * @brief Construct a new PlazzaException object.
     * 
     * @param type The type of the error.
     */
    explicit PlazzaException(PlazzaErrorTypes type);

    /**
     * @brief Get the error message.
     * 
     * @return const char* The error message.
     */
    [[nodiscard]] const char* what() const noexcept override;
};