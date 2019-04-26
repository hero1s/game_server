#pragma once

#include <string>
#include "sol/sol.hpp"
#include "modern/noncopyable.hpp"

namespace svrlib {

    class lua_service;

    class lua_bind : public svrlib::noncopyable {
    public:
        explicit lua_bind(sol::state& _lua);

        ~lua_bind();

        void export_lua_bind();

    protected:
        void bind_conf();

        void bind_util();

        void bind_log();

        void bind_service();

    private:
        sol::state& lua;
    };

};


