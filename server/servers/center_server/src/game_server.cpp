/*
* game_server.cpp
*
*  modify on: 2018-5-23
*      Author: toney
*/
#include "game_define.h"
#include "framework/application.h"
#include "game_server_config.h"
#include "center_mgr.h"
#include "svrlib.h"
#include <iostream>
#include "utility/time_function.h"
#include "msg_define.pb.h"

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
	do
	{

		stIOHANDLER_DESC desc;
		desc.ioHandlerKey       = 0;
		desc.maxConnectBuffSize = 1024*1024;
		desc.sendBufferSize     = SERVER_SOCKET_BUFF_SIZE;
		desc.recvBufferSize     = SERVER_SOCKET_BUFF_SIZE;
		desc.timeOut            = 60*60*24;
		desc.maxPacketSize      = INNER_MAX_SIZE;
		desc.allocFunc          = [](){ return new CSvrCliNetObj(CCenterMgr::Instance()); };
		desc.openMsgQueue       = true;

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

}

void CApplication::Tick(uint64_t diffTime)
{

}
void CApplication::ExceptionHandle()
{

}

int main(int argc, char* argv[])
{
	return FrameworkMain(argc, argv);
}
