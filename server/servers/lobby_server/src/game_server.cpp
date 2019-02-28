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
#include "stdafx.h"
#include "game_server_config.h"
#include "utility/time_function.h"
#include "lua_logic.h"
#include "center_client.h"
#include "player_cache_mgr.h"

using namespace svrlib;
using namespace std;

bool CApplication::Initialize() {
    defLuaConfig(m_solLua);
    defLuaLogic(m_solLua);

    // 加载lua 配置
    m_solLua["load_logic_script"]();
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
    if(CPlayerCacheMgr::Instance().Init(110, false) == false){
        LOG_ERROR("init player cache mgr fail ");
        return false;
    }
    do
    {

        // client处理端口
        stIOHANDLER_DESC desc[2];
        desc[0].dwIoHandlerKey = 0;
        desc[0].dwMaxAcceptSession = 5000;
        desc[0].dwMaxConnectSession = 0;
        desc[0].dwMaxConnectBuffSize = SERVER_SOCKET_BUFF_SIZE;
        desc[0].dwSendBufferSize = PACKET_MAX_SIZE * 2;
        desc[0].dwRecvBufferSize = 1024 * 4;
        desc[0].dwTimeOut = 2000;
        desc[0].dwMaxPacketSize = 1024 * 2;
        desc[0].pool = new CClientNetObj();
        desc[0].openMsgQueue = false;
        desc[0].webSocket = true;

        desc[1].dwIoHandlerKey = 1;
        desc[1].dwMaxAcceptSession = 0;
        desc[1].dwMaxConnectSession = 100;
        desc[1].dwMaxConnectBuffSize = 1024 * 1024;
        desc[1].dwSendBufferSize = SERVER_SOCKET_BUFF_SIZE;
        desc[1].dwRecvBufferSize = SERVER_SOCKET_BUFF_SIZE;
        desc[1].dwTimeOut = 60 * 60 * 24;
        desc[1].dwMaxPacketSize = INNER_MAX_SIZE;
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

    if (!CRedisMgr::Instance().Init(GameServerConfig::Instance().redisConf))
    {
        LOG_ERROR("init redis fail");
        return false;
    }
    if (!CPlayerMgr::Instance().Init())
    {
        LOG_ERROR("playermgr init fail");
        return false;
    }
    if (!CGobalEventMgr::Instance().Init())
    {
        LOG_ERROR("global mgr init fail");
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

    return true;
}

void CApplication::ShutDown() {

    CPlayerMgr::Instance().ShutDown();
    CRedisMgr::Instance().ShutDown();

    CDBMysqlMgr::Instance().ShutDown();
}

/**
* 本函数将在程序启动时和每次配置改变时被调用。
* 第一次调用将在Initialize()之前
*/
void CApplication::ConfigurationChanged() {
    // 重加载配置
    LOG_INFO("configuration changed");
    CDataCfgMgr::Instance().Reload();

    try
    {
        m_solLua["load_logic_script"]();
    }
    catch (sol::error &e)
    {
        LOG_ERROR("sol error:{}", e.what());
    }
}

void CApplication::Tick() {
    CGobalEventMgr::Instance().ProcessTime();
}

void CApplication::ExceptionHandle() {

}

int main(int argc, char *argv[]) {
    return FrameworkMain(argc, argv);
}
