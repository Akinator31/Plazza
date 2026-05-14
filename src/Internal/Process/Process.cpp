//
// Created by pavel on 12/05/2026.
//

#include "Process.hpp"

#include <signal.h>

#include "Errors/PlazzaErrors.hpp"

namespace Internal {
    Process::Process() {
        const int pid = fork();

        if (pid < 0)
            throw PlazzaException(ForkError);
        this->_pid = pid;
    }

    void Process::kill() const {
        ::kill(this->_pid, SIGINT);
    }
}