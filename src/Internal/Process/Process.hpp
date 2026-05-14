//
// Created by pavel on 12/05/2026.
//

#pragma once

namespace Internal {
    class Process {
        int _pid;

    public:
        explicit Process();

        void kill() const;
    };
}