#include <data_cfg_mgr.h>
#include "lobby_mgr.h"
#include "game_server_config.h"
#include "game_define.h"
#include "stdafx.h"

#include "net/msg_lobby_handle.h"

using namespace svrlib;
using namespace std;
using namespace Network;
using namespace net;

CLobbyMgr::CLobbyMgr()
		:m_timer(this)
{

}

void CLobbyMgr::OnTimer()
{
	ReportInfo2Lobby();
	CApplication::Instance().schedule(&m_timer, 3000);
}

bool CLobbyMgr::Init()
{
	auto refCfg = CDataCfgMgr::Instance().GetCurSvrCfg();

	vector<stGameServerCfg> vecLobbyCfg;
	CDataCfgMgr::Instance().GetSvrsCfgByType(refCfg.group, emSERVER_TYPE_LOBBY, vecLobbyCfg);
	if (vecLobbyCfg.empty())
	{
		LOG_ERROR("lobby svr config not find :group:{}", refCfg.group);
		return false;
	}
	LOG_ERROR("lobby svr config :group:{}--lobbysize:{}", refCfg.group, vecLobbyCfg.size());
	auto iocpServer = CApplication::Instance().GetIOCPServer();
	for (uint32_t i = 0; i < vecLobbyCfg.size(); ++i)
	{
		stGameServerCfg & lobbycfg = vecLobbyCfg[i];
		CLobbyNetObj* pNetObj  = new CLobbyNetObj();
		pNetObj->SetUID(lobbycfg.svrid);
		pNetObj->Init(&iocpServer, 0, lobbycfg.svrlanip, lobbycfg.svrlanport);

		stLobbyServer lobbySvr;
		lobbySvr.svrID   = lobbycfg.svrid;
		lobbySvr.isRun   = false;
		lobbySvr.pNetObj = pNetObj;

		m_lobbySvrs.insert(make_pair(lobbySvr.svrID, lobbySvr));
	}

	CApplication::Instance().schedule(&m_timer, 3000);

	return true;
}

void CLobbyMgr::Register(uint16_t svrid)
{
	net::msg_register_svr msg;

	auto curSvrCfg = CDataCfgMgr::Instance().GetCurSvrCfg();
	msg.set_svr_type(curSvrCfg.svrType);
	msg.set_svrid(CApplication::Instance().GetServerID());
	msg.set_game_type(curSvrCfg.gameType);
	msg.set_game_subtype(curSvrCfg.gameSubType);
	for (auto play:curSvrCfg.openPlays)
	{
		msg.add_play_types(play);
	}
	SendMsg2Lobby(&msg, net::S2L_MSG_REGISTER, svrid, 1);
	LOG_DEBUG("Register GameServer svrid:{}--gameType:{}", msg.svrid(), msg.game_type());
}

void CLobbyMgr::RegisterRep(uint16_t svrid, bool rep)
{
	LOG_DEBUG("Register GameServer Rep svrid:{}--res:{}", svrid, rep);
	stLobbyServer* pSvr = GetLobbySvrBySvrID(svrid);
	pSvr->isRun = rep;
}

bool CLobbyMgr::SendMsg2Client(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin, uint32_t svrid)
{
	stLobbyServer* pSvr = GetLobbySvrBySvrID(svrid);
	if (pSvr != NULL)
	{
		return SendInnerMsg(pSvr->pNetObj, msg, msg_type, uin);
	}
	else
	{
		stLobbyServer* pSvr = GetLobbySvrByUin(uin);
		if (pSvr == NULL || !pSvr->isRun)
			return false;
		return SendInnerMsg(pSvr->pNetObj, msg, msg_type, uin);
	}
}

bool CLobbyMgr::SendMsg2Lobby(const google::protobuf::Message* msg, uint16_t msg_type, uint16_t svrid, uint8_t encrypt)
{
	stLobbyServer* pSvr = GetLobbySvrBySvrID(svrid);
	if (pSvr == NULL)
		return false;
	return SendInnerMsg(pSvr->pNetObj, msg, msg_type, 0);
}

bool CLobbyMgr::SendMsg2AllLobby(const google::protobuf::Message* msg, uint16_t msg_type, uint8_t encrypt)
{
	for (auto& it : m_lobbySvrs)
	{
		stLobbyServer* pSvr = &it.second;
		if (pSvr->isRun)
		{
			SendInnerMsg(pSvr->pNetObj, msg, msg_type, 0);
		}
	}
	return true;
}

void CLobbyMgr::OnConnect(bool bSuccess, CLobbyNetObj* pNetObj)
{
	LOG_ERROR("lobby on connect {},{}", bSuccess, pNetObj->GetUID());
	stLobbyServer* pSvr = GetLobbySvrBySvrID(pNetObj->GetUID());
	if (bSuccess)
	{
		pSvr->isRun = true;
		Register(pSvr->svrID);
	}
}

void CLobbyMgr::OnCloseClient(CLobbyNetObj* pNetObj)
{
	LOG_ERROR("lobby OnClose:{}", pNetObj->GetUID());
	stLobbyServer* pSvr = GetLobbySvrBySvrID(pNetObj->GetUID());
	pSvr->isRun = false;
}

// 请求大厅修改数值
void CLobbyMgr::NotifyLobbyChangeAccValue(uint32_t uid, int32_t operType, int32_t subType, int64_t coin, int64_t safeCoin,
                                          const string& chessid)
{
	net::msg_notify_change_account_data msg;
	msg.set_uid(uid);
	msg.set_coin(coin);
	msg.set_safe_coin(safeCoin);
	msg.set_oper_type(operType);
	msg.set_sub_type(subType);
	msg.set_chessid(chessid);

	SendMsg2Client(&msg, S2L_MSG_NOTIFY_CHANGE_ACCOUNT_DATA, uid, 0);
}

