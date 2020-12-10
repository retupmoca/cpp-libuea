#pragma once
#include <fmt/format.h>
#include <vector>
#include <string>

namespace uea {
    struct fd {
        static std::array<fd, 2> make_pipe();

        fd(int fd);
        fd(const fd& from);
        fd(fd&& from);
        ~fd();
        fd& operator =(const fd& from);

        explicit operator bool();

        std::string getline();
        void print(std::string data);

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
