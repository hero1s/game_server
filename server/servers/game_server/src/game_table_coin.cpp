//
// Created by Administrator on 2018/6/13.
//

#include "game_table_coin.h"
#include "stdafx.h"
#include "game_room.h"
#include "dbmysql_mgr.h"
#include "lobby_mgr.h"
#include "data_cfg_mgr.h"

using namespace svrlib;
using namespace std;
using namespace net;

CGameCoinTable::CGameCoinTable(CGameRoom* pRoom, int64_t tableID)
		:CGameTable(pRoom, tableID, emTABLE_TYPE_SYSTEM)
{

}

CGameCoinTable::~CGameCoinTable()
{

}
void CGameCoinTable::OnProccess()
{
	OnTimeTick();
	CApplication::Instance().schedule(&m_timer, 500);
}
//游戏消息
int CGameCoinTable::OnMessage(CGamePlayer* pPlayer, uint16_t cmdID, const uint8_t* pkt_buf, uint16_t buf_len)
{

	return OnGameMessage(pPlayer, cmdID, pkt_buf, buf_len);
}

bool CGameCoinTable::EnterTable(CGamePlayer* pPlayer)
{
	if (CanEnterTable(pPlayer) != net::RESULT_CODE_SUCCESS)
		return false;
	if (NeedSitDown())
	{//需要手动坐下站起
		pPlayer->SetTable(this);
		LOG_TAB_DEBUG("进入桌子：room:{}--tb:{},uid:{}", m_pHostRoom->GetRoomID(), GetTableID(), pPlayer->GetUID());
		OnPlayerJoin(true, 0, pPlayer);

		pPlayer->NotifyLobbyChangePlayStatus();
		return true;
	}
	else
	{//自动坐下
		for (uint8_t i = 0; i < m_vecPlayers.size(); ++i)
		{
			auto& seat = m_vecPlayers[i];
			if (seat.pPlayer == NULL)
			{
				SitDown(seat, pPlayer);
				pPlayer->SetTable(this);
				LOG_TAB_DEBUG("进入桌子：room:{}--tb:{},uid:{}", m_pHostRoom->GetRoomID(), GetTableID(), pPlayer->GetUID());
				OnPlayerJoin(true, i, pPlayer);
				if (pPlayer->IsAutoReady())
				{
					PlayerReady(pPlayer, true);
				}

				pPlayer->NotifyLobbyChangePlayStatus();
				return true;
			}
		}
	}
	LOG_TAB_ERROR("进入桌子失败:{}", pPlayer->GetUID());
	return false;
}

bool CGameCoinTable::LeaveTable(CGamePlayer* pPlayer, bool bNotify, uint8_t leaveType)
{
	if (pPlayer->GetTable() != this)
	{
		LOG_TAB_ERROR("error table:{}--{}", pPlayer->GetTableID(), this->GetTableID());
		return false;
	}
	if (NeedSitDown())
	{   //需要手动坐下站起
		for (uint8_t i = 0; i < m_vecPlayers.size(); ++i)
		{
			if (m_vecPlayers[i].pPlayer == pPlayer)
			{
				m_vecPlayers[i].Reset();

				OnActionUserStandUp(i, pPlayer);
				OnPlayerJoin(false, i, pPlayer);

				pPlayer->SetTable(NULL);
				LOG_TAB_DEBUG("离开桌子:room:{}--tb:{},uid:{}", m_pHostRoom->GetRoomID(), GetTableID(), pPlayer->GetUID());
				if (bNotify)
				{
					NotifyPlayerLeave(pPlayer, leaveType);
				}
				pPlayer->NotifyLobbyChangePlayStatus();
				return true;
			}
		}
		LOG_TAB_DEBUG("旁观者离开桌子:{}", pPlayer->GetUID());
		OnPlayerJoin(false, 0, pPlayer);
		pPlayer->SetTable(NULL);
		if (bNotify)
		{
			NotifyPlayerLeave(pPlayer, leaveType);
		}
		pPlayer->NotifyLobbyChangePlayStatus();
		return true;
	}
	else
	{
		for (uint8_t i = 0; i < m_vecPlayers.size(); ++i)
		{
			if (m_vecPlayers[i].pPlayer == pPlayer)
			{
				m_vecPlayers[i].Reset();
				pPlayer->SetTable(NULL);
				LOG_TAB_DEBUG("离开桌子:room:{}--tb:{},uid:{}", m_pHostRoom->GetRoomID(), GetTableID(), pPlayer->GetUID());

				OnPlayerJoin(false, i, pPlayer);
				if (bNotify)
				{
					NotifyPlayerLeave(pPlayer, leaveType);
				}
				pPlayer->NotifyLobbyChangePlayStatus();
				return true;
			}
		}
	}
	LOG_TAB_ERROR("离开桌子失败:{}", pPlayer->GetUID());
	return false;
}

