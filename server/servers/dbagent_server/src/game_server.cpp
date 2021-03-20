/*
* game_server.cpp
*
*  modify on: 2019-9-6
*      Author: toney
*/
#include "game_define.h"
#include "framework/application.h"
#include "game_server_config.h"
#include "server_mgr.h"
#include "dbmysql_mgr.h"
#include "svrlib.h"
#include <iostream>
#include "time/time.hpp"
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
    LOG_INFO("load config is:id:{},uuid:{}", m_uiServerID, m_uuid);
    // db
    if (CDBMysqlMgr::Instance().Init(GameServerConfig::Instance().DBConfs) == false) {
        LOG_ERROR("init mysqlmgr fail ");
        return false;
    }
    // 初始化共享内存缓存
    if (CPlayerCacheMgr::Instance().Init(m_ioContext, 110, false,
                                         [](uint32_t uid, uint8_t cacheType, const string &data) {
                                             CDBMysqlMgr::Instance().SavePlayerDataInfo(uid, cacheType, data,
                                                                                        time::getSysTime());
                                         }) == false) {
        LOG_ERROR("init player cache mgr fail ");
        return false;
    }
    if (CServerMgr::Instance().Init() == false) {
        LOG_ERROR("CenterMgr init fail");
        return false;
    }

    do {
        auto dbagentIp = m_solLua.get<sol::table>("server_config").get<sol::table>("dbagent");
        auto tcpSvr = std::make_shared<TCPServer>(m_ioContext, "0.0.0.0", dbagentIp.get<int>("port"), "dbagentServer");
        tcpSvr->SetConnectionCallback([](const TCPConnPtr &conn) {
            if (conn->IsConnected()) {
                LOG_DEBUG("{},connection accepted", conn->GetName());
                conn->SetMessageDecode(make_shared<InnerDecode>());
            } else {
                CServerMgr::Instance().RemoveServer(conn);
                LOG_DEBUG("{},connection disconnecting", conn->GetName());
            }
        });
        tcpSvr->SetMessageCallback([](const TCPConnPtr &conn, uint8_t *pData, uint32_t length) {
            //LOG_DEBUG("recv msg {}",buffer.Size());
            CServerMgr::Instance().OnHandleClientMsg(conn, pData, length);
        });
        tcpSvr->Start();
        m_tcpServers.push_back(tcpSvr);

    } while (false);


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
