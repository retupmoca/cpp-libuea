#include <uea/user.hpp>

namespace uea {
    passwd::passwd(uid_t uid) {}
    passwd::passwd(std::string username) {}
    std::vector<passwd> passwd::load_all() {return {};}

    std::string passwd::password_hash() {return "";}
}
