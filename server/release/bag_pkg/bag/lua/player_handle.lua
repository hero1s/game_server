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

    return true;
end
-- 登出回调
function login_out(player)
    c_log("login out call back "..player:GetUID());

    return true;
end

-- 获取破产补助
function get_bankrupt_help(player)
    c_log("lua 领取破产补助" .. player:GetUID());
    local coin = player:GetAccountValue(emACC_VALUE_COIN);
    local bankrupt = player:GetBankruptCount();
    if coin > 2000 or bankrupt > 5 then
        c_log("破产补助不满足条件");
        return false;
    end ;
    -- 补助2000金币
    player:SyncChangeAccountValue(7, 0, 2000, 0);
    player:SetBankruptCount(bankrupt + 1);
    player:UpdateAccValue2Client();

    return true;
end



