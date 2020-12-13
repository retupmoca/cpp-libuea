#include <unistd.h>

#include <vector>
#include <string>

namespace uea {
    /**
     * passwd: WARNING: reads /etc/passwd and /etc/shadow directly! This means
     * that if you use any kind of nsswitch.conf settings that load user data
     * from elsewhere, this class will not work correctly!
     *
     * The intention is for this to be used where a) user data is stored in the
     * traditional files, and b) where the glibc libnss functions (getpwuid,
     * getspnam, etc) cannot be used, either because static linking is required
     * or for any other reason.
     */
    struct passwd {
        uid_t uid;
        std::string username;

        passwd(uid_t uid);
        passwd(std::string username);
        static std::vector<passwd> load_all();

        std::string password_hash();
    };
}
