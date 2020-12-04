#include <uea/ps.hpp>

#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

namespace uea {
    pid_t  _spawn(std::vector<std::string> execute, spawn_options options, posix_spawn_file_actions_t & spawn_actions) {
        std::vector<char*> args(execute.size() + 1);
        args[execute.size()] = 0;
        for(size_t i=0; i<execute.size(); ++i)
            args[i] = &execute[i][0];

        char ** c_args = &args[0];
        if(!c_args || execute.size() < 1)
            throw "BOOM";

        pid_t child_pid;

        int err = options.use_path ?
            posix_spawnp(&child_pid, args[0], &spawn_actions, nullptr, c_args, environ)
          : posix_spawn(&child_pid, args[0], &spawn_actions, nullptr, c_args, environ);

        if(err){throw "BOOM";}

        return child_pid;
    }
    subprocess spawn(std::vector<std::string> execute, spawn_options options) {
        posix_spawn_file_actions_t spawn_actions;
        posix_spawn_file_actions_init(&spawn_actions);
        posix_spawn_file_actions_addclose(&spawn_actions, 0);
        posix_spawn_file_actions_addclose(&spawn_actions, 1);
        posix_spawn_file_actions_addclose(&spawn_actions, 2);

        return { .pid=_spawn(execute, options, spawn_actions) };
    }
    subprocess spawn_shared(std::vector<std::string> execute, spawn_options options) {
        posix_spawn_file_actions_t spawn_actions;
        posix_spawn_file_actions_init(&spawn_actions);

        return { .pid=_spawn(execute, options, spawn_actions) };
    }
    subprocess_piped spawn_piped(std::vector<std::string> execute, spawn_options options) {
        int stdin[2], stdout[2], stderr[2];
        if(pipe(stdin) || pipe(stdout) || pipe(stderr))
            throw "BOOM";

        posix_spawn_file_actions_t spawn_actions;
        posix_spawn_file_actions_init(&spawn_actions);
        posix_spawn_file_actions_adddup2(&spawn_actions, stdin[0], 0);
        posix_spawn_file_actions_addclose(&spawn_actions, stdin[0]);
        posix_spawn_file_actions_addclose(&spawn_actions, stdin[1]);
        posix_spawn_file_actions_adddup2(&spawn_actions, stdout[1], 1);
        posix_spawn_file_actions_addclose(&spawn_actions, stdout[0]);
        posix_spawn_file_actions_addclose(&spawn_actions, stdout[1]);
        posix_spawn_file_actions_adddup2(&spawn_actions, stderr[1], 2);
        posix_spawn_file_actions_addclose(&spawn_actions, stderr[0]);
        posix_spawn_file_actions_addclose(&spawn_actions, stderr[1]);

        subprocess_piped ret{};
        ret.pid = _spawn(execute, options, spawn_actions);
        ret.stdin = stdin[1];
        ret.stdout = stdout[0];
        ret.stderr = stderr[0];
        return ret;
    }

    int subprocess::join() {
        siginfo_t result;
        waitid(P_PID, pid, &result, WEXITED);
        return result.si_status;
    }
}
