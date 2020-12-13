#include <uea/string.hpp>

namespace uea {
    std::vector<std::string> split(char d, std::string line) {
        std::vector<std::string> out;
        unsigned start = 0;
        for(unsigned cur = 0; cur < line.size(); ++cur) {
            if (line[cur] == d) {
                out.push_back(line.substr(start, cur - start));
                start = cur + 1;
            }
        }
        out.push_back(line.substr(start));
        return out;
    }
}
