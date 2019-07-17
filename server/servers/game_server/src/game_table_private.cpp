//
// Created by toney on 2018/6/11.
//

#include "game_table_private.h"
#include "stdafx.h"
#include "game_room.h"
#include "dbmysql_mgr.h"
#include "lobby_mgr.h"
#include "data_cfg_mgr.h"
#include <math.h>

using namespace svrlib;
using namespace std;
using namespace net;

namespace
{
static const uint32_t s_MaxPlayTime = SECONDS_IN_ONE_HOUR*3;//最多游戏时间3小时
}

CGamePrivateTable::CGamePrivateTable(CGameRoom* pRoom, int64_t tableID)
		:CGameTable(pRoom, tableID, emTABLE_TYPE_PLAYER)
{
	m_agreeCooling.clearCool();
	m_agreeID        = 0;
	m_voteEnd        = true;
	m_voteDiss       = false;
	m_forceDiss      = false;
	m_logicDiss      = false;
	m_canJoinPlaying = false;
	m_round          = 0;
	m_bClosed        = false;
	m_startTime      = 0;
	m_endTime        = 0;
	m_createTime     = 0;
	m_needOpenRecord = true;

	m_scoreSetting.Reset();
}

CGamePrivateTable::~CGamePrivateTable()
{

}
bool CGamePrivateTable::Init()
{

	return true;
}
void CGamePrivateTable::ShutDown()
{
	FlushClubPrivateListToClient(true);
}
void CGamePrivateTable::OnProccess()
{
	if (!m_bClosed)
	{
		if (m_agreeCooling.isTimeOut())
		{
			OnTimeTick();
		}
		CheckPrivateGameOver();
	}
	CheckAutoLeaveTable();

	CApplication::Instance().schedule(&m_timer, 100);
}
//游戏消息
int CGamePrivateTable::OnMessage(CGamePlayer* pPlayer, uint16_t cmdID, const uint8_t* pkt_buf, uint16_t buf_len)
{
	switch (cmdID)
	{
	case net::C2S_MSG_PRIVATE_GAMEOVER_REQ:
	{
		net::msg_private_gameover_req msg;
		PARSE_MSG_FROM_ARRAY(msg);
		LOG_TAB_DEBUG("私人房投票:{}--{}--{}", pPlayer->GetUID(), msg.is_agree(), msg.req_id());
		VoteGameOver(msg.reason(), pPlayer->GetUID(), msg.req_id(), msg.is_agree());
		return 0;
	}

	}
	return OnGameMessage(pPlayer, cmdID, pkt_buf, buf_len);
}

