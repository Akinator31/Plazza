//
// Created by pavel on 12/05/2026.
//

#pragma once

#define PID int
#include "Internal/Process/Process.hpp"

class Kitchen {
    Internal::Process _process;

public:
    Kitchen();
    void close() const;
};