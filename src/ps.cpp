#include <uea/ps.hpp>

#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

namespace uea {
    void spawn(std::vector<std::string> execute, spawnOptions options) {
        if(execute.size() < 1)
            return;

        pid_t childPid;

        posix_spawn_file_actions_t spawn_actions;
        posix_spawn_file_actions_init(&spawn_actions);
        posix_spawnattr_t attrs;
        posix_spawnattr_init(&attrs);
        char ** args = new char*[execute.size()];
        for(int i=0; i<execute.size(); ++i)
            args[i] = &execute[i][0];
        int err = posix_spawn(&childPid, args[0], &spawn_actions, &attrs, args, environ);
        delete [] args;

        siginfo_t result;
        waitid(P_PID, childPid, &result, WEXITED);
    }
}
