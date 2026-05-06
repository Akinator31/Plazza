//
// Created by pavel on 06/05/2026.
//

#include "PlazzaErrors.hpp"

PlazzaException::PlazzaException(const PlazzaErrorTypes type) {
    this->_error = this->_error_map[type];
}

const char* PlazzaException::what() const noexcept {
    return this->_error.c_str();
}