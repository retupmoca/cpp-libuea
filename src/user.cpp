#include <uea/user.hpp>
#include <uea/io.hpp>
#include <uea/string.hpp>

namespace uea {
    passwd::passwd(uid_t uid) {
        fd etc_passwd = fd::open_file("/etc/passwd");
        std::string uidstring = fmt::format("{}", uid);
        std::string line; 
        while((line = etc_passwd.getline()) != "") {
            std::vector<std::string> fields = split(':', line);
            if (fields[2] == uidstring) {
                this->uid = uid;
                username = fields[0];
                return;
            }
        }

        throw "Not Found";
    }

    std::string passwd::password_hash() {
        fd etc_shadow = fd::open_file("/etc/shadow");
        std::string line; 
        while((line = etc_shadow.getline()) != "") {
            std::vector<std::string> fields = split(':', line);
            if (fields[0] == username) {
                return fields[1];
            }
        }

        throw "Not Found";
    }
}
