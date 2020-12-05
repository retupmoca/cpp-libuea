#pragma once
#include <vector>
#include <string>

namespace uea {
    struct subprocess {
        enum class io {
            close,
            share,
            open
        };
        struct spawn_options {
            bool use_path = false;
            io stdin = io::close;
            io stdout = io::close;
            io stderr = io::close;
        };
        subprocess(std::vector<std::string> execute) : subprocess(execute, {}) {}
        subprocess(std::vector<std::string> execute, spawn_options options);

        pid_t pid;
        int stdin = -1;
        int stdout = -1;
        int stderr = -1;

        int join();
    };
}
