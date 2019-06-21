#pragma once

#include <cstdint>
#include <vector>
#include <mutex>
#include <functional>

//������Ψһid������
namespace svrlib {
    class puid {
    public:
        puid(void) : id_(1) {};

        uint64_t get_id(void) {
            std::mutex::scoped_lock guard(lock_);
            if (0 == id_) id_ = 1;
            uint64_t ret = id_;
            ++id_;
            return ret;
        };

    private:
        uint64_t id_;
        std::mutex lock_;
    };
}