bool CGamePrivateTable::EnterTable(CGamePlayer* pPlayer)
{
	if (CanEnterTable(pPlayer) != net::RESULT_CODE_SUCCESS)
		return false;
	if (m_conf.rentType == 1)
	{
		if (!PayAARoomCard(pPlayer))return false;
	}
	if (NeedSitDown())
	{//需要手动坐下站起
		pPlayer->SetTable(this);
		LOG_TAB_DEBUG("进入桌子：room:{},uid:{}", m_pHostRoom->GetRoomID(), pPlayer->GetUID());
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
				LOG_TAB_DEBUG("进入桌子：room:{},uid:{}", m_pHostRoom->GetRoomID(), pPlayer->GetUID());
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
	if (m_conf.rentType == 1)
	{
		BackPayAARoomCard(pPlayer);
	}
	return false;
}

bool CGamePrivateTable::LeaveTable(CGamePlayer* pPlayer, bool bNotify, uint8_t leaveType)
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
				LOG_TAB_DEBUG("离开桌子:room:{},uid:{}", m_pHostRoom->GetRoomID(), pPlayer->GetUID());
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
				LOG_TAB_DEBUG("离开桌子:room:{},uid:{}", m_pHostRoom->GetRoomID(), pPlayer->GetUID());

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
int32_t CGamePrivateTable::CanEnterTable(CGamePlayer* pPlayer)
{
	if (pPlayer->GetTable() != NULL || m_bClosed)
	{
		LOG_TAB_ERROR("玩家已存在桌子或者桌子已关闭");
		return net::RESULT_CODE_FAIL;
	}
	if (IsFullTable() /*|| GetGameState() != TABLE_STATE_FREE*/)
	{
		LOG_TAB_ERROR("桌子已满");
		return net::RESULT_CODE_TABLE_FULL;
	}
	if (IsVoteStatus())
	{
		LOG_TAB_ERROR("投票状态不能进入");
		return net::RESULT_CODE_VOTE_STATUS;
	}

	if (m_mpPlayerScoreInfo.size() >= m_conf.seatNum)
	{
		if (!IsExistPrivatePlayerInfo(pPlayer->GetUID()))
		{
			LOG_TAB_ERROR("桌子已满，并且不存在自己的ID");
			return net::RESULT_CODE_TABLE_FULL;
		}
	}
	if (!IsExistPrivatePlayerInfo(pPlayer->GetUID()))
	{
		if (!m_canJoinPlaying && IsStartGame())
		{
			LOG_TAB_DEBUG("禁止中途加入");
			return net::RESULT_CODE_FAIL;
		}
	}
	//积分场最小带入
	if (m_conf.createType == emPRIVATE_CREATE_SCORE && pPlayer->GetClubScore() < GetEnterMin())
	{
		LOG_TAB_ERROR("积分不够进入:{}--{}", pPlayer->GetClubScore(), GetEnterMin());
		return net::RESULT_CODE_CION_ERROR;
	}

	return net::RESULT_CODE_SUCCESS;
}

bool CGamePrivateTable::CanLeaveTable(CGamePlayer* pPlayer)
{
	//旁观者可以离开
	if (IsExistLooker(pPlayer->GetUID()) && IsExistPlayer(pPlayer->GetUID()) == false)return true;

	uint16_t chairID = GetChairID(pPlayer);

	if (m_bClosed)
	{
		return true;
	}
	else
	{
		if (IsVoteStatus())
		{
			LOG_TAB_ERROR("投票状态不能离开");
			return false;
		}
		//没有参与可以离开
		if (!IsJoinPlay(chairID) && pPlayer->GetUID() != GetHostID())
		{
			LOG_TAB_DEBUG("没有参与游戏,可以离开:{}", pPlayer->GetUID());
			return true;
		}
		if (IsStartGame() || (GetClubID() == 0 && pPlayer->GetUID() == GetHostID()))
		{
			//LOG_TAB_DEBUG("开局了或者房主不能离开:{}", pPlayer->GetUID());
			return false;
		}
	}

	//非关闭状态桌子只能空闲状态才能离开
	if (GetGameState() != TABLE_STATE_FREE)
	{
		//LOG_TAB_DEBUG("非关闭状态桌子只能空闲才能离开:{}", pPlayer->GetUID());
		//return false;
	}

	return true;
}
// 是否参与游戏
bool CGamePrivateTable::IsJoinPlay(uint16_t chairID)
{
	if (chairID >= m_vecPlayers.size())return false;
	auto& seat = m_vecPlayers[chairID];
	if (seat.pPlayer == NULL)return false;
	if (seat.playStatus == 0 && !IsExistPrivatePlayerInfo(seat.uid))
	{
		return false;
	}

	return true;
}
bool CGamePrivateTable::CanSitDown(CGamePlayer* pPlayer, uint16_t chairID)
{
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

bool CGamePrivateTable::CanStandUp(CGamePlayer* pPlayer)
{
	if (CanLeaveTable(pPlayer))
		return true;

	return false;
}

bool CGamePrivateTable::NeedSitDown()
{
	return false;
}

bool CGamePrivateTable::PlayerReady(CGamePlayer* pPlayer, bool bReady)
{
	LOG_TAB_DEBUG("玩家准备:{}--{}", pPlayer->GetUID(), bReady);
	if (IsScoreMode() && !CanMinus() && GetPlayerCurScore(pPlayer) <= 0)
	{
		LOG_TAB_DEBUG("积分太少无法准备uid {}--cur {}--min {}", pPlayer->GetUID(), GetPlayerCurScore(pPlayer), GetEnterMin());
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
			FlushClubPrivateListToClient();
			return true;
		}
	}
	LOG_TAB_ERROR("player ready fail :{}", pPlayer->GetUID());
	return false;
}
//用户断线或重连
bool CGamePrivateTable::OnActionUserNetState(CGamePlayer* pPlayer, bool bConnected, bool isJoin)
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
			if (m_agreeID != 0)
			{
				NotifyVoteGameOver();
			}
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
bool CGamePrivateTable::OnActionUserSitDown(uint16_t wChairID, CGamePlayer* pPlayer)
{
	SendSeatInfoToClient();
	return true;
}

//用户起立
bool CGamePrivateTable::OnActionUserStandUp(uint16_t wChairID, CGamePlayer* pPlayer)
{
	SendSeatInfoToClient();
	return true;
}

//用户同意
bool CGamePrivateTable::OnActionUserOnReady(uint16_t wChairID, CGamePlayer* pPlayer)
{
	return true;
}

//玩家进入或离开
void CGamePrivateTable::OnPlayerJoin(bool isJoin, uint16_t chairID, CGamePlayer* pPlayer)
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
	FlushClubPrivateListToClient();
	if (isJoin)
	{
		CDBMysqlMgr::Instance().UpdatePlayerOnlinePlayInfo(pPlayer->GetUID(), -1, GetTableID());
		if (IsScoreMode())
		{
			ResetClubScore(pPlayer);
		}
	}
	else
	{
		CDBMysqlMgr::Instance().UpdatePlayerOnlinePlayInfo(pPlayer->GetUID(), -1, 0);
		if (IsScoreMode())
		{//解冻积分
			pPlayer->ClearClubScore();
		}
		if (!m_bClosed && m_conf.rentType == 1)
		{
			BackPayAARoomCard(pPlayer);
		}
		if (!IsStartGame())
		{//未开始,删除积分信息
			AutoDelNoSeatPrivatePlayerInfo();
		}
	}
	FlushPrivateTableInfoToCache();
}
// 是否需要回收
bool CGamePrivateTable::NeedRecover()
{
	if (m_bClosed == true)
	{
		if (IsEmptyTable())
		{
			return true;
		}
	}
	return false;
}

bool CGamePrivateTable::WantNeedRecover()
{
	if (m_bClosed == true)
	{
		return true;
	}
	return false;
}
void CGamePrivateTable::LoadPrivateTable(stPrivateTable& data)
{
	m_conf.baseScore  = data.baseScore;
	m_conf.consume    = data.consume;
	m_conf.dueTime    = data.dueTime;
	m_conf.dueround   = data.dueround;
	m_conf.enterMin   = data.enterMin;
	m_conf.feeType    = 0;
	m_conf.feeValue   = 0;
	m_conf.seatNum    = data.seatNum;
	m_conf.playType   = data.playType;
	m_conf.addParam   = data.addParam;
	m_conf.matchParam = data.matchParam;
	m_conf.enterMin   = data.enterMin;
	m_conf.consume    = data.consume;
	m_conf.rentType   = data.rentType;
	m_conf.rentFee    = data.fee;
	m_conf.clubID     = data.clubID;      // 俱乐部ID
	m_conf.clubFloor  = data.clubFloor;
	m_conf.clubPos    = data.clubPos;     // 俱乐部桌子位置
	m_conf.idxRound   = data.idxRound;

	m_conf.hostID = data.hostID;
	m_conf.passwd = data.passwd;
	m_tableID    = data.tableID;
	m_createTime = data.createTime;
	m_conf.createType = data.createType;

	json jvalue;
	LOG_TAB_DEBUG("Load gameInfo:{}", data.gameInfo);
	if (ParseJsonFromString(jvalue, data.gameInfo))
	{
		try
		{
			for (uint8_t i = 0; i < jvalue["user"].size(); ++i)
			{
				stCalcScoreInfo info;
				info.uid   = jvalue["user"][i]["uid"];
				info.score = jvalue["user"][i]["score"];
				info.buyIn = jvalue["user"][i]["buyin"];
				info.fee   = jvalue["user"][i]["fee"];
				info.param = jvalue["user"][i]["param"].dump();
				info.score += info.buyIn;

				m_mpPlayerScoreInfo.insert(make_pair(info.uid, info));
			}
			m_round = jvalue["round"];
		}
		catch (json::exception& e)
		{
			LOG_TAB_ERROR("json error:{}", e.what());
		}
	}
	m_startTime       = data.startTime;
	LOG_TAB_DEBUG("加载私人房信息:curRound:{}--dueRound:{}", m_round, m_conf.dueround);

	SetMatchParam();
}

bool CGamePrivateTable::CreatePrivateTable()
{
	stPrivateTable table;
	table.baseScore = m_conf.baseScore;
	table.seatNum   = m_conf.seatNum;

	table.createTime = getSysTime();
	table.dueTime    = m_conf.dueTime;
	table.dueround   = m_conf.dueround;

	table.gameType = m_pHostRoom->GetGameType();
	table.playType = GetPlayType();

	table.hostID     = m_conf.hostID;
	table.passwd     = m_conf.passwd;
	table.tableID    = GetTableID();
	table.createType = m_conf.createType;
	table.addParam   = m_conf.addParam;
	table.matchParam = m_conf.matchParam;
	table.consume    = m_conf.consume;
	table.enterMin   = m_conf.enterMin;
	table.rentType   = m_conf.rentType;
	table.fee        = m_conf.rentFee;
	table.clubID     = m_conf.clubID;
	table.clubFloor  = m_conf.clubFloor;
	table.clubPos    = m_conf.clubPos;
	table.idxRound   = m_conf.idxRound;

	if (!CheckPrivateTableConfRight())
	{
		LOG_TAB_DEBUG("私人房参数配置错误");
		return false;
	}
	SetMatchParam();

	m_tableID    = CDBMysqlMgr::Instance().GetSyncDBOper(DB_INDEX_TYPE_ACC).CreatePrivateTable(table);
	m_createTime = getSysTime();

	return m_tableID != 0;
}
bool CGamePrivateTable::CheckPrivateTableConfRight()
{

	return true;
}
void CGamePrivateTable::VoteGameOver(uint32_t reason, uint32_t opid, uint32_t reqID, uint32_t agree)
{
	if (m_voteEnd && reqID != 0)
	{
		LOG_TAB_DEBUG("投票已经结束:{}--{}--{}", opid, reqID, agree);
		return;
	}
	if (m_agreeID != 0 && m_agreeID == reqID)
	{//投票
		for (auto& seat : m_vecPlayers)
		{
			if (seat.uid == opid)
			{
				if (seat.agreeStatus != 0)
				{
					LOG_TAB_DEBUG("已经投票过了，不用再投票:{}", opid);
					return;
				}
				seat.agreeStatus = (agree == TRUE) ? 1 : 2;
				//一票否决
				if (seat.agreeStatus == 2)
				{
					m_voteEnd = true;
					NotifyVoteGameOver();
					LOG_TAB_DEBUG("一票否决:{}", opid);
					ResetVoteInfo();
					return;
				}
			}
		}
		if (AllAgreeVote())
		{
			LOG_TAB_DEBUG("全体同意解散房间");
			m_voteDiss = true;
			m_voteEnd  = true;
		}
		NotifyVoteGameOver();
	}
	else
	{
		//未开局普通玩家不能解散
		if (!IsStartGame() && opid != GetHostID())
		{
			LOG_TAB_DEBUG("未开局普通玩家不能解散:{}--{}", opid, GetHostID());
			return;
		}
		if (reqID == 0 && m_voteEnd)
		{// 发起新投票
			m_agreeID    = opid;
			m_voteReason = reason;
			m_agreeCooling.beginCooling(90*1000);
			m_voteEnd = false;
			for (uint16_t i = 0; i < m_vecPlayers.size(); ++i)
			{
				auto& seat = m_vecPlayers[i];
				seat.agreeStatus = (seat.uid == opid) ? 1 : 0;
			}
			LOG_TAB_DEBUG("发起表决:{}--{}", opid, reqID);
			if (!IsStartGame() && opid == GetHostID())
			{
				m_voteEnd  = true;
				m_voteDiss = true;
				LOG_TAB_DEBUG("房主解散不通知:{}", opid);
			}
			else
			{
				NotifyVoteGameOver();
			}
		}
		else
		{
			LOG_TAB_DEBUG("错误的投票流程");
		}
	}

	LOG_TAB_DEBUG("投票表决：rease:{}-opid:{}--{}-agree:{}", reason, opid, reqID, agree);
}
bool CGamePrivateTable::AllAgreeVote()
{
	bool bAllAgree = true;
	for (auto& seat:m_vecPlayers)
	{
		if (seat.pPlayer == NULL)continue;
		if (seat.agreeStatus != 1)
		{
			bAllAgree = false;
			break;
		}
	}
	return bAllAgree;
}
void CGamePrivateTable::ResetVoteInfo()
{
	m_agreeID = 0;
	m_agreeCooling.clearCool();
	for (auto& seat : m_vecPlayers)
	{
		seat.agreeStatus = 0;
	}
}
bool CGamePrivateTable::IsVoteStatus()
{
	if (m_agreeID != 0) return true;

	return false;
}
// 通知游戏提前结束
void CGamePrivateTable::NotifyGameOver(uint32_t reason)
{
	net::msg_notify_game_over_rep rep;
	rep.set_reason(reason);
	TableMsgToAll(&rep, net::S2C_MSG_NOTIFY_GAME_OVER_REP, false);
}
void CGamePrivateTable::NotifyVoteGameOver()
{
	net::msg_private_gameover_rep rep;
	rep.set_req_id(m_agreeID);
	rep.set_reason(m_voteReason);
	rep.set_left_time(m_agreeCooling.getCoolTick());
	rep.set_is_end(m_voteEnd ? 1 : 0);
	for (uint16_t i = 0; i < m_vecPlayers.size() && i < m_conf.seatNum; ++i)
	{
		rep.add_agree_state(m_vecPlayers[i].agreeStatus);
	}
	TableMsgToAll(&rep, net::S2C_MSG_PRIVATE_GAMEOVER_REP, false);
	LOG_TAB_DEBUG("通知解散房间");
}

void CGamePrivateTable::ResetVoteGameOverState()
{
	if (m_agreeID != 0)
	{
		if (m_voteEnd)
		{//已经结束了
			ResetVoteInfo();
			return;
		}
		if (m_agreeCooling.isTimeOut())
		{//投票超时
			bool bAllAgree = true;
			for (auto& seat:m_vecPlayers)
			{
				CGamePlayer* pTmp = seat.pPlayer;
				if (pTmp == NULL)continue;
				if (pTmp->GetNetState() == 0 && seat.agreeStatus == 0)
				{//掉线超时默认同意
					seat.agreeStatus = 1;
					continue;
				}
				if (seat.agreeStatus != 1)
				{
					bAllAgree = false;
					break;
				}
			}
			if (bAllAgree)
			{// 全体同意解散
				m_voteDiss = true;
			}
			m_voteEnd      = true;
			ResetVoteInfo();
			NotifyVoteGameOver();
		}
	}
}

void CGamePrivateTable::ForceGameOver(bool bNormal)
{
	LOG_TAB_DEBUG("强制结束房间:--{}--{}", GetHostID(), bNormal);
	if (bNormal)
	{
		m_voteDiss = true;
	}
	else
	{
		m_forceDiss = true;
	}
	m_coolLogic.clearCool();
}
// 逻辑结束
void CGamePrivateTable::LogicGameOver()
{
	LOG_TAB_DEBUG("逻辑结束房间");
	m_logicDiss = true;
	m_coolLogic.clearCool();
}

void CGamePrivateTable::AddPrivatePlayerInfo(uint32_t uid, bool bAddLog)
{
	auto it = m_mpPlayerScoreInfo.find(uid);
	if (it == m_mpPlayerScoreInfo.end())
	{
		stCalcScoreInfo info;
		info.uid   = uid;
		info.score = 0;
		info.param = "{}";
		m_mpPlayerScoreInfo.insert(make_pair(info.uid, info));
		if (bAddLog)
		{
			CDBMysqlMgr::Instance().UpdatePrivateTableGameLog(GetTableID(), uid, 1, 0, 0, 0);
		}
		LOG_TAB_DEBUG("添加玩家计分信息:{}--{}", uid, bAddLog);
	}
}

void CGamePrivateTable::DelPrivatePlayerInfo(uint32_t uid)
{
	if (m_mpPlayerScoreInfo.find(uid) != m_mpPlayerScoreInfo.end())
	{
		LOG_TAB_DEBUG("删除玩家计分信息:{}", uid);
		m_mpPlayerScoreInfo.erase(uid);
	}
}

// 自动删除没有座位的玩家
void CGamePrivateTable::AutoDelNoSeatPrivatePlayerInfo()
{
	vector<uint32_t> uids;
	for (auto& it:m_mpPlayerScoreInfo)
	{
		if (IsExistPlayer(it.first))continue;
		uids.push_back(it.first);
	}
	for (auto id:uids)
	{
		DelPrivatePlayerInfo(id);
	}
}

bool CGamePrivateTable::IsExistPrivatePlayerInfo(uint32_t uid)
{
	return m_mpPlayerScoreInfo.find(uid) != m_mpPlayerScoreInfo.end();
}

void CGamePrivateTable::SetPrivatePlayerInfoParam(uint32_t uid, string key, int32_t value)
{
	LOG_TAB_DEBUG("SetPrivate Player Info {}--{}--{}", uid, key, value);
	auto it = m_mpPlayerScoreInfo.find(uid);
	if (it == m_mpPlayerScoreInfo.end())
	{
		AddPrivatePlayerInfo(uid, true);
		it = m_mpPlayerScoreInfo.find(uid);
	}

	if (it != m_mpPlayerScoreInfo.end())
	{
		stCalcScoreInfo& info = it->second;
		json jparam;
		if (!ParseJsonFromString(jparam, info.param))
		{
			LOG_TAB_ERROR("解析param参数错误:{}", info.param);
		}
		jparam[key] = value;
		info.param = jparam.dump();
	}
	else
	{
		LOG_TAB_ERROR("the player param is null");
	}
}

int32_t CGamePrivateTable::GetPrivatePlayerInfoParam(uint32_t uid, string key)
{
	auto it = m_mpPlayerScoreInfo.find(uid);
	if (it != m_mpPlayerScoreInfo.end())
	{
		stCalcScoreInfo& info = it->second;
		json jparam;
		if (ParseJsonFromString(jparam, info.param))
		{
			int32_t ov = jparam.value(key, 0);
			return ov;
		}
		else
		{
			LOG_TAB_ERROR("解析参数错误：{}", info.param);
		}
	}
	return 0;
}

void CGamePrivateTable::ChangePrivatePlayerInfoParam(uint32_t uid, string key, int32_t value, bool bReplaceMax)
{
	LOG_TAB_DEBUG("Change Private Player Info {}--{}--{}--{}", uid, key, value, bReplaceMax);
	int32_t curValue = GetPrivatePlayerInfoParam(uid, key);
	if (bReplaceMax)
	{
		if (value > curValue)
		{
			SetPrivatePlayerInfoParam(uid, key, value);
		}
	}
	else
	{
		SetPrivatePlayerInfoParam(uid, key, curValue + value);
	}
}

int64_t CGamePrivateTable::GetPlayerGameScore(uint32_t uid)
{
	auto it = m_mpPlayerScoreInfo.find(uid);
	if (it != m_mpPlayerScoreInfo.end())
	{
		return it->second.score;
	}
	return 0;
}

void CGamePrivateTable::SetPlayerBuyIn(uint32_t uid, int64_t score)
{
	auto it = m_mpPlayerScoreInfo.find(uid);
	if (it == m_mpPlayerScoreInfo.end())
	{
		AddPrivatePlayerInfo(uid, true);
		it = m_mpPlayerScoreInfo.find(uid);
	}
	if (it != m_mpPlayerScoreInfo.end())
	{
		it->second.score = score;
		it->second.buyIn = score;
	}
	SendSeatInfoToClient();
}

// 是否可为负
bool CGamePrivateTable::CanMinus()
{
	if (m_conf.createType == emPRIVATE_CREATE_SCORE)
	{
		return m_scoreSetting.canMinus;
	}
	return true;
}

int64_t CGamePrivateTable::ChangeScoreByUID(uint32_t uid, int64_t score)
{
	auto it = m_mpPlayerScoreInfo.find(uid);
	if (it == m_mpPlayerScoreInfo.end())
	{
		AddPrivatePlayerInfo(uid, true);
		it = m_mpPlayerScoreInfo.find(uid);
	}
	if (it != m_mpPlayerScoreInfo.end())
	{
		it->second.score += score;
		return it->second.score;
	}
	return 0;
}

int64_t CGamePrivateTable::SetScoreByUID(uint32_t uid, int64_t score)
{
	auto it = m_mpPlayerScoreInfo.find(uid);
	if (it == m_mpPlayerScoreInfo.end())
	{
		AddPrivatePlayerInfo(uid, true);
		it = m_mpPlayerScoreInfo.find(uid);
	}
	if (it != m_mpPlayerScoreInfo.end())
	{
		it->second.score = score;
		return it->second.score;
	}
	return 0;
}

// 结算玩家信息
void CGamePrivateTable::CalcPlayerGameInfo(uint32_t uid, int64_t winScore)
{
	LOG_TAB_DEBUG("calc player game info:{}  {}", uid, winScore);
	CGamePlayer* pPlayer = (CGamePlayer*) CPlayerMgr::Instance().GetPlayer(uid);
	if (pPlayer != NULL)
	{
		//修改积分
		ChangeScoreByUID(uid, winScore);
		//刷新日志计分
		ChangeUserBlingLog(pPlayer, winScore);

		//刷新当前小局输赢
		auto it = m_mpPlayerScoreInfo.find(uid);
		if (it != m_mpPlayerScoreInfo.end())
		{
			it->second.roundWin += winScore;
		}
	}
}

// 清理当前局输赢
void CGamePrivateTable::ClearRoundWinScore()
{
	for (auto& it:m_mpPlayerScoreInfo)
	{
		it.second.roundWin = 0;
	}
}
//结束自动离开
void CGamePrivateTable::CheckAutoLeaveTable()
{
	if (WantNeedRecover())
	{
		//座位玩家
		for (auto& seat : m_vecPlayers)
		{
			CGamePlayer* pPlayer = seat.pPlayer;
			if (pPlayer == NULL)continue;
			if (pPlayer->GetDisconnectTime() > 0 || (getSysTime() - m_endTime) > 30)
			{//断线玩家立即离开
				if (CanLeaveTable(pPlayer))
				{
					LeaveTable(pPlayer, true, 1);
				}
			}
		}
		//旁观者
		vector<CGamePlayer*> lookers;
		for (auto& it:m_mpLookers)
		{
			lookers.push_back(it.second);
		}
		for (auto& it:lookers)
		{
			LeaveTable(it, true, 1);
		}
	}
	if (CApplication::Instance().GetStatus() == emSERVER_STATE_REPAIR)
	{
		if (!IsStartGame() && !WantNeedRecover())
		{
			LOG_TAB_ERROR("维护状态未开始游戏自动解散");
			ForceGameOver(false);
		}
	}
}
// 检测积分模式是否需要提前结束
bool CGamePrivateTable::CheckScoreModeGameOver()
{
	return false;
}
void CGamePrivateTable::OnGameRoundStart()
{
	if (m_round == 0)
	{
		m_startTime = getSysTime();
		CDBMysqlMgr::Instance().UpdatePrivateTableStartTime(GetTableID(), m_startTime);
		for (auto& seat:m_vecPlayers)
		{
			if (seat.pPlayer != NULL && IsReady(seat.pPlayer))
			{
				AddPrivatePlayerInfo(seat.pPlayer->GetUID(), true);
			}
		}
	}
	FlushClubPrivateListToClient();
	LOG_TAB_DEBUG("游戏一局开始 {}", m_round);
}
// 一局结束
void CGamePrivateTable::OnGameRoundOver()
{
	m_round++;
	SavePrivateTableGameInfo();
	CheckPrivateGameOver();
	FlushPrivateTableInfoToCache();
	LOG_TAB_DEBUG("游戏一局结束 {}", m_round);
}
void CGamePrivateTable::OnGameRoundFlush()
{
	CalcRoundTaxScore();
	ClearRoundWinScore();
	FlushSeatValueInfoToClient();
	CheckPlayerCanPlay();
	if (CheckScoreModeGameOver())
	{
		NotifyGameOver(1);//通知结束
		LogicGameOver();
	}
	LOG_TAB_DEBUG("游戏结束刷新数据 {}", m_round);
}
// 游戏结束
void CGamePrivateTable::OnGameOver(uint8_t overType)
{

}
uint8_t CGamePrivateTable::NeedGameOver()
{
	if (m_round >= m_conf.dueround && GetGameState() == TABLE_STATE_FREE)
	{
		LOG_TAB_DEBUG("局数满了,解散私人房:{}-{}", m_round, m_conf.dueround);
		return emTABLE_OVER_ROUND;
	}
	if (getSysTime() > m_conf.dueTime)
	{
		if (!IsStartGame())
		{
			LOG_TAB_DEBUG("未开局房间过期解散 私人房过期:{}--{}", getSysTime(), m_conf.dueTime);
			return emTABLE_OVER_DUETIME;
		}
		else
		{
			if (IsAllDisconnect(SECONDS_IN_30_MIN))
			{//集体断线60分钟,解散
				LOG_TAB_DEBUG("已开局房间 集体掉线30分钟的桌子自动解散");
				return emTABLE_OVER_DUETIME;
			}
			//开局游戏时间解散
			if ((getSysTime() - m_startTime) > s_MaxPlayTime)
			{
				LOG_TAB_DEBUG("开局超过游戏解散{}", s_MaxPlayTime);
				return emTABLE_OVER_DUETIME;
			}
		}
	}
	//俱乐部桌子10分钟没人则解散
	if (m_conf.clubID > 0 && (getSysTime() - m_createTime) > SECONDS_IN_MIN*10)
	{
		if (IsEmptyTable())
		{
			LOG_TAB_DEBUG("俱乐部桌子10分钟没人则解散");
			return emTABLE_OVER_DUETIME;
		}
	}

	if (m_voteDiss)
	{
		LOG_TAB_DEBUG("投票表决解散");
		return emTABLE_OVER_VOTE;
	}
	if (m_forceDiss)
	{
		LOG_TAB_DEBUG("强制解散房间");
		return emTABLE_OVER_FROCE;
	}
	if (m_logicDiss)
	{
		LOG_TAB_DEBUG("逻辑结束");
		return emTABLE_OVER_LOGIC;
	}

	return 0;
}

