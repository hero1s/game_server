/*
* game_server.cpp
*
*  modify on: 2018-5-23
*      Author: toney
*/
#include "game_define.h"
#include "framework/application.h"
#include "game_server_config.h"
#include "game_net_mgr.h"

#include "svrlib.h"
#include <iostream>
#include "stdafx.h"
#include "utility/time_function.h"

using namespace svrlib;
using namespace std;

bool CApplication::Initialize()
{
	defLuaConfig(m_solLua);

	// 加载lua 配置
	bool bRet = m_solLua["center_config"](m_uiServerID, &GameServerConfig::Instance());
	if (bRet == false)
	{
		LOG_ERROR("load center_config fail ");
		return false;
	}
	LOG_INFO("load config is:id:{}", m_uiServerID);
	// 读取配置信息
	if (CDataCfgMgr::Instance().Init() == false)
	{
		LOG_ERROR("init datamgr fail ");
		return false;
	}
	do
	{

		stIOHANDLER_DESC desc;
		desc.dwIoHandlerKey       = 0;
		desc.dwMaxAcceptSession   = 1000;
		desc.dwMaxConnectSession  = 0;
		desc.dwMaxConnectBuffSize = 1024*1024;
		desc.dwSendBufferSize     = SERVER_SOCKET_BUFF_SIZE;
		desc.dwRecvBufferSize     = SERVER_SOCKET_BUFF_SIZE;
		desc.dwTimeOut            = 60*60*24;
		desc.dwMaxPacketSize      = INNER_MAX_SIZE;
		desc.pool                 = new CCenterNetObj();
		desc.openMsgQueue         = true;

		if (!m_iocpServer.AddIoHandler(desc))
		{
			LOG_ERROR("IOCP Init fail");
			return false;
		}
		auto centerIp = m_solLua.get<sol::table>("server_config").get<sol::table>("center");
		if (!m_iocpServer.StartListen(0, "0.0.0.0", centerIp.get<int>("port")))
		{
			LOG_ERROR("IOCP SERVER StartListen fail {}", centerIp.get<int>("port"));
			return false;
		}

	} while (false);
	if (CCenterMgr::Instance().Init() == false)
	{
		LOG_ERROR("CenterMgr init fail");
		return false;
	}

	LOG_INFO("center server start is successed {}", m_uiServerID);
	return true;
}

void CApplication::ShutDown()
{

}

/**
* 本函数将在程序启动时和每次配置改变时被调用。
* 第一次调用将在Initialize()之前
*/
void CApplication::ConfigurationChanged()
{
	// 重加载配置
	LOG_INFO("configuration changed");
	CDataCfgMgr::Instance().Reload();
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
