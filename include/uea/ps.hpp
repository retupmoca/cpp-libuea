#pragma once
#include <vector>
#include <string>
#include <optional>

#include <uea/io.hpp>

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
        std::optional<fd> stdin;
        std::optional<fd> stdout;
        std::optional<fd> stderr;

        int join();
    };

    std::string pid_exepath(pid_t pid);
}
