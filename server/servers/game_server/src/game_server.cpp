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
#include "lobby_mgr.h"
#include "svrlib.h"
#include <iostream>
#include <game_room_mgr.h>
#include "stdafx.h"
#include "game_server_config.h"
#include "utility/time_function.h"
#include "data_cfg_mgr.h"
#include "net/msg_lobby_handle.h"
#include "lobby_mgr.h"
#include "game_room.h"
#include "center_client.h"

using namespace svrlib;
using namespace std;

bool CApplication::Initialize()
{
	defLuaConfig(m_solLua);
	defLuaLogic(m_solLua);

	// 加载lua 配置
	auto lubBind = lua_bind(m_solLua);
	lubBind.add_lua_cpath({"clualib"});
	lubBind.add_lua_path({"lualib","lua","scp_lua"});

	lubBind.reload_lua_dir("lua");

	SOL_CALL_LUA(m_solLua["init_lua_service"](m_luaService));

	bool bRet = m_solLua["game_config"](m_uiServerID, &GameServerConfig::Instance());
	if (bRet == false) {
		LOG_ERROR("load game_config fail ");
		return false;
	}
	LOG_INFO("load config is:id:{}", m_uiServerID);
	// 读取配置信息
	if (CDataCfgMgr::Instance().Init() == false) {
		LOG_ERROR("init datamgr fail ");
		return false;
	}
	do {
		// center
		stIOHANDLER_DESC desc;
		desc.ioHandlerKey = 0;
		desc.maxConnectBuffSize = SERVER_SOCKET_BUFF_SIZE;
		desc.sendBufferSize = SERVER_SOCKET_BUFF_SIZE;
		desc.recvBufferSize = SERVER_SOCKET_BUFF_SIZE;
		desc.timeOut = 60 * 60 * 24;
		desc.maxPacketSize = INNER_MAX_SIZE;

		if (!m_iocpServer.AddIoHandler(desc)) {
			LOG_ERROR("IOCP Init fail");
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
	if (CGameRoomMgr::Instance().Init() == false)
	{
		LOG_ERROR("init room mgr fail");
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

	m_luaService->start();

	return true;
}

void CApplication::ShutDown()
{
	CPlayerMgr::Instance().ShutDown();
	CGameRoomMgr::Instance().ShutDown();
	CRedisMgr::Instance().ShutDown();

	CDBMysqlMgr::Instance().ShutDown();
}

/**
* 本函数将在程序启动时和每次配置改变时被调用。
* 第一次调用将在Initialize()之前
*/
void CApplication::ConfigurationChanged()
{
	// 重加载配置
	LOG_INFO("configuration changed");
	CDataCfgMgr::Instance().Reload(CDBMysqlMgr::Instance().GetSyncDBOper(DB_INDEX_TYPE_CFG));
	m_solLua["load_logic_script"]();
}

void CApplication::Tick()
{

}
void CApplication::ExceptionHandle()
{

}
int main(int argc, char* argv[])
{
	return FrameworkMain(argc, argv);
}
