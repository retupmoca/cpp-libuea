#pragma once
#include <vector>
#include <string>

namespace uea {
    std::vector<std::string> split(char d, std::string line);

    template<typename T>
    bool secure_compare(T secret, T unknown) {
        size_t ssize = secret.size();
        size_t usize = unknown.size();

        // get the corner cases out of the way. These aren't secure cases anyway,
        // so don't bother going through the constant-time stuff
        if (ssize==0 && usize==0)
            return true;
        if (ssize==0 || usize==0)
            return false;

        // assume whatever we're comparing fits in a register
        // TODO: use c++ magic to get the actual subtype from T
        uint64_t result = ssize ^ usize;

        for (size_t i = 0; i < ssize; ++i) {
            result |= secret[i] ^ unknown[i % usize];
        }

        return !result;
    }

    inline bool startswith(std::string prefix, std::string unknown) {
        return unknown.rfind(prefix, 0) == 0;
    }

    inline bool endswith(std::string postfix, std::string unknown) {
        return unknown.find(postfix, unknown.size() - postfix.size()) == unknown.size() - postfix.size();
    }
}
