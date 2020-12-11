#include "uea/io.hpp"
#include <iostream>

#include <unistd.h>
#include <fcntl.h>

namespace __glob {
#include <glob.h>
}

namespace uea {
    fd stdin{0};
    fd stdout{1};
    fd stderr{2};

    std::array<fd, 2> fd::make_pipe() {
        int fds[2];
        pipe2(fds, O_CLOEXEC);
        return {fd{fds[0]}, fd{fds[1]}};
    }

    fd::fd(int fd) : _fd{fd} {}
    fd::fd(const fd& from) {
        _fd = fcntl(from._fd, F_DUPFD_CLOEXEC, 0);
    }
    fd::fd(fd&& from) : _fd{from._fd} {
        from._fd = -1;
    }
    fd::~fd() {
        if (*this)
            close();
    }
    fd& fd::operator =(const fd& from) {
        if (*this)
            close();
        _fd = fcntl(from._fd, F_DUPFD_CLOEXEC, 0);
        return *this;
    }

    fd::operator bool() {
        return _fd > -1;
    }

    // TODO: implementations that are less terrible
    void fd::print(std::string data) {
        write(_fd, data.c_str(), data.size());
    }
    std::string fd::getline() {
        std::string out;
        char c;
        while((read(_fd, &c, 1) > 0) && c != '\n')
            out += c;
        return out;
    }
    void fd::close() {
        if (*this) {
            ::close(_fd);
            _fd = -1;
        }
    }
}

namespace uea {
    void print(std::string stuff) {
        std::cout << stuff;
    }

    std::string getline() {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    std::vector<std::string> glob(std::string path) {
        std::vector<std::string> results;

        __glob::glob_t globbuf;
        __glob::glob(path.c_str(), 0, nullptr, &globbuf);

        for(size_t i=0; i < globbuf.gl_pathc; i++) {
            results.push_back(globbuf.gl_pathv[i]);
        }

        __glob::globfree(&globbuf);

        return results;
    }
}

namespace uea {
    std::string color_t::reset() { return enabled ? "\033[0m" : ""; }
    std::string color_t::black() { return enabled ? "\033[0;30m" : ""; }
    std::string color_t::red() { return enabled ? "\033[0;31m" : ""; }
    std::string color_t::green() { return enabled ? "\033[0;32m" : ""; }
    std::string color_t::yellow() { return enabled ? "\033[0;33m" : ""; }
    std::string color_t::blue() { return enabled ? "\033[0;34m" : ""; }
    std::string color_t::magenta() { return enabled ? "\033[0;35m" : ""; }
    std::string color_t::cyan() { return enabled ? "\033[0;36m" : ""; }
    std::string color_t::white() { return enabled ? "\033[0;37m" : ""; }
    std::string color_t::bg_black() { return enabled ? "\033[0;40m" : ""; }
    std::string color_t::bg_red() { return enabled ? "\033[0;41m" : ""; }
    std::string color_t::bg_green() { return enabled ? "\033[0;42m" : ""; }
    std::string color_t::bg_yellow() { return enabled ? "\033[0;43m" : ""; }
    std::string color_t::bg_blue() { return enabled ? "\033[0;44m" : ""; }
    std::string color_t::bg_magenta() { return enabled ? "\033[0;45m" : ""; }
    std::string color_t::bg_cyan() { return enabled ? "\033[0;46m" : ""; }
    std::string color_t::bg_white() { return enabled ? "\033[0;47m" : ""; }
    std::string color_t::bold_black() { return enabled ? "\033[1;30m" : ""; }
    std::string color_t::bold_red() { return enabled ? "\033[1;31m" : ""; }
    std::string color_t::bold_green() { return enabled ? "\033[1;32m" : ""; }
    std::string color_t::bold_yellow() { return enabled ? "\033[1;33m" : ""; }
    std::string color_t::bold_blue() { return enabled ? "\033[1;34m" : ""; }
    std::string color_t::bold_magenta() { return enabled ? "\033[1;35m" : ""; }
    std::string color_t::bold_cyan() { return enabled ? "\033[1;36m" : ""; }
    std::string color_t::bold_white() { return enabled ? "\033[1;37m" : ""; }

    color_t::color_t() {
        enabled = true;
    }

    color_t color;
}

namespace uea {
    int menu(std::vector<std::string> options) {
        while(true) {
            say();
            int i = 0;
            for (auto &o : options) {
                say("{}{}{}) {}", color.bold_cyan(), ++i, color.reset(), o);
            } 
            say();

            std::string selection = input("{}Selection?{} ", color.bold_white(), color.reset());

            try {
                return std::stoi(selection) - 1;
            }
            catch(std::invalid_argument const &) {
                say("Invalid option.");
            }
        }
    }
}
