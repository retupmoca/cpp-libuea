#pragma once
#include <vector>
#include <string>

namespace uea {
    struct subprocess {
        pid_t pid;

        int join();
    };
    struct subprocess_piped : subprocess {
        int stdin;
        int stdout;
        int stderr;
    };
    struct spawn_options {
        bool use_path = false;
    };
    subprocess spawn(std::vector<std::string> execute, spawn_options options); 
    subprocess spawn_shared(std::vector<std::string> execute, spawn_options options); 
    subprocess_piped spawn_piped(std::vector<std::string> execute, spawn_options options); 
}
