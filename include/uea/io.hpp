#pragma once
#include <fmt/format.h>
#include <vector>
#include <string>

namespace uea {
    struct posix_error : public std::exception {
        posix_error() { posix_errno = errno; }

        const char * what() const noexcept override {
            return strerror(posix_errno);
        }

        int posix_errno;
    };

    struct end_of_file : public std::exception {
        const char * what() const noexcept override {
            return "End of file";
        }
    };
}

namespace uea {
    struct fd {
        struct open_file_options {
            bool read = true;
            bool write = false;
            bool create = false;
            bool create_only = false;
            bool truncate = false;
        };
        static fd open_file(std::string path);
        static fd open_file(std::string path, open_file_options options);
        static std::array<fd, 2> open_pipe();

        fd(int fd);
        fd(const fd& from);
        fd(fd&& from);
        ~fd();
        fd& operator =(const fd& from);

        // if you do something dumb like close() this fd
        // you get to keep both pieces
        int _internal_fd() { return _fd; }

        explicit operator bool();

        std::string getline();
        void for_lines(std::function<void(std::string)> fn);
        void print(std::string data);
        void close();

        // TODO: not a C api
        ssize_t read(char * buf, size_t len);

    private:
        int _fd;
    };

    extern fd stdin;
    extern fd stdout;
    extern fd stderr;
}

namespace uea {
    struct color_t {
        bool enabled;

        color_t();

        std::string reset();

        std::string black();
        std::string red();
        std::string green();
        std::string yellow();
        std::string blue();
        std::string magenta();
        std::string cyan();
        std::string white();

        std::string bg_black();
        std::string bg_red();
        std::string bg_green();
        std::string bg_yellow();
        std::string bg_blue();
        std::string bg_magenta();
        std::string bg_cyan();
        std::string bg_white();

        std::string bold_black();
        std::string bold_red();
        std::string bold_green();
        std::string bold_yellow();
        std::string bold_blue();
        std::string bold_magenta();
        std::string bold_cyan();
        std::string bold_white();
    };
    extern color_t color;
}

namespace uea {
    void print(std::string);

    template<typename... Targs>
    void print(std::string format, Targs... Fargs) {
        print(fmt::format(format, Fargs...));
    }
}

namespace uea {
    template<typename... Targs>
    void say(std::string format, Targs... Fargs) {
        print(format + "\n", Fargs...);
    }

    inline void say(std::string stuff = "") {
        print(stuff + "\n");
    }
}

namespace uea {
    std::string getline();

    template<typename... Targs>
    std::string input(Targs... Fargs) {
        print(Fargs...);
        return getline();
    }
}

namespace uea {
    std::vector<std::string> glob(std::string);

    template<typename... Targs>
    std::vector<std::string> glob(std::string format, Targs... Fargs) {
        return glob(fmt::format(format, Fargs...));
    }
}

namespace uea {
    int menu(std::vector<std::string> options);

    template <typename T>
    T menu(std::vector<std::pair<T, std::string>> options) {
        std::vector<std::string> optionNames;
        std::transform(options.begin(), options.end(), std::back_inserter(optionNames),
            [](auto a) { return a.second; }
        );

        int selected = menu(optionNames);

        return options.at(selected).first;
    }
}
