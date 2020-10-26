#pragma once
#include <array>

namespace uea {
    template<typename T>
    struct queue {
        virtual ~queue();

        virtual void put(const T&) = 0;
        virtual T get() = 0;
    };

    template<typename T, unsigned SZ>
    class ring_queue : public queue<T> {
    public:
        void put(const T& val) {
            size_t next = (writer + 1) % SZ;
            if (next != reader) {
                ring[writer] = std::move(val);
                writer = next;
            }
            else {} // throw? block?
        }

        T get() override {
            if (reader != writer) {
                T val = std::move(ring[reader]);
                reader = (reader + 1) % SZ;
                return val;
            }
            else {} // throw? block?
        }
    private:
        std::array<T, SZ> ring;
        size_t writer = 0;
        size_t reader = 0;
    };
};
