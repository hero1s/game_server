/*
* game_server.cpp
*
*  modify on: 2018-5-23
*      Author: toney
*/
#include "game_define.h"
#include "framework/application.h"
#include "gate_server_mgr.h"
#include "svrlib.h"
#include <iostream>
#include "utility/time_function.h"
#include "msg_define.pb.h"

using namespace svrlib;
using namespace std;

bool CApplication::Initialize()
{
	do
	{
		stIOHANDLER_DESC desc;
		desc.ioHandlerKey       = 0;
		desc.maxConnectBuffSize = 1024*1024;
		desc.sendBufferSize     = SERVER_SOCKET_BUFF_SIZE;
		desc.recvBufferSize     = SERVER_SOCKET_BUFF_SIZE;
		desc.timeOut            = 60*60*24;
		desc.maxPacketSize      = INNER_MAX_SIZE;
		desc.allocFunc          = [](){ return new CSvrCliNetObj(CGateSvrMgr::Instance()); };
		desc.openMsgQueue       = true;

		if (!m_iocpServer.AddIoHandler(desc))
		{
			LOG_ERROR("IOCP Init fail");
			return false;
		}
		auto centerIp = m_solLua.get<sol::table>("server_config").get<sol::table>("gate");
		if (!m_iocpServer.StartListen(0, "0.0.0.0", centerIp.get<int>("port")))
		{
			LOG_ERROR("IOCP SERVER StartListen fail {}", centerIp.get<int>("port"));
			return false;
		}

	} while (false);
	if (CGateSvrMgr::Instance().Init() == false)
	{
		LOG_ERROR("CenterMgr init fail");
		return false;
	}

	LOG_INFO("gate server start is successed {}", m_uiServerID);

	return true;
}

void CApplication::ShutDown()
{

}

/**
* ���������ڳ�������ʱ��ÿ�����øı�ʱ�����á�
* ��һ�ε��ý���Initialize()֮ǰ
*/
void CApplication::ConfigurationChanged()
{
	// �ؼ�������
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