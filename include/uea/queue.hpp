#pragma once
#include <array>
#include <condition_variable>

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
            if (is_full()) {
                std::lock_guard<std::mutex> lk(writer_mutex);
                if (is_full())
                    writer_blocked.wait(lk, [this](){ return !is_full(); });
            }

            size_t next = (writer + 1) % SZ;
            ring[writer] = std::move(val);

            bool was_empty = is_empty();
            if (was_empty)
                reader_mutex.lock();

            writer = next;

            if (was_empty) {
                reader_mutex.unlock();
                reader_blocked.notify_one();
            }
        }

        T get() override {
            if (is_empty()) {
                std::lock_guard<std::mutex> lk(reader_mutex);
                if (is_empty())
                    reader_blocked.wait(lk, [this](){ return !is_empty(); });
            }

            T val = std::move(ring[reader]);

            bool was_full = is_full();
            if (was_full)
                writer_mutex.lock();

            reader = (reader + 1) % SZ;

            if (was_full) {
                writer_mutex.unlock();
                writer_blocked.notify_one();
            }

            return val;
        }

        bool is_full() {
            size_t next = (writer + 1) % SZ;
            return next == reader;
        }

        bool is_empty() {
            return reader == writer;
        }

    private:
        std::array<T, SZ> ring;

        size_t writer = 0;
        std::mutex writer_mutex;
        std::condition_variable writer_blocked;

        size_t reader = 0;
        std::mutex reader_mutex;
        std::condition_variable reader_blocked;
    };
};