void CGamePrivateTable::CheckPrivateGameOver()
{
	if (m_bClosed == false)
	{
		uint8_t bRet = NeedGameOver();
		if (bRet > 0)
		{
			//房卡游戏结束，发送结果
			CDBMysqlMgr::Instance().UpdatePrivateTableCloseState(GetTableID(), bRet);
			m_bClosed = true;
			m_endTime = getSysTime();
			OnGameOver(bRet);
			if (m_conf.passwd.length() > 3)
			{
				CRedisMgr::Instance().DelTablePasswdFromCache(m_conf.passwd);
				CRedisMgr::Instance().DelTableOnlineSvrInfo(GetPasswd());
				m_conf.passwd = "";
			}
			if (m_round == 0 || bRet == emTABLE_OVER_FROCE)
			{
				BackRoomCard();
				if (m_round == 0)
				{
					CDBMysqlMgr::Instance().DeletePrivateTable(GetTableID());
					CRedisMgr::Instance().DelPrivateTableInfo(GetTableID());
				}
			}
			else
			{
				PayRoomCard();
				LogClubGameInfo();
			}

			SavePrivateTableGameInfo();
			SendPrivateGameInfo();
			LOG_TAB_DEBUG("关闭私人房");
		}
	}

	ResetVoteGameOverState();
}
bool CGamePrivateTable::IsStartGame()
{
	if (GetGameState() != TABLE_STATE_FREE || m_round != 0)
	{
		return true;
	}

	return false;
}

void CGamePrivateTable::SavePrivateTableGameInfo()
{
	json logValue;
	for (auto& it : m_mpPlayerScoreInfo)
	{
		stCalcScoreInfo& user = it.second;
		json juser;
		json jparam;

		if (!ParseJsonFromString(jparam, user.param))
		{
			jparam.clear();
		}

		juser["uid"]   = user.uid;
		juser["score"] = user.WinScore();
		juser["buyin"] = user.buyIn;
		juser["fee"]   = user.fee;
		juser["param"] = jparam;

		logValue["user"].push_back(juser);
	}
	logValue["round"] = m_round;
	LOG_TAB_DEBUG("update gameinfo:{}", logValue.dump());
	CDBMysqlMgr::Instance().UpdatePrivateTableGameInfo(GetTableID(), logValue.dump(), m_round);
}
void CGamePrivateTable::FlushPrivateTableInfoToCache()
{
	json logValue;
	logValue["round"]    = m_round;
	logValue["passwd"]   = m_conf.passwd;
	logValue["dueround"] = m_conf.dueround;
	logValue["isclose"]  = m_bClosed ? 1 : 0;
	logValue["stime"]    = m_startTime;
	logValue["etime"]    = m_endTime;

	for (auto& seat : m_vecPlayers)
	{
		if (seat.pPlayer == NULL)continue;
		json juser;
		juser["uid"]  = seat.uid;
		juser["name"] = seat.pPlayer->GetPlayerName();

		logValue["user"].push_back(juser);
	}

	//LOG_TAB_DEBUG("update table info to redis:{}", logValue.dump());
	CRedisMgr::Instance().SavePrivateTableInfo(GetTableID(), logValue.dump());
}

void CGamePrivateTable::SendPrivateGameInfo()
{
	if (m_round == 0)
	{
		for (auto& seat:m_vecPlayers)
		{
			if (seat.pPlayer != NULL)
			{
				AddPrivatePlayerInfo(seat.pPlayer->GetUID(), false);
			}
		}
	}

	net::msg_private_game_result msg_rep;
	net::private_game_info* pInfo = msg_rep.mutable_game_info();
	pInfo->set_tableid(GetTableID());

	pInfo->set_due_round(m_conf.dueround);
	pInfo->set_basescore(m_conf.baseScore);

	pInfo->set_game_type(m_pHostRoom->GetGameType());
	pInfo->set_seat_num(m_conf.seatNum);
	pInfo->set_create_time(getSysTime());
	pInfo->set_hostid(m_conf.hostID);
	pInfo->set_passwd(m_conf.passwd);
	pInfo->set_play_type(GetPlayType());

	pInfo->set_add_param(m_conf.addParam);
	pInfo->set_rent_type(m_conf.rentType);
	pInfo->set_cur_round(m_round);

	for (auto& it : m_mpPlayerScoreInfo)
	{
		net::player_game_info* pUser   = pInfo->add_user_info();
		stCalcScoreInfo      & refInfo = it.second;
		pUser->set_uid(refInfo.uid);
		pUser->set_score(refInfo.WinScore());
		pUser->set_buy_in(refInfo.buyIn);
		pUser->set_fee(refInfo.fee);
		pUser->set_param(refInfo.param);
	}
	TableMsgToAll(&msg_rep, net::S2C_MSG_PRIVATE_GAME_RESULT);

}

// 刷新桌子变更到俱乐部
void CGamePrivateTable::FlushClubPrivateListToClient(bool bDelete)
{
	if (m_conf.clubID == 0)return;

	net::msg_table_list_rep repmsg;
	repmsg.set_game_type(m_pHostRoom->GetGameType());
	repmsg.set_is_alter(bDelete ? 2 : 1);
	repmsg.set_club_id(m_conf.clubID);

	net::table_face_info* pInfo = repmsg.add_tables();
	GetTableFaceInfo(pInfo);
	LOG_TAB_DEBUG("同步俱乐部桌子列表到俱乐部:{}", m_conf.clubID);

	CCenterClientMgr::Instance().SendBroadCastMsg(&repmsg, net::S2C_MSG_TABLE_LIST, m_conf.clubID);
}

