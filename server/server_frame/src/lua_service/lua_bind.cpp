
#include "lua_service/lua_bind.h"
#include "lua_service/lua_service.h"
#include "config/config.h"
#include "svrlib.h"

namespace svrlib {

    lua_bind::lua_bind(sol::state& _lua)
            :lua(_lua)
    {
    }

    lua_bind::~lua_bind()
    {
    }

    void lua_bind::export_lua_bind()
    {
        bind_conf();
        bind_log();
        bind_util();
        bind_service();

    }

    void lua_bind::bind_conf()
    {
        lua.new_usertype<stDBConf>
                (
                        "stDBConf",
                        "SetDBInfo", &stDBConf::SetDBInfo
                );
        lua.new_usertype<stRedisConf>
                (
                        "stRedisConf",
                        "SetRedisHost", &stRedisConf::SetRedisHost
                );
        lua.new_usertype<stServerCfg>
                (
                        "stServerCfg",
                        "SetLogInfo", &stServerCfg::SetLogInfo
                );
    }

    void lua_bind::bind_util()
    {
        lua.set_function("getSysTime", []() {
            return getSysTime();
        });
        lua.set_function("getTick", []() {
            return getTickCount64();
        });
        lua.set_function("curTimeStr", []() {
            return time_format(getSysTime());
        });


    }

    void lua_bind::bind_log()
    {
        lua.set_function("log_debug", [](const char* logStr) {
            LOG_DEBUG("{}", logStr);
        });
        lua.set_function("log_error", [](const char* logStr) {
            LOG_ERROR("{}", logStr);
        });
        lua.set_function("log_info", [](const char* logStr) {
            LOG_INFO("{}", logStr);
        });
    }

    void lua_bind::bind_service()
    {
        lua.new_usertype<lua_service>
                (
                        "lua_service",
                        "set_start", &lua_service::set_start,
                        "set_exit", &lua_service::set_exit,
                        "set_dispatch", &lua_service::set_dispatch
                );
    }
};


