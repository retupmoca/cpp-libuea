#pragma once
#include <vector>
#include <string>

namespace uea {
    struct spawn_options {
        bool use_path = false;
    };
    void spawn(std::vector<std::string> execute, spawn_options options); 
}
