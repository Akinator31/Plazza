//
// Created by pavel on 12/05/2026.
//

#include "Process.hpp"

#include <signal.h>
#include <wait.h>

#include "Errors/PlazzaErrors.hpp"

namespace Internal {
    Process::Process() {
        this->_pid = fork();

        if (this->_pid < 0)
            throw PlazzaException(ForkError);
    }

    bool Process::isChild() const {
        return this->_pid == 0;
    }

    bool Process::isParent() const {
        return this->_pid > 0;
    }

    pid_t Process::pid() const {
        return this->_pid;
    }

    void Process::kill() const {
        ::kill(this->_pid, SIGINT);
    }

    void Process::wait() const {
        waitpid(this->_pid, nullptr, 0);
    }
}