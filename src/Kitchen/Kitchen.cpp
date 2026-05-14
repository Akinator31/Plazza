//
// Created by pavel on 12/05/2026.
//

#include "Kitchen.hpp"

Kitchen::Kitchen() {}

void Kitchen::close() const {
    this->_process.kill();
}