#include <data_cfg_mgr.h>
#include "lobby_mgr.h"
#include "game_server_config.h"
#include "game_define.h"
#include "net/msg_lobby_handle.h"
#include "player_mgr.h"

using namespace svrlib;
using namespace std;
using namespace Network;
using namespace net;

//大厅连接
CLobbyClient::CLobbyClient()
{

}
CLobbyClient::~CLobbyClient()
{

}


CLobbyMgr::CLobbyMgr()
		:m_timer(this)
{

}

void CLobbyMgr::OnTimer()
{
	ReportInfo2Lobby();
	CApplication::Instance().Schedule(&m_timer, 3000);
}

bool CLobbyMgr::Init()
{
	net::svr::server_info info;
	info.set_svrid(CApplication::Instance().GetServerID());
	info.set_game_type(0);
	info.set_game_subtype(0);
	info.set_svr_type(emSERVER_TYPE_GAME);
	info.set_uuid(CApplication::Instance().GetUUID());

	//连接大厅服
	auto lobbyIp = CApplication::Instance().GetSolLuaState().get<sol::table>("server_config").get<sol::table>("lobby");
	auto lobbyClient = std::make_shared<CLobbyClient>();
	if (lobbyClient->Init(info, lobbyIp.get<string>("ip"), lobbyIp.get<int>("in_port"),"lobby_connector",1) == false) {
		LOG_ERROR("init lobby client mgr fail");
		return false;
	}
    //添加派发消息handle
    lobbyClient->RegisterSubHandle(std::make_shared<CHandleLobbyMsg>());
	m_lobbySvrs.insert(make_pair(lobbyClient->GetSvrID(),lobbyClient));

	CApplication::Instance().Schedule(&m_timer, 3000);

	return true;
}

bool CLobbyMgr::SendMsg2Client(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin, uint16_t svrid)
{
	auto pSvr = GetLobbySvrBySvrID(svrid);
	if (pSvr != nullptr)
	{
		pSvr->SendMsg2Svr(msg,msg_type,uin,0,0,0,0);
		return true;
	}
	return false;
}

bool CLobbyMgr::SendMsg2Lobby(const google::protobuf::Message* msg, uint16_t msg_type, uint16_t svrid)
{
	auto pSvr = GetLobbySvrBySvrID(svrid);
	if (pSvr == nullptr)
		return false;
	pSvr->SendMsg2Svr(msg,msg_type,0,0,0,0,0);
	return true;
}

bool CLobbyMgr::SendMsg2AllLobby(const google::protobuf::Message* msg, uint16_t msg_type)
{
	for (auto& it : m_lobbySvrs)
	{
		auto pSvr = it.second;
		pSvr->SendMsg2Svr(msg, msg_type,0,0,0,0,0);
	}
	return true;
}

std::shared_ptr<CLobbyClient> CLobbyMgr::GetLobbySvrBySvrID(uint16_t svrid)
{
	auto it = m_lobbySvrs.find(svrid);
	if (it == m_lobbySvrs.end())return nullptr;
	return it->second;
}

void CLobbyMgr::ReportInfo2Lobby()
{
	net::svr::msg_report_svr_info info;

	uint32_t players = CPlayerMgr::Instance().GetOnlines();
	info.set_onlines(players);

	SendMsg2AllLobby(&info, net::svr::GS2L_MSG_REPORT);
}













