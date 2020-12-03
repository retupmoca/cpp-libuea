#include <uea/ps.hpp>

#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

namespace uea {
    void spawn(std::vector<std::string> execute, spawn_options options) {
        if(execute.size() < 1)
            return;

        pid_t child_pid;

        posix_spawn_file_actions_t spawn_actions;
        posix_spawn_file_actions_init(&spawn_actions);
        posix_spawnattr_t attrs;
        posix_spawnattr_init(&attrs);

        std::vector<char*> args;
        args[execute.size()] = 0;
        for(size_t i=0; i<execute.size(); ++i)
            args[i] = &execute[i][0];

        int err = options.use_path ?
            posix_spawnp(&child_pid, args[0], &spawn_actions, &attrs, &args[0], environ)
          : posix_spawn(&child_pid, args[0], &spawn_actions, &attrs, &args[0], environ);

        if(err){throw "BOOM";}

        siginfo_t result;
        waitid(P_PID, child_pid, &result, WEXITED);
    }
}
