//
// Created by pavel on 06/05/2026.
//

#pragma once
#include <exception>
#include <map>
#include <string>

enum PlazzaErrorTypes {
    IncorrectArgs,
};

class PlazzaException : public std::exception {
    std::map<PlazzaErrorTypes, std::string> _error_map {
        {IncorrectArgs, "Incorrect arguments!"}
    };

    std::string _error;

public:
    explicit PlazzaException(PlazzaErrorTypes type);
    [[nodiscard]] const char* what() const noexcept override;
};