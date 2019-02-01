
pb = require "protobuf"
pb.register_file("../server_config/lua/cs_msg.pb")


-- 新的一天回调
function new_day(player)
    c_log("new day call back "..player:GetUID());

    return true;
end
-- 新的一周回调
function new_week(player)
    c_log("new week call back "..player:GetUID());

    return true;
end
-- 新的一月回调
function new_month(player)
    c_log("new month call back "..player:GetUID());

    return true;
end
-- 登录回调
function login_on(player)
    c_log("login on call back "..player:GetUID());

    stringbuffer = pb.encode("net.server_info",
    {
        svrid  	   	 = 1;		-- 服务器ID
        svr_type     = 2;		-- 服务器类型
        game_type  	 = 3;		-- 游戏类型
        game_subtype = 4;		-- 游戏子类型
    })
    result = pb.decode("net.server_info", stringbuffer)
    c_log("test protobuf:svrid "..result.svrid);

    return true;
end
-- 登出回调
function login_out(player)
    c_log("login out call back "..player:GetUID());

    return true;
end




