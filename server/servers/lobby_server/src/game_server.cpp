/*
* game_server.cpp
*
*  modify on: 2015-12-2
*      Author: toney
*/
#include "game_define.h"
#include "framework/application.h"
#include "game_net_mgr.h"
#include "dbmysql_mgr.h"
#include "data_cfg_mgr.h"
#include "svrlib.h"
#include <iostream>
#include "game_server_config.h"
#include "utility/time_function.h"
#include "lua_logic.h"
#include "center_client.h"
#include "player_mgr.h"
#include "redis_mgr.h"
#include "player.h"

using namespace svrlib;
using namespace std;

bool CApplication::Initialize() {
    defLuaConfig(m_solLua);
    defLuaLogic(m_solLua);

    // 加载lua 配置
    SOL_CALL_LUA(m_solLua["load_logic_script"]());
    bool bRet = m_solLua["lobby_config"](m_uiServerID, &GameServerConfig::Instance());
    if (bRet == false)
    {
        LOG_ERROR("load lobby_config fail ");
        return false;
    }
    LOG_INFO("load config is:id:{}", m_uiServerID);
    // db
    if (CDBMysqlMgr::Instance().Init(GameServerConfig::Instance().DBConf) == false)
    {
        LOG_ERROR("init mysqlmgr fail ");
        return false;
    }
    // 读取配置信息
    if (CDataCfgMgr::Instance().Init() == false)
    {
        LOG_ERROR("init datamgr fail ");
        return false;
    }
    // 初始化共享内存缓存
    if(CPlayerCacheMgr::Instance().Init(m_ioContext,110, false,[](uint32_t uid, uint8_t cacheType, const string& data){}) == false){
        LOG_ERROR("init player cache mgr fail ");
        return false;
    }
    do
    {

        // client处理端口
        stIOHANDLER_DESC desc[2];
        desc[0].ioHandlerKey = 0;
        desc[0].maxAcceptSession = 5000;
        desc[0].maxConnectSession = 0;
        desc[0].maxConnectBuffSize = SERVER_SOCKET_BUFF_SIZE;
        desc[0].sendBufferSize = PACKET_MAX_SIZE * 2;
        desc[0].recvBufferSize = 1024 * 4;
        desc[0].timeOut = 2000;
        desc[0].maxPacketSize = 1024 * 2;
        desc[0].pool = new CClientNetObj();
        desc[0].openMsgQueue = false;
        desc[0].webSocket = true;

        desc[1].ioHandlerKey = 1;
        desc[1].maxAcceptSession = 0;
        desc[1].maxConnectSession = 100;
        desc[1].maxConnectBuffSize = 1024 * 1024;
        desc[1].sendBufferSize = SERVER_SOCKET_BUFF_SIZE;
        desc[1].recvBufferSize = SERVER_SOCKET_BUFF_SIZE;
        desc[1].timeOut = 60 * 60 * 24;
        desc[1].maxPacketSize = INNER_MAX_SIZE;
        desc[1].pool = NULL;
        desc[1].openMsgQueue = true;

        if (!m_iocpServer.AddIoHandler(desc[0]) || !m_iocpServer.AddIoHandler(desc[1]))
        {
            LOG_ERROR("IOCP Init fail");
            return false;
        }

        if (!m_iocpServer.StartListen(0, "0.0.0.0", 8888))
        {
            LOG_ERROR("IOCP SERVER StartListen fail {}", 8888);
            return false;
        }
    }
    while (false);

    if (!CRedisMgr::Instance().Init(m_ioContext,GameServerConfig::Instance().redisConf))
    {
        LOG_ERROR("init redis fail");
        return false;
    }
    if (!CPlayerMgr::Instance().Init())
    {
        LOG_ERROR("playermgr init fail");
        return false;
    }

    net::server_info info;
    info.set_svrid(GetServerID());

    auto centerIp = m_solLua.get<sol::table>("server_config").get<sol::table>("center");

    if (CCenterClientMgr::Instance().Init(1, info,centerIp.get<string>("ip"),centerIp.get<int>("port")) == false)
    {
        LOG_ERROR("init center client mgr fail");
        return false;
    }

    CDBMysqlMgr::Instance().ReportStartServer(true);

    //test toney
    CPlayer* pPlayer = new CPlayer(PLAYER_TYPE_ONLINE);
    pPlayer->SetUID(110);
    CPlayerMgr::Instance().AddPlayer(pPlayer);
    pPlayer->OnLogin();

    m_luaService->start();

    return true;
}

void CApplication::ShutDown() {

    CPlayerMgr::Instance().ShutDown();
    CRedisMgr::Instance().ShutDown();

    CDBMysqlMgr::Instance().ShutDown();

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

    SOL_CALL_LUA(m_solLua["load_logic_script"]());
}

void CApplication::Tick() {

}

void CApplication::ExceptionHandle() {

}

int main(int argc, char *argv[]) {
    return FrameworkMain(argc, argv);
}
