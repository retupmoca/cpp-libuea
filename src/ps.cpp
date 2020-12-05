#include <uea/ps.hpp>

#include <spawn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

namespace uea {
    struct _pipefds {
        int read;
        int write;
    };
    static _pipefds _setup_subprocess_fd(
        posix_spawn_file_actions_t &spawn_actions,
        subprocess::spawn_options options,
        int target_fd
    ) {
        subprocess::io type = target_fd == 0 ? options.stdin
                            : target_fd == 1 ? options.stdout
                            : target_fd == 2 ? options.stderr
                            : throw "BOOM";
        if (type == subprocess::io::close) {
            posix_spawn_file_actions_addclose(&spawn_actions, target_fd);
            return {-1, -1};
        }
        else if (type == subprocess::io::open) {
            int p[2];
            if(pipe(p))
                throw "BOOM";
            if (target_fd == 0) {
                posix_spawn_file_actions_adddup2(&spawn_actions, p[0], target_fd);
                posix_spawn_file_actions_addclose(&spawn_actions, p[0]);
                posix_spawn_file_actions_addclose(&spawn_actions, p[1]);
                return {p[0], p[1]};
            }
            else {
                posix_spawn_file_actions_adddup2(&spawn_actions, p[1], target_fd);
                posix_spawn_file_actions_addclose(&spawn_actions, p[0]);
                posix_spawn_file_actions_addclose(&spawn_actions, p[1]);
                return {p[0], p[1]};
            }
        }
        else {
            return {-1, -1};
        }
    }
    subprocess::subprocess(std::vector<std::string> execute, spawn_options options) {
        posix_spawn_file_actions_t spawn_actions;
        posix_spawn_file_actions_init(&spawn_actions);
        std::vector<int> closefds;
        _pipefds tmp;
        tmp = _setup_subprocess_fd(spawn_actions, options, 0);
        if(tmp.read >= 0) {
            closefds.push_back(tmp.read);
            stdin = tmp.write;
        }
        tmp = _setup_subprocess_fd(spawn_actions, options, 1);
        if(tmp.read >= 0) {
            closefds.push_back(tmp.write);
            stdout = tmp.read;
        }
        tmp = _setup_subprocess_fd(spawn_actions, options, 2);
        if(tmp.read >= 0) {
            closefds.push_back(tmp.write);
            stderr = tmp.read;
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

        for (auto fd : closefds)
            close(fd);

        if(err){throw "BOOM";}
    }

    int subprocess::join() {
        siginfo_t result;
        waitid(P_PID, pid, &result, WEXITED);
        return result.si_status;
    }
}
