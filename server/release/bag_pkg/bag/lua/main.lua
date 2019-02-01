-- 加载服务器逻辑脚本
function load_logic_script()
    c_log("load_logic_script");

    -- 全局设置随机数
    math.randomseed(os.time())
    reload_file("common_define");
    reload_file("common_func");

    reload_file("main");
    reload_file("item");
    reload_file("player_handle");
    reload_file("huodong");
    reload_file("game_config");
    reload_file("game_logic");

    return true;
end

function reload_file(filename)
    --卸载旧文件
    package.loaded[filename] = nil
    --装载新文件
    require(filename);
    c_log("load lua file:" .. filename .. ".lua");
    return true;
end


