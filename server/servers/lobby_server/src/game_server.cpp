/*
* game_server.cpp
*
*  modify on: 2015-12-2
*      Author: toney
*/
#include "game_define.h"
#include "framework/application.h"
#include "net/game_net_mgr.h"
#include "data_cfg_mgr.h"
#include "svrlib.h"
#include <iostream>
#include "game_server_config.h"
#include "time/time_function.h"
#include "lua_logic/lua_logic.h"
#include "net/center_client.h"
#include "net/dbagent_client.h"
#include "player_mgr.h"
#include "redis_mgr.h"
#include "player.h"
#include "lua_service/lua_bind.h"

using namespace svrlib;
using namespace std;

bool CApplication::Initialize() {
    defLuaConfig(m_solLua);
    defLuaLogic(m_solLua);

    // 加载lua 配置
    auto lubBind = lua_bind(m_solLua);
    lubBind.add_lua_cpath({"clualib"});
    lubBind.add_lua_path({"lualib","lua","scp_lua"});

    lubBind.reload_lua_dir("lua");

    SOL_CALL_LUA(m_solLua["init_lua_service"](m_luaService));

    bool bRet = m_solLua["lobby_config"](m_uiServerID, &GameServerConfig::Instance());
    if (bRet == false) {
        LOG_ERROR("load lobby_config fail ");
        return false;
    }
    LOG_INFO("load config is:id:{}", m_uiServerID);
    // 读取配置信息
    if (CDataCfgMgr::Instance().Init() == false) {
        LOG_ERROR("init datamgr fail ");
        return false;
    }
    do {
        // client处理端口
        stIOHANDLER_DESC desc[2];
        desc[0].ioHandlerKey = 0;
        desc[0].maxConnectBuffSize = SERVER_SOCKET_BUFF_SIZE;
        desc[0].sendBufferSize = PACKET_MAX_SIZE * 2;
        desc[0].recvBufferSize = PACKET_MAX_SIZE;
        desc[0].timeOut = 2000;
        desc[0].maxPacketSize = PACKET_MAX_SIZE;
        desc[0].allocFunc = []() { return new CClientNetObj(); };
        desc[0].webSocket = true;

        desc[1].ioHandlerKey = 1;
        desc[1].maxConnectBuffSize = SERVER_SOCKET_BUFF_SIZE;
        desc[1].sendBufferSize = SERVER_SOCKET_BUFF_SIZE;
        desc[1].recvBufferSize = SERVER_SOCKET_BUFF_SIZE;
        desc[1].timeOut = 60 * 60 * 24;
        desc[1].maxPacketSize = INNER_MAX_SIZE;

        if (!m_iocpServer.AddIoHandler(desc[0]) || !m_iocpServer.AddIoHandler(desc[1])) {
            LOG_ERROR("IOCP Init fail");
            return false;
        }

        uint32_t port = m_solLua["get_lobby_listen"](m_uiServerID);

        if (!m_iocpServer.StartListen(0, "0.0.0.0", port)) {
            LOG_ERROR("IOCP SERVER StartListen fail {}", port);
            return false;
        }
    } while (false);

    if (!CRedisMgr::Instance().Init(m_ioContext, GameServerConfig::Instance().redisConf)) {
        LOG_ERROR("init redis fail");
        return false;
    }
    if (!CPlayerMgr::Instance().Init()) {
        LOG_ERROR("playermgr init fail");
        return false;
    }

    net::svr::server_info info;
    info.set_svrid(GetServerID());

    //连接中心服
    auto centerIp = m_solLua.get<sol::table>("server_config").get<sol::table>("center");
    if (CCenterClientMgr::Instance().Init(1, info, centerIp.get<string>("ip"), centerIp.get<int>("port")) == false) {
        LOG_ERROR("init center client mgr fail");
        return false;
    }
    //连接DBAgent
    auto dbagentIp = m_solLua.get<sol::table>("server_config").get<sol::table>("dbagent");
    if (CDBAgentClientMgr::Instance().Init(1, info, dbagentIp.get<string>("ip"), dbagentIp.get<int>("port")) == false) {
        LOG_ERROR("init dbagent client mgr fail");
        return false;
    }

    m_luaService->start();

    //test toney
    static TimerEvent<std::function<void()>> timer([]() {
        CPlayer *pPlayer = new CPlayer(PLAYER_TYPE_ONLINE);
        pPlayer->SetUID(110);
        CPlayerMgr::Instance().AddPlayer(pPlayer);
        pPlayer->OnLogin();
    });
    schedule(&timer, 10000);

    return true;
}

void CApplication::ShutDown() {

    CPlayerMgr::Instance().ShutDown();
    CRedisMgr::Instance().ShutDown();


    m_luaService->exit();
}

/**
* 本函数将在程序启动时和每次配置改变时被调用。
* 第一次调用将在Initialize()之前
*/
void CApplication::ConfigurationChanged() {
    // 重加载配置
    LOG_INFO("configuration changed");
    CDataCfgMgr::Instance().Reload();

    auto lubBind = lua_bind(m_solLua);
    lubBind.reload_lua_dir("lua");

}

void CApplication::Tick(uint64_t diffTime) {

}

void CApplication::ExceptionHandle() {

}

int main(int argc, char *argv[]) {
    return FrameworkMain(argc, argv);
}
