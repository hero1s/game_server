-- 加载服务器逻辑脚本
function load_logic_script()
    log_debug("load_logic_script");

    -- 全局设置随机数
    math.randomseed(os.time())
    reload_file("base.preload");
    reload_file("main");
    reload_file("player_handle");
    return true;
end

function reload_file(filename)
    --卸载旧文件
    package.loaded[filename] = nil
    --装载新文件
    require(filename);
    log_debug("load lua file:" .. filename .. ".lua");
    return true;
end

function init_lua_service(lua_svr)
    log_debug("init lua service");
    lua_svr:set_start(function()
        log_debug(curTimeStr().." lua_service start call back");
    end);
    lua_svr:set_exit(function()
        log_debug(curTimeStr().." lua_service exit call back");
    end);
    lua_svr:set_dispatch(function(cmd,msg)
        log_debug("lua_service dispatch msg "..cmd..msg);
    end);

end
