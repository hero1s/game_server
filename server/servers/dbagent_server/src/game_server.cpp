/*
* game_server.cpp
*
*  modify on: 2019-9-6
*      Author: toney
*/
#include "game_define.h"
#include "framework/application.h"
#include "game_server_config.h"
#include "dbmysql_mgr.h"
#include "svrlib.h"
#include <iostream>
#include "time/time.hpp"
#include "msg_define.pb.h"
#include "center_client.h"

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

    net::svr::server_info info;
    info.set_svrid(GetServerID());
    info.set_game_type(0);
    info.set_game_subtype(0);
    info.set_svr_type(emSERVER_TYPE_DBA);
    info.set_uuid(m_uuid);

    //连接中心服
    auto centerIp = m_solLua.get<sol::table>("server_config").get<sol::table>("center");
    if (CDBACenterClientMgr::Instance().Init(info, centerIp.get<string>("ip"), centerIp.get<int>("port"),
                                          "center_connector", 1) == false) {
        LOG_ERROR("init center client mgr fail");
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