// 记录俱乐部相关信息
void CGamePrivateTable::LogClubGameInfo()
{
	CalcTableTaxScore();
	CalcClubScore();
	// 记录俱乐部开房费用
	CDBMysqlMgr::Instance().ChangeClubDayCardInfo(GetClubID(), m_conf.rentFee);
	// 记录游戏局数跟大赢家
	uint32_t maxWinID    = 0;
	int64_t  maxWinScore = 0;
	for (auto& it : m_mpPlayerScoreInfo)
	{
		stCalcScoreInfo& refInfo = it.second;
		if (refInfo.score >= maxWinScore)
		{
			maxWinID    = refInfo.uid;
			maxWinScore = refInfo.score;
		}
	}
	for (auto& it : m_mpPlayerScoreInfo)
	{
		stCalcScoreInfo& refInfo = it.second;
		uint8_t win = refInfo.uid == maxWinID ? 1 : 0;
		CDBMysqlMgr::Instance().ChangeClubWinInfo(GetClubID(), refInfo.uid, win);
		CDBMysqlMgr::Instance().UpdatePrivateTableGameLog(GetTableID(), refInfo.uid, 1, refInfo.WinScore(), refInfo.fee, win);
		CDBMysqlMgr::Instance().AddPlayGameCount(refInfo.uid, m_pHostRoom->GetGameType(), GetPlayType(), m_round);
	}

}
bool CGamePrivateTable::PayAARoomCard(CGamePlayer* pPlayer)
{
	LOG_TAB_DEBUG("AA支付房卡:{}", pPlayer->GetUID());
	int64_t card = m_conf.rentFee;
	if (CLobbyMgr::Instance().FrozenPlayerRoomCard(pPlayer->GetUID(), 0, card))
	{
		LOG_TAB_DEBUG("冻结房卡{}--{}", pPlayer->GetUID(), card);
		return true;
	}
	return false;
}
void CGamePrivateTable::BackPayAARoomCard(CGamePlayer* pPlayer)
{
	LOG_TAB_DEBUG("AA支付退回房卡:{}", pPlayer->GetUID());
	int64_t card = m_conf.rentFee;
	CLobbyMgr::Instance().UnfrozenPlayerRoomCard(pPlayer->GetUID(), 0, card);
	LOG_TAB_DEBUG("游戏未开始解散房间,退回房卡{}--{}", pPlayer->GetUID(), card);
}
//支付房卡
void CGamePrivateTable::PayRoomCard()
{
	LOG_TAB_DEBUG("支付房卡");
	int64_t card = m_conf.rentFee;
	if (m_conf.rentType == 0)
	{//房主支付
		if (card != 0 && m_conf.hostID != 0)
		{
			CLobbyMgr::Instance().MoveFrozenPlayerRoomCard(m_conf.hostID, m_conf.clubID, emACCTRAN_OPER_TYPE_PROOM, GetTableID(), card);
			LOG_TAB_DEBUG("游戏正常结束,消耗房卡{}--{}", m_conf.hostID, card);
		}
	}
	else
	{// AA支付
		for (uint16_t i = 0; i < m_vecPlayers.size(); ++i)
		{
			CGamePlayer* pGamePlayer = m_vecPlayers[i].pPlayer;
			if (pGamePlayer != NULL)
			{
				CLobbyMgr::Instance().MoveFrozenPlayerRoomCard(pGamePlayer->GetUID(), 0, emACCTRAN_OPER_TYPE_PROOM, GetTableID(), card);
				LOG_TAB_DEBUG("游戏正常结束,消耗房卡{}--{}", pGamePlayer->GetUID(), card);
			}
		}
	}

}
//退回房卡
void CGamePrivateTable::BackRoomCard()
{
	LOG_TAB_DEBUG("退回房卡");
	int64_t card = m_conf.rentFee;
	if (m_conf.rentType == 0)
	{//房主支付
		if (card != 0 && m_conf.hostID != 0)
		{
			CLobbyMgr::Instance().UnfrozenPlayerRoomCard(m_conf.hostID, m_conf.clubID, card);
			LOG_TAB_DEBUG("游戏未开始或者强制解散房间,退回房卡{}--{}", m_conf.hostID, card);
		}
	}
	else
	{// AA支付
		for (uint16_t i = 0; i < m_vecPlayers.size(); ++i)
		{
			CGamePlayer* pGamePlayer = m_vecPlayers[i].pPlayer;
			if (pGamePlayer != NULL)
			{
				BackPayAARoomCard(pGamePlayer);
			}
		}
	}
}
//是否积分场
bool CGamePrivateTable::IsScoreMode()
{
	if (m_conf.createType == emPRIVATE_CREATE_SCORE)
	{
		return true;
	}
	return false;
}
//初始化积分场积分
void CGamePrivateTable::ResetClubScore(CGamePlayer* pPlayer)
{
	if (!IsScoreMode())return;

	SetPlayerBuyIn(pPlayer->GetUID(), pPlayer->GetClubScore());
}
//抽水
void CGamePrivateTable::CalcRoundTaxScore()
{
	if (!IsScoreMode())return;
	if (m_scoreSetting.taxType != emTAX_TYPE_ROUND)return;

	LOG_TAB_DEBUG("扣除积分模式抽水:{} {} {}", m_scoreSetting.taxType, GetTableID(), GetClubID());
	vector<stCalcScoreInfo> winers;
	for (auto& p:m_mpPlayerScoreInfo)
	{
		if (p.second.roundWin > 0)
		{
			winers.push_back(p.second);
		}
	}
	//排序
	sort(winers.begin(), winers.end(), [](stCalcScoreInfo& p1, stCalcScoreInfo& p2)
	{
	  if (p1.roundWin > p2.roundWin)
	  {
		  return true;
	  }
	  return false;
	});

	for (uint8_t i = 0; i < winers.size() && i < m_scoreSetting.taxWinNum; ++i)
	{
		int64_t win = winers[i].roundWin;
		if (win < m_scoreSetting.taxBaseWin)
		{
			LOG_TAB_DEBUG("不足抽水计数:{}--{}", win, m_scoreSetting.taxBaseWin);
			continue;
		}
		int64_t tax = ceil(((double) (win*m_scoreSetting.tax)/PRO_DENO_100));//百分比抽水
		auto  it  = m_mpPlayerScoreInfo.find(winers[i].uid);
		it->second.fee += tax;
		it->second.score -= tax;
		LOG_TAB_DEBUG("小局积分抽水:{}--{}", it->second.roundWin, tax);
	}
}
void CGamePrivateTable::CalcTableTaxScore()
{
	if (!IsScoreMode())return;
	if (m_scoreSetting.taxType != emTAX_TYPE_TABLE)return;

	LOG_TAB_DEBUG("扣除积分模式抽水:{} {} {}", m_scoreSetting.taxType, GetTableID(), GetClubID());
	vector<stCalcScoreInfo> winers;
	for (auto& p:m_mpPlayerScoreInfo)
	{
		if (p.second.WinScore() > 0)
		{
			winers.push_back(p.second);
		}
	}
	//排序
	sort(winers.begin(), winers.end(), [](stCalcScoreInfo& p1, stCalcScoreInfo& p2)
	{
	  if (p1.WinScore() > p2.WinScore())
	  {
		  return true;
	  }
	  return false;
	});

	for (uint8_t i = 0; i < winers.size() && i < m_scoreSetting.taxWinNum; ++i)
	{
		int64_t win = winers[i].WinScore();
		if (win < m_scoreSetting.taxBaseWin)
		{
			LOG_TAB_DEBUG("不足抽水计数:{}--{}", win, m_scoreSetting.taxBaseWin);
			continue;
		}
		int64_t tax = ceil(((double) (win*m_scoreSetting.tax)/PRO_DENO_100));//百分比抽水
		auto  it  = m_mpPlayerScoreInfo.find(winers[i].uid);
		it->second.fee = tax;
		it->second.score -= tax;
		LOG_TAB_DEBUG("积分抽水:uid {} buyin {}--score {}-- fee {}", it->second.uid, it->second.buyIn, it->second.score, it->second.fee);
	}

}
//刷新积分信息到俱乐部
void CGamePrivateTable::CalcClubScore()
{
	if (!IsScoreMode())return;
	LOG_TAB_DEBUG("结算积分模式俱乐部分数 {}", GetClubID());
	for (auto& p:m_mpPlayerScoreInfo)
	{
		auto win = p.second.WinScore();
		LOG_TAB_DEBUG("结算俱乐部积分:uid {}--cur {},buyIn {}", p.second.uid, p.second.score, p.second.buyIn);
		CDBMysqlMgr::Instance().ChangeClubScore(GetClubID(), p.second.uid, win, p.second.fee);
	}
}
//检测玩家是否还能继续
void CGamePrivateTable::CheckPlayerCanPlay()
{
	if (!IsScoreMode())return;
	if (CanMinus())return;
	LOG_TAB_DEBUG("检测积分模式玩家是否还能继续游戏");
	for (auto& seat:m_vecPlayers)
	{
		if (seat.pPlayer != NULL && GetPlayerGameScore(seat.uid) <= 0)
		{
			seat.readyStatus = 0;
			seat.autoStatus  = 0;
			seat.readyTime   = 0;
			seat.overTimes   = 0;
		}
	}
	SendReadyStateToClient();

	uint16_t bReadCount    = GetReadyNum();
	uint16_t bNotReadCount = GetNoReadyNum();
	if (m_pHostRoom->GetGameType() == net::GAME_CATE_NIUNIU)
	{
		if (bReadCount <= 1)
		{
			LOG_TAB_DEBUG("斗牛只剩一人无法继续");
			m_voteDiss = true;
		}
	}
	else
	{
		if (bNotReadCount >= 1)
		{
			LOG_TAB_DEBUG("有人无法准备解散");
			m_voteDiss = true;
		}
	}
}
//能否坐庄
bool CGamePrivateTable::CanBankerScore(uint32_t uid)
{
	if (IsScoreMode() && !CanMinus() && GetPlayerGameScore(uid) < m_scoreSetting.minBankerScore)
	{
		LOG_TAB_DEBUG("积分不足不能做庄:{}--{}--{}", uid, m_scoreSetting.minBankerScore, GetPlayerGameScore(uid));
		return false;
	}
	return true;
}
//设置积分场参数
bool CGamePrivateTable::SetMatchParam()
{
	if (!IsScoreMode())return false;

	json jvalue;
	LOG_TAB_DEBUG("比赛场积分参数解析:{}---{}", m_conf.playType, m_conf.matchParam);
	if (!ParseJsonFromString(jvalue, m_conf.matchParam))
	{
		LOG_TAB_DEBUG("比赛场参数解析错误:{}---{}", m_conf.playType, m_conf.matchParam);
		return false;
	}
	try
	{
		//积分场信息
		m_scoreSetting.canMinus       = jvalue.value("isFufen", 0);         // 是否可负分 1-可负分 0-不可负分
		m_scoreSetting.taxType        = jvalue.value("taxType", 0);         // 抽水方式
		m_scoreSetting.tax            = jvalue.value("biaoqing", 0);        // 抽水比例
		m_scoreSetting.taxWinNum      = jvalue.value("dayingjia", 1);       // 抽水人数(大于3则全部)
		if (m_scoreSetting.taxWinNum > 3)
		{// 大于3人则抽全部
			m_scoreSetting.taxWinNum = m_conf.seatNum;
		}
		m_scoreSetting.taxBaseWin     = jvalue.value("maxScore", 0);        // 抽水基数(大于多少分才抽)
		m_scoreSetting.minBankerScore = jvalue.value("bankerScore", 0);     // 参与抢庄分数
		m_scoreSetting.scoreExchange  = jvalue.value("jifen", 1);           // 积分兑换比例
		m_conf.enterMin               = jvalue.value("gameScore", 0);       // 参与游戏分数
		m_scoreSetting.scoreExchange  = MAX(1, m_scoreSetting.scoreExchange);
		m_conf.baseScore *= m_scoreSetting.scoreExchange;
	}
	catch (json::exception& e)
	{
		LOG_TAB_ERROR("json error:{}", e.what());
		return false;
	}

	return true;
}

































