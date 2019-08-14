
--*********************************************
--************ 服务器配置
--*********************************************
--数据库配置(改成只使用一个数据库地址toney)
local database_config = { ip = "139.199.209.147", port = 3307, user = "root", passwd = "e23456" };
local database_dbname = {}
database_dbname[0] = "chess";
database_dbname[1] = "chess_sysdata";
database_dbname[2] = "chess_center";
database_dbname[3] = "chess_log";

--redis配置
local redis_config = { host = "139.199.209.147",port = 13000,passwd="e2345" };
--全局配置信息
server_config =
{
    center = { ip = "127.0.0.1",port = 8888},
    dbagent= { ip = "127.0.0.1",port = 9999},
}

-- 中心服务器配置
function center_config(sid, gameConfig)
    load_db_config(sid, gameConfig);
    load_redis_config(sid, gameConfig);
    return true
end

-- dbagent服务器配置
function dbagent_config(sid, gameConfig)
    load_db_config(sid, gameConfig);
    load_redis_config(sid, gameConfig);
    return true
end

-- 大厅服务器配置
function lobby_config(sid, lobbyConfig)
    load_db_config(sid, lobbyConfig);
    load_redis_config(sid, lobbyConfig);
    return true;
end

-- 游戏服务器配置
function game_config(sid, gameConfig)
    load_db_config(sid, gameConfig);
    load_redis_config(sid, gameConfig);
    return true;
end

-- 大厅服端口
function get_lobby_listen(sid)
    return 7777;
end


-- 加载数据库
function load_db_config(sid, serviceConfig)
    for k, v in pairs(database_dbname) do
        local cfg = serviceConfig:GetDBConf(k);
        cfg:SetDBInfo(database_config.ip, database_config.port, v, database_config.user, database_config.passwd);
    end
end
-- 加载redis
function load_redis_config(serverID,serviceConfig)
    local cfg = serviceConfig:GetRedisConf();
    cfg:SetRedisHost(redis_config.host, redis_config.port,redis_config.passwd);
end
-- 设置服务器基础信息
function set_server_cfg(sid,serverCfg)

    local _loglv        = 0;
    local _logsize      = 52428800;
    local _logdays      = 5;
    local _logasync     = 0;
    local _logname      = sid.."_log.txt";
    local _logmysql     = sid.."_mysql_error.txt";
    if(sid ~= 9999) then
        _logmysql = "";
    end

    serverCfg:SetLogInfo(_loglv,_logsize,_logdays,_logasync,_logname,_logmysql);
end