int32_t CGameCoinTable::CanEnterTable(CGamePlayer* pPlayer)
{
	if (pPlayer->GetTable() != NULL)
	{
		LOG_TAB_ERROR("玩家已存在桌子或者桌子已关闭");
		return net::RESULT_CODE_FAIL;
	}
	if (IsFullTable() /*|| GetGameState() != TABLE_STATE_FREE*/)
	{
		LOG_TAB_ERROR("桌子已满");
		return net::RESULT_CODE_TABLE_FULL;
	}
	// 限额进入
	if (GetPlayerCurScore(pPlayer) < GetEnterMin())
	{
		LOG_TAB_ERROR("积分不够进入:{}--{}", GetPlayerCurScore(pPlayer), GetEnterMin());
		return net::RESULT_CODE_CION_ERROR;
	}

	return net::RESULT_CODE_SUCCESS;
}

bool CGameCoinTable::CanLeaveTable(CGamePlayer* pPlayer)
{
	//旁观者可以离开(线上百人场考虑下注与否toney)
	if (IsExistLooker(pPlayer->GetUID()) && IsExistPlayer(pPlayer->GetUID()) == false)return true;

	uint16_t chairID = GetChairID(pPlayer);
	//金币场没有参与即可离开
	if (GetPlayStatus(chairID) == 0)
		return true;

	//非关闭状态桌子只能空闲状态才能离开
	if (GetGameState() != TABLE_STATE_FREE)
	{
		//LOG_TAB_DEBUG("非关闭状态桌子只能空闲才能离开:{}", pPlayer->GetUID());
		return false;
	}

	return false;
}
// 是否参与游戏
bool CGameCoinTable::IsJoinPlay(uint16_t chairID)
{
	if (chairID >= m_vecPlayers.size())return false;
	auto& seat = m_vecPlayers[chairID];
	if (seat.pPlayer == NULL)return false;
	if (seat.playStatus == 0)
	{
		return false;
	}

	return true;
}
bool CGameCoinTable::CanSitDown(CGamePlayer* pPlayer, uint16_t chairID)
{
	if (GetPlayerCurScore(pPlayer) < m_pHostRoom->GetSitDown())
	{
		LOG_TAB_DEBUG("the sitdown condition not have :{}", m_pHostRoom->GetSitDown());
		return false;
	}
	if (!IsExistLooker(pPlayer->GetUID()))
	{
		LOG_TAB_DEBUG("not in looklist:{}", pPlayer->GetUID());
		return false;
	}
	if (chairID >= m_vecPlayers.size())
	{
		LOG_TAB_DEBUG("the seat is more big:{}", chairID);
		return false;
	}
	if (m_vecPlayers[chairID].pPlayer != NULL)
	{
		LOG_TAB_DEBUG("the seat is other player");
		return false;
	}

	return true;
}

bool CGameCoinTable::CanStandUp(CGamePlayer* pPlayer)
{
	if (CanLeaveTable(pPlayer))
		return true;

	return false;
}

bool CGameCoinTable::NeedSitDown()
{
	return false;
}

bool CGameCoinTable::PlayerReady(CGamePlayer* pPlayer, bool bReady)
{
	if (GetPlayerCurScore(pPlayer) < GetEnterMin())
	{
		LOG_TAB_DEBUG("can't ready {}", pPlayer->GetUID());
		return false;
	}
	for (uint8_t i = 0; i < m_vecPlayers.size(); ++i)
	{
		auto& seat = m_vecPlayers[i];
		if (seat.pPlayer == pPlayer)
		{
			seat.readyStatus = bReady ? 1 : 0;
			seat.autoStatus  = 0;
			seat.readyTime   = getSysTime();
			seat.overTimes   = 0;
			SendReadyStateToClient();
			OnActionUserOnReady(i, pPlayer);
			return true;
		}
	}
	LOG_TAB_ERROR("player ready fail :{}--{}", pPlayer->GetUID(), GetTableID());
	return false;
}
//用户断线或重连
bool CGameCoinTable::OnActionUserNetState(CGamePlayer* pPlayer, bool bConnected, bool isJoin)
{
	if (bConnected)//断线重连
	{
		if (isJoin)
		{
			pPlayer->SetPlayDisconnect(false);
			SendTableInfoToClient(pPlayer);
			SendSeatInfoToClient();

			if (IsExistLooker(pPlayer->GetUID()))
			{
				NotifyPlayerJoin(pPlayer, true);
			}
			SendLookerListToClient(pPlayer);
			SendGameScene(pPlayer);
			SendReadyStateToClient();
		}
	}
	else
	{
		pPlayer->SetPlayDisconnect(true);
		SendSeatInfoToClient();
		if (IsExistLooker(pPlayer->GetUID()))
		{
			pPlayer->TryLeaveCurTable();
		}
		SendReadyStateToClient();
	}

	LOG_TAB_DEBUG("OnAction Net State:{}--{}--{}", pPlayer->GetUID(), bConnected, isJoin);

	return true;
}

//用户坐下
bool CGameCoinTable::OnActionUserSitDown(uint16_t wChairID, CGamePlayer* pPlayer)
{
	SendSeatInfoToClient();
	return true;
}

//用户起立
bool CGameCoinTable::OnActionUserStandUp(uint16_t wChairID, CGamePlayer* pPlayer)
{
	SendSeatInfoToClient();
	return true;
}

//用户同意
bool CGameCoinTable::OnActionUserOnReady(uint16_t wChairID, CGamePlayer* pPlayer)
{
	return true;
}

//玩家进入或离开
void CGameCoinTable::OnPlayerJoin(bool isJoin, uint16_t chairID, CGamePlayer* pPlayer)
{
	if (isJoin)
	{
		SendTableInfoToClient(pPlayer);
	}
	if (NeedSitDown())
	{
		if (isJoin)
		{
			AddLooker(pPlayer);
			SendLookerListToClient(pPlayer);
		}
		else
		{
			RemoveLooker(pPlayer);
		}
	}
	SendSeatInfoToClient();
	SendReadyStateToClient();
	if (isJoin)
	{
		CDBMysqlMgr::Instance().UpdatePlayerOnlinePlayInfo(pPlayer->GetUID(), -1, GetTableID());
	}
	else
	{
		CDBMysqlMgr::Instance().UpdatePlayerOnlinePlayInfo(pPlayer->GetUID(), -1, 0);
	}
}
// 是否需要回收
bool CGameCoinTable::NeedRecover()
{

	return false;
}

bool CGameCoinTable::WantNeedRecover()
{

	return false;
}
//没积分的玩家自动站起
void CGameCoinTable::StandUpNotScore()
{
	for (auto& seat : m_vecPlayers)
	{
		CGamePlayer* pPlayer = seat.pPlayer;
		if (pPlayer != NULL && GetPlayerCurScore(pPlayer) < GetEnterMin())
		{
			LeaveTable(pPlayer, true, 1);
		}
	}
}
// 扣除开始台费
void CGameCoinTable::DeductStartFee(bool bNeedReady)
{
	LOG_TAB_DEBUG("Deduct Start Fee");
	if (m_conf.feeType == TABLE_FEE_TYPE_ALLBASE)
	{
		int64_t       fee = -(m_conf.baseScore*m_conf.feeValue/PRO_DENO_10000);
		for (uint32_t i   = 0; i < m_vecPlayers.size(); ++i)
		{
			CGamePlayer* pPlayer = GetPlayer(i);
			if (pPlayer == NULL || (bNeedReady && m_vecPlayers[i].readyStatus == 0))
				continue;

			ChangeScoreValueByUID(pPlayer->GetUID(), fee, emACCTRAN_OPER_TYPE_FEE, GetTableID());
			ChangeUserBlingLogFee(pPlayer->GetUID(), fee);
		}
		FlushSeatValueInfoToClient();
	}
}

// 扣除结算台费
void CGameCoinTable::DeducEndFee(uint32_t uid, int64_t& winScore)
{
	LOG_TAB_DEBUG("Deduc End Fee");
	if (m_conf.feeType == TABLE_FEE_TYPE_WIN)
	{
		if (winScore > 0)
		{
			int64_t fee = -(winScore*m_conf.feeValue/PRO_DENO_10000);
			winScore += fee;
			ChangeUserBlingLogFee(uid, fee);
		}
		else
		{
			int64_t fee = -(winScore*m_conf.feeValue/PRO_DENO_10000);
		}
	}
}

// 上报游戏战报
void CGameCoinTable::ReportGameResult(uint32_t uid, int64_t winScore)
{
	net::msg_report_game_result msg;
	msg.set_uid(uid);
	msg.set_consume(GetConsumeType());
	msg.set_game_type(m_pHostRoom->GetGameType());
	msg.set_win_score(winScore);
	CLobbyMgr::Instance().SendMsg2Client(&msg, net::S2L_MSG_REPORT_GAME_RESULT, uid, 0);
}

// 结算玩家信息
void CGameCoinTable::CalcPlayerGameInfo(uint32_t uid, int64_t winScore)
{
	LOG_TAB_DEBUG("calc player game info:{}  {}", uid, winScore);
	ReportGameResult(uid, winScore);
	CGamePlayer* pPlayer = (CGamePlayer*) CPlayerMgr::Instance().GetPlayer(uid);
	if (pPlayer != NULL)
	{
		//抽水
		DeducEndFee(uid, winScore);
		//修改积分
		ChangeScoreValueByUID(uid, winScore, emACCTRAN_OPER_TYPE_GAME, m_pHostRoom->GetGameType());
		//刷新日志计分
		ChangeUserBlingLog(pPlayer, winScore);
	}
}
int64_t CGameCoinTable::ChangeScoreValueByUID(uint32_t uid, int64_t& score, uint16_t operType, uint16_t subType)
{
	uint8_t consumeType = GetConsumeType();
	CGamePlayer* pGamePlayer = (CGamePlayer*) CPlayerMgr::Instance().GetPlayer(uid);
	switch (consumeType)
	{
	case net::ROOM_CONSUME_TYPE_COIN:
	{
		if (pGamePlayer != NULL)
		{
			pGamePlayer->SyncChangeAccountValue(operType, subType, score, 0, m_chessid);
		}
		else
		{
			CLobbyMgr::Instance().NotifyLobbyChangeAccValue(uid, operType, subType, score, 0, m_chessid);
		}
		break;
	}
	default:
		break;
	}

	return score;
}


























































