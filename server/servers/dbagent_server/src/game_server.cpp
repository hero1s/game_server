/*
* game_server.cpp
*
*  modify on: 2018-5-23
*      Author: toney
*/
#include "game_define.h"
#include "framework/application.h"
#include "game_server_config.h"
#include "server_mgr.h"
#include "dbmysql_mgr.h"
#include "svrlib.h"
#include <iostream>
#include "time/time_function.h"
#include "msg_define.pb.h"

using namespace svrlib;
using namespace std;

bool CApplication::Initialize() {
    defLuaConfig(m_solLua);

    // 加载lua 配置
    bool bRet = m_solLua["center_config"](m_uiServerID, &GameServerConfig::Instance());
    if (bRet == false) {
        LOG_ERROR("load center_config fail ");
        return false;
    }
    LOG_INFO("load config is:id:{}", m_uiServerID);
    // db
    if (CDBMysqlMgr::Instance().Init(GameServerConfig::Instance().DBConf) == false) {
        LOG_ERROR("init mysqlmgr fail ");
        return false;
    }
    // 初始化共享内存缓存
    if (CPlayerCacheMgr::Instance().Init(m_ioContext, 110, false,
                                         [](uint32_t uid, uint8_t cacheType, const string &data) {
                                             CDBMysqlMgr::Instance().SavePlayerDataInfo(uid,cacheType, data, getSysTime());
                                         }) == false) {
        LOG_ERROR("init player cache mgr fail ");
        return false;
    }
    do {
        stIOHANDLER_DESC desc;
        desc.ioHandlerKey = 0;
        desc.maxConnectBuffSize = SERVER_SOCKET_BUFF_SIZE;
        desc.sendBufferSize = SERVER_SOCKET_BUFF_SIZE;
        desc.recvBufferSize = SERVER_SOCKET_BUFF_SIZE;
        desc.timeOut = 60 * 60 * 24;
        desc.maxPacketSize = INNER_MAX_SIZE;
        desc.allocFunc = [](){ return new CSvrCliNetObj(CServerMgr::Instance()); };

        if (!m_iocpServer.AddIoHandler(desc)) {
            LOG_ERROR("IOCP Init fail");
            return false;
        }
        auto dbagentIp = m_solLua.get<sol::table>("server_config").get<sol::table>("dbagent");
        if (!m_iocpServer.StartListen(0, "0.0.0.0", dbagentIp.get<int>("port"))) {
            LOG_ERROR("IOCP SERVER StartListen fail {}", dbagentIp.get<int>("port"));
            return false;
        }

    } while (false);
    if (CServerMgr::Instance().Init() == false) {
        LOG_ERROR("CenterMgr init fail");
        return false;
    }

    LOG_INFO("dbagent server start is successed {}", m_uiServerID);

    return true;
}

void CApplication::ShutDown() {
    CPlayerCacheMgr::Instance().ShutDown();
    CDBMysqlMgr::Instance().ShutDown();
}

/**
* 本函数将在程序启动时和每次配置改变时被调用。
* 第一次调用将在Initialize()之前
*/
void CApplication::ConfigurationChanged() {
    // 重加载配置
    LOG_INFO("configuration changed");

}

void CApplication::Tick(uint64_t diffTime) {

}

void CApplication::ExceptionHandle() {

}

int main(int argc, char *argv[]) {
    return FrameworkMain(argc, argv);
}
