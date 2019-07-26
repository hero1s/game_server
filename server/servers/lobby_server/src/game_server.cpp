/*
* game_server.cpp
*
*  modify on: 2015-12-2
*      Author: toney
*/
#include "game_define.h"
#include "framework/application.h"
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
#include "client_msg/msg_client_handle.h"

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
        uint32_t port = m_solLua["get_lobby_listen"](m_uiServerID);
        auto tcpSvr = std::make_shared<TCPServer>(m_ioContext, "0.0.0.0", port, "lobbyServer");
        tcpSvr->SetConnectionCallback([](const TCPConnPtr& conn) {
            if (conn->IsConnected()) {
                LOG_DEBUG("{},connection accepted",conn->GetName());
            }
            else {
                LOG_ERROR("client ondisconnect:{}--{}", conn->GetUID(), conn->GetRemoteAddress());
                uint32_t uid = conn->GetUID();
                CPlayer* pPlayer = (CPlayer*) CPlayerMgr::Instance().GetPlayer(uid);
                if (pPlayer != NULL)
                {
                    // 不直接断线，保留一定时间
                    pPlayer->SetSession(nullptr);
                    conn->SetUID(0);
                }
                LOG_DEBUG("{},connection disconnecting",conn->GetName());
            }
        });
        tcpSvr->SetMessageCallback([](const TCPConnPtr& conn, ByteBuffer& buffer) {
            LOG_DEBUG("recv msg {}",std::string(buffer.Data(), buffer.Size()));
            CHandleClientMsg::Instance().OnHandleClientMsg(conn,(uint8_t*)buffer.Data(),buffer.Size());
        });
        tcpSvr->Start();
        m_tcpServers.push_back(tcpSvr);

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
    if (CCenterClientMgr::Instance().Init(info, centerIp.get<string>("ip"), centerIp.get<int>("port")) == false) {
        LOG_ERROR("init center client mgr fail");
        return false;
    }
    //连接DBAgent
    auto dbagentIp = m_solLua.get<sol::table>("server_config").get<sol::table>("dbagent");
    if (CDBAgentClientMgr::Instance().Init(info, dbagentIp.get<string>("ip"), dbagentIp.get<int>("port")) == false) {
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
