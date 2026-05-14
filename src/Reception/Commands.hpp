//
// Created by pavel on 13/05/2026.
//

#pragma once
#include "Reception.hpp"

class Commands {

public:
    static void status(Reception &reception, const std::string&);
    static void order(Reception &reception, std::string order);
};