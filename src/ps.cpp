#include <uea/ps.hpp>

#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

namespace uea {
    subprocess::subprocess(std::vector<std::string> execute, spawn_options options) {
        posix_spawn_file_actions_t spawn_actions;
        posix_spawn_file_actions_init(&spawn_actions);

        std::optional<std::array<fd, 2>> tmp_stdin;
        if (options.stdin == io::close)
            posix_spawn_file_actions_addclose(&spawn_actions, 0);
        else if (options.stdin == io::open) {
            tmp_stdin = fd::make_pipe();
            posix_spawn_file_actions_adddup2(&spawn_actions, tmp_stdin->at(0)._fd, 0);
            stdin = tmp_stdin->at(1);
        }

        std::optional<std::array<fd, 2>> tmp_stdout;
        if (options.stdout == io::close)
            posix_spawn_file_actions_addclose(&spawn_actions, 1);
        else if (options.stdout == io::open) {
            tmp_stdout = fd::make_pipe();
            posix_spawn_file_actions_adddup2(&spawn_actions, tmp_stdout->at(1)._fd, 1);
            stdout = tmp_stdout->at(0);
        }

        std::optional<std::array<fd, 2>> tmp_stderr;
        if (options.stderr == io::close)
            posix_spawn_file_actions_addclose(&spawn_actions, 2);
        else if (options.stdout == io::open) {
            tmp_stderr = fd::make_pipe();
            posix_spawn_file_actions_adddup2(&spawn_actions, tmp_stderr->at(1)._fd, 2);
            stderr = tmp_stderr->at(0);
        }

        std::vector<char*> args(execute.size() + 1);
        args[execute.size()] = 0;
        for(size_t i=0; i<execute.size(); ++i)
            args[i] = &execute[i][0];

        char ** c_args = &args[0];
        if(!c_args || execute.size() < 1)
            throw "BOOM";

        int err = options.use_path ?
            posix_spawnp(&pid, args[0], &spawn_actions, nullptr, c_args, environ)
          : posix_spawn(&pid, args[0], &spawn_actions, nullptr, c_args, environ);

        if(err){throw "BOOM";}
    }

    int subprocess::join() {
        siginfo_t result;
        waitid(P_PID, pid, &result, WEXITED);
        return result.si_status;
    }
}
