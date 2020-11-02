#include <fmt/format.h>
#include <vector>
#include <string>

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