void CLobbyMgr::FlushRoomCardValueToLobby(uint32_t uid, int64_t roomCard)
{
	net::msg_flush_room_card_data msg;
	msg.set_uid(uid);
	msg.set_room_card(roomCard);

	SendMsg2Client(&msg, S2L_MSG_FLUSH_ROOM_CARD_DATA, uid, 0);
}
// 刷新俱乐部房卡
void CLobbyMgr::FlushClubInfo(uint32_t uid,int64_t clubID, int64_t roomCard, int64_t forze)
{
	if (clubID == 0)return;

	net::msg_flush_club_info_rep msg;
	msg.set_club_id(clubID);
	msg.set_room_card(roomCard);
	msg.set_frozen_card(forze);

	CCenterClientMgr::Instance().SendBroadCastMsg(&msg, S2C_MSG_FLUSH_CLUB_INFO_REP, clubID);
	CRedisMgr::Instance().SetMasterRoomCard(uid,roomCard,forze);
}
// 变更房卡
bool CLobbyMgr::AtomChangePlayerRoomCard(uint32_t uid, uint16_t operType, uint16_t subType, int64_t roomCard)
{
	int64_t curVal = 0;
	if (CCommonLogic::AtomChangePlayerRoomCard(uid, operType, subType, roomCard, curVal) == false)
	{
		LOG_ERROR("{} roomcard not enough :{}--{}", uid, roomCard, curVal);
		return false;
	}
	else
	{
		FlushRoomCardValueToLobby(uid, curVal);
	}
	auto pGamePlayer = CPlayerMgr::Instance().GetPlayer(uid);
	if (pGamePlayer != NULL)
	{
		pGamePlayer->SetAccountValue(emACC_VALUE_ROOMCARD, curVal);
	}
	return true;
}
// 冻结房卡
bool CLobbyMgr::FrozenPlayerRoomCard(uint32_t uid, int64_t clubID, int64_t roomCard)
{
	int64_t curVal = 0, forze = 0;
	if (CCommonLogic::FrozenPlayerRmcard(uid, roomCard, curVal, forze) == false)
	{
		LOG_ERROR("{} roomcard not enough :{}--{}", uid, roomCard, curVal);
		return false;
	}
	else
	{
		FlushRoomCardValueToLobby(uid, curVal);
		FlushClubInfo(uid,clubID, curVal, forze);
	}
	auto pGamePlayer = CPlayerMgr::Instance().GetPlayer(uid);
	if (pGamePlayer != NULL)
	{
		pGamePlayer->SetAccountValue(emACC_VALUE_ROOMCARD, curVal);
	}
	return true;
}
bool CLobbyMgr::UnfrozenPlayerRoomCard(uint32_t uid, int64_t clubID, int64_t roomCard)
{
	int64_t curVal = 0, forze = 0;
	if (CCommonLogic::UnfrozePlayerRmcard(uid, roomCard, curVal, forze) == false)
	{
		LOG_ERROR("{} unfrozen roomcard not enough :{}--{}", uid, roomCard, curVal);
		return false;
	}
	else
	{
		FlushRoomCardValueToLobby(uid, curVal);
		FlushClubInfo(uid,clubID, curVal, forze);
	}
	auto pGamePlayer = CPlayerMgr::Instance().GetPlayer(uid);
	if (pGamePlayer != NULL)
	{
		pGamePlayer->SetAccountValue(emACC_VALUE_ROOMCARD, curVal);
	}
	return true;
}
bool CLobbyMgr::MoveFrozenPlayerRoomCard(uint32_t uid, int64_t clubID, uint16_t operType, uint16_t subType, int64_t roomCard)
{
	int64_t curVal = 0, forze = 0;
	if (CCommonLogic::MoveFrozePlayerRmcard(uid, operType, subType, roomCard, curVal, forze) == false)
	{
		LOG_ERROR("{} move roomcard not enough :{}", uid, roomCard);
		return false;
	}
	FlushClubInfo(uid,clubID, curVal, forze);
	return true;
}

stLobbyServer* CLobbyMgr::GetLobbySvrBySvrID(uint16_t svrid)
{
	auto it = m_lobbySvrs.find(svrid);
	if (it == m_lobbySvrs.end())return NULL;
	return &it->second;
}

stLobbyServer* CLobbyMgr::GetLobbySvrByUin(uint32_t uin)
{
	CGamePlayer* pPlayer = (CGamePlayer*) CPlayerMgr::Instance().GetPlayer(uin);
	if (pPlayer == NULL)
	{
		uint16_t svrid = CRedisMgr::Instance().GetPlayerOnlineInfo(uin, "lsid");
		LOG_DEBUG("GetLobbySvrIDByUID:{}--{}", uin, svrid);
		if (svrid > 0)
		{
			return GetLobbySvrBySvrID(svrid);
		}
		else
		{
			auto it = m_lobbySvrs.begin();
			return &it->second;
		}
	}
	else
	{
		return GetLobbySvrBySvrID(pPlayer->GetLoginLobbySvrID());
	}

}

void CLobbyMgr::ReportInfo2Lobby()
{
	net::msg_report_svr_info info;

	uint32_t players = CPlayerMgr::Instance().GetOnlines();
	info.set_onlines(players);

	SendMsg2AllLobby(&info, net::S2L_MSG_REPORT);
}













