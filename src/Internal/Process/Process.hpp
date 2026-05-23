//
// Created by pavel on 12/05/2026.
//

#pragma once
#include <sched.h>

namespace Internal {
    class Process {
        pid_t _pid;

    public:
        explicit Process();

        [[nodiscard]] bool isChild() const;
        [[nodiscard]] bool isParent() const;
        [[nodiscard]] pid_t pid() const;
        void kill() const;
        void wait() const;
    };
}