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
static const uint32_t s_MaxPlayTime = SECONDS_IN_ONE_HOUR*3;//�����Ϸʱ��3Сʱ
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
//��Ϸ��Ϣ
int CGamePrivateTable::OnMessage(CGamePlayer* pPlayer, uint16_t cmdID, const uint8_t* pkt_buf, uint16_t buf_len)
{
	switch (cmdID)
	{
	case net::C2S_MSG_PRIVATE_GAMEOVER_REQ:
	{
		net::msg_private_gameover_req msg;
		PARSE_MSG_FROM_ARRAY(msg);
		LOG_TAB_DEBUG("˽�˷�ͶƱ:{}--{}--{}", pPlayer->GetUID(), msg.is_agree(), msg.req_id());
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
	{//��Ҫ�ֶ�����վ��
		pPlayer->SetTable(this);
		LOG_TAB_DEBUG("�������ӣ�room:{},uid:{}", m_pHostRoom->GetRoomID(), pPlayer->GetUID());
		OnPlayerJoin(true, 0, pPlayer);

		pPlayer->NotifyLobbyChangePlayStatus();
		return true;
	}
	else
	{//�Զ�����
		for (uint8_t i = 0; i < m_vecPlayers.size(); ++i)
		{
			auto& seat = m_vecPlayers[i];
			if (seat.pPlayer == NULL)
			{
				SitDown(seat, pPlayer);
				pPlayer->SetTable(this);
				LOG_TAB_DEBUG("�������ӣ�room:{},uid:{}", m_pHostRoom->GetRoomID(), pPlayer->GetUID());
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
	LOG_TAB_ERROR("��������ʧ��:{}", pPlayer->GetUID());
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
	{   //��Ҫ�ֶ�����վ��
		for (uint8_t i = 0; i < m_vecPlayers.size(); ++i)
		{
			if (m_vecPlayers[i].pPlayer == pPlayer)
			{
				m_vecPlayers[i].Reset();

				OnActionUserStandUp(i, pPlayer);
				OnPlayerJoin(false, i, pPlayer);

				pPlayer->SetTable(NULL);
				LOG_TAB_DEBUG("�뿪����:room:{},uid:{}", m_pHostRoom->GetRoomID(), pPlayer->GetUID());
				if (bNotify)
				{
					NotifyPlayerLeave(pPlayer, leaveType);
				}
				pPlayer->NotifyLobbyChangePlayStatus();
				return true;
			}
		}
		LOG_TAB_DEBUG("�Թ����뿪����:{}", pPlayer->GetUID());
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
				LOG_TAB_DEBUG("�뿪����:room:{},uid:{}", m_pHostRoom->GetRoomID(), pPlayer->GetUID());

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
	LOG_TAB_ERROR("�뿪����ʧ��:{}", pPlayer->GetUID());
	return false;
}
int32_t CGamePrivateTable::CanEnterTable(CGamePlayer* pPlayer)
{
	if (pPlayer->GetTable() != NULL || m_bClosed)
	{
		LOG_TAB_ERROR("����Ѵ������ӻ��������ѹر�");
		return net::RESULT_CODE_FAIL;
	}
	if (IsFullTable() /*|| GetGameState() != TABLE_STATE_FREE*/)
	{
		LOG_TAB_ERROR("��������");
		return net::RESULT_CODE_TABLE_FULL;
	}
	if (IsVoteStatus())
	{
		LOG_TAB_ERROR("ͶƱ״̬���ܽ���");
		return net::RESULT_CODE_VOTE_STATUS;
	}

	if (m_mpPlayerScoreInfo.size() >= m_conf.seatNum)
	{
		if (!IsExistPrivatePlayerInfo(pPlayer->GetUID()))
		{
			LOG_TAB_ERROR("�������������Ҳ������Լ���ID");
			return net::RESULT_CODE_TABLE_FULL;
		}
	}
	if (!IsExistPrivatePlayerInfo(pPlayer->GetUID()))
	{
		if (!m_canJoinPlaying && IsStartGame())
		{
			LOG_TAB_DEBUG("��ֹ��;����");
			return net::RESULT_CODE_FAIL;
		}
	}
	//���ֳ���С����
	if (m_conf.createType == emPRIVATE_CREATE_SCORE && pPlayer->GetClubScore() < GetEnterMin())
	{
		LOG_TAB_ERROR("���ֲ�������:{}--{}", pPlayer->GetClubScore(), GetEnterMin());
		return net::RESULT_CODE_CION_ERROR;
	}

	return net::RESULT_CODE_SUCCESS;
}

bool CGamePrivateTable::CanLeaveTable(CGamePlayer* pPlayer)
{
	//�Թ��߿����뿪
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
			LOG_TAB_ERROR("ͶƱ״̬�����뿪");
			return false;
		}
		//û�в�������뿪
		if (!IsJoinPlay(chairID) && pPlayer->GetUID() != GetHostID())
		{
			LOG_TAB_DEBUG("û�в�����Ϸ,�����뿪:{}", pPlayer->GetUID());
			return true;
		}
		if (IsStartGame() || (GetClubID() == 0 && pPlayer->GetUID() == GetHostID()))
		{
			//LOG_TAB_DEBUG("�����˻��߷��������뿪:{}", pPlayer->GetUID());
			return false;
		}
	}

	//�ǹر�״̬����ֻ�ܿ���״̬�����뿪
	if (GetGameState() != TABLE_STATE_FREE)
	{
		//LOG_TAB_DEBUG("�ǹر�״̬����ֻ�ܿ��в����뿪:{}", pPlayer->GetUID());
		//return false;
	}

	return true;
}
// �Ƿ������Ϸ
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
	LOG_TAB_DEBUG("���׼��:{}--{}", pPlayer->GetUID(), bReady);
	if (IsScoreMode() && !CanMinus() && GetPlayerCurScore(pPlayer) <= 0)
	{
		LOG_TAB_DEBUG("����̫���޷�׼��uid {}--cur {}--min {}", pPlayer->GetUID(), GetPlayerCurScore(pPlayer), GetEnterMin());
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
//�û����߻�����
bool CGamePrivateTable::OnActionUserNetState(CGamePlayer* pPlayer, bool bConnected, bool isJoin)
{
	if (bConnected)//��������
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
//�û�����
bool CGamePrivateTable::OnActionUserSitDown(uint16_t wChairID, CGamePlayer* pPlayer)
{
	SendSeatInfoToClient();
	return true;
}

//�û�����
bool CGamePrivateTable::OnActionUserStandUp(uint16_t wChairID, CGamePlayer* pPlayer)
{
	SendSeatInfoToClient();
	return true;
}

//�û�ͬ��
bool CGamePrivateTable::OnActionUserOnReady(uint16_t wChairID, CGamePlayer* pPlayer)
{
	return true;
}

//��ҽ�����뿪
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
		{//�ⶳ����
			pPlayer->ClearClubScore();
		}
		if (!m_bClosed && m_conf.rentType == 1)
		{
			BackPayAARoomCard(pPlayer);
		}
		if (!IsStartGame())
		{//δ��ʼ,ɾ��������Ϣ
			AutoDelNoSeatPrivatePlayerInfo();
		}
	}
	FlushPrivateTableInfoToCache();
}
// �Ƿ���Ҫ����
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
	m_conf.clubID     = data.clubID;      // ���ֲ�ID
	m_conf.clubFloor  = data.clubFloor;
	m_conf.clubPos    = data.clubPos;     // ���ֲ�����λ��
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
	LOG_TAB_DEBUG("����˽�˷���Ϣ:curRound:{}--dueRound:{}", m_round, m_conf.dueround);

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
		LOG_TAB_DEBUG("˽�˷��������ô���");
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
		LOG_TAB_DEBUG("ͶƱ�Ѿ�����:{}--{}--{}", opid, reqID, agree);
		return;
	}
	if (m_agreeID != 0 && m_agreeID == reqID)
	{//ͶƱ
		for (auto& seat : m_vecPlayers)
		{
			if (seat.uid == opid)
			{
				if (seat.agreeStatus != 0)
				{
					LOG_TAB_DEBUG("�Ѿ�ͶƱ���ˣ�������ͶƱ:{}", opid);
					return;
				}
				seat.agreeStatus = (agree == TRUE) ? 1 : 2;
				//һƱ���
				if (seat.agreeStatus == 2)
				{
					m_voteEnd = true;
					NotifyVoteGameOver();
					LOG_TAB_DEBUG("һƱ���:{}", opid);
					ResetVoteInfo();
					return;
				}
			}
		}
		if (AllAgreeVote())
		{
			LOG_TAB_DEBUG("ȫ��ͬ���ɢ����");
			m_voteDiss = true;
			m_voteEnd  = true;
		}
		NotifyVoteGameOver();
	}
	else
	{
		//δ������ͨ��Ҳ��ܽ�ɢ
		if (!IsStartGame() && opid != GetHostID())
		{
			LOG_TAB_DEBUG("δ������ͨ��Ҳ��ܽ�ɢ:{}--{}", opid, GetHostID());
			return;
		}
		if (reqID == 0 && m_voteEnd)
		{// ������ͶƱ
			m_agreeID    = opid;
			m_voteReason = reason;
			m_agreeCooling.beginCooling(90*1000);
			m_voteEnd = false;
			for (uint16_t i = 0; i < m_vecPlayers.size(); ++i)
			{
				auto& seat = m_vecPlayers[i];
				seat.agreeStatus = (seat.uid == opid) ? 1 : 0;
			}
			LOG_TAB_DEBUG("������:{}--{}", opid, reqID);
			if (!IsStartGame() && opid == GetHostID())
			{
				m_voteEnd  = true;
				m_voteDiss = true;
				LOG_TAB_DEBUG("������ɢ��֪ͨ:{}", opid);
			}
			else
			{
				NotifyVoteGameOver();
			}
		}
		else
		{
			LOG_TAB_DEBUG("�����ͶƱ����");
		}
	}

	LOG_TAB_DEBUG("ͶƱ�����rease:{}-opid:{}--{}-agree:{}", reason, opid, reqID, agree);
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
// ֪ͨ��Ϸ��ǰ����
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
	LOG_TAB_DEBUG("֪ͨ��ɢ����");
}

void CGamePrivateTable::ResetVoteGameOverState()
{
	if (m_agreeID != 0)
	{
		if (m_voteEnd)
		{//�Ѿ�������
			ResetVoteInfo();
			return;
		}
		if (m_agreeCooling.isTimeOut())
		{//ͶƱ��ʱ
			bool bAllAgree = true;
			for (auto& seat:m_vecPlayers)
			{
				CGamePlayer* pTmp = seat.pPlayer;
				if (pTmp == NULL)continue;
				if (pTmp->GetNetState() == 0 && seat.agreeStatus == 0)
				{//���߳�ʱĬ��ͬ��
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
			{// ȫ��ͬ���ɢ
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
	LOG_TAB_DEBUG("ǿ�ƽ�������:--{}--{}", GetHostID(), bNormal);
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
// �߼�����
void CGamePrivateTable::LogicGameOver()
{
	LOG_TAB_DEBUG("�߼���������");
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
		LOG_TAB_DEBUG("�����ҼƷ���Ϣ:{}--{}", uid, bAddLog);
	}
}

void CGamePrivateTable::DelPrivatePlayerInfo(uint32_t uid)
{
	if (m_mpPlayerScoreInfo.find(uid) != m_mpPlayerScoreInfo.end())
	{
		LOG_TAB_DEBUG("ɾ����ҼƷ���Ϣ:{}", uid);
		m_mpPlayerScoreInfo.erase(uid);
	}
}

// �Զ�ɾ��û����λ�����
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
			LOG_TAB_ERROR("����param��������:{}", info.param);
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
			LOG_TAB_ERROR("������������{}", info.param);
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

// �Ƿ��Ϊ��
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

// ���������Ϣ
void CGamePrivateTable::CalcPlayerGameInfo(uint32_t uid, int64_t winScore)
{
	LOG_TAB_DEBUG("calc player game info:{}  {}", uid, winScore);
	CGamePlayer* pPlayer = (CGamePlayer*) CPlayerMgr::Instance().GetPlayer(uid);
	if (pPlayer != NULL)
	{
		//�޸Ļ���
		ChangeScoreByUID(uid, winScore);
		//ˢ����־�Ʒ�
		ChangeUserBlingLog(pPlayer, winScore);

		//ˢ�µ�ǰС����Ӯ
		auto it = m_mpPlayerScoreInfo.find(uid);
		if (it != m_mpPlayerScoreInfo.end())
		{
			it->second.roundWin += winScore;
		}
	}
}

// ����ǰ����Ӯ
void CGamePrivateTable::ClearRoundWinScore()
{
	for (auto& it:m_mpPlayerScoreInfo)
	{
		it.second.roundWin = 0;
	}
}
//�����Զ��뿪
void CGamePrivateTable::CheckAutoLeaveTable()
{
	if (WantNeedRecover())
	{
		//��λ���
		for (auto& seat : m_vecPlayers)
		{
			CGamePlayer* pPlayer = seat.pPlayer;
			if (pPlayer == NULL)continue;
			if (pPlayer->GetDisconnectTime() > 0 || (getSysTime() - m_endTime) > 30)
			{//������������뿪
				if (CanLeaveTable(pPlayer))
				{
					LeaveTable(pPlayer, true, 1);
				}
			}
		}
		//�Թ���
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
			LOG_TAB_ERROR("ά��״̬δ��ʼ��Ϸ�Զ���ɢ");
			ForceGameOver(false);
		}
	}
}
// ������ģʽ�Ƿ���Ҫ��ǰ����
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
	LOG_TAB_DEBUG("��Ϸһ�ֿ�ʼ {}", m_round);
}
// һ�ֽ���
void CGamePrivateTable::OnGameRoundOver()
{
	m_round++;
	SavePrivateTableGameInfo();
	CheckPrivateGameOver();
	FlushPrivateTableInfoToCache();
	LOG_TAB_DEBUG("��Ϸһ�ֽ��� {}", m_round);
}
void CGamePrivateTable::OnGameRoundFlush()
{
	CalcRoundTaxScore();
	ClearRoundWinScore();
	FlushSeatValueInfoToClient();
	CheckPlayerCanPlay();
	if (CheckScoreModeGameOver())
	{
		NotifyGameOver(1);//֪ͨ����
		LogicGameOver();
	}
	LOG_TAB_DEBUG("��Ϸ����ˢ������ {}", m_round);
}
// ��Ϸ����
void CGamePrivateTable::OnGameOver(uint8_t overType)
{

}
uint8_t CGamePrivateTable::NeedGameOver()
{
	if (m_round >= m_conf.dueround && GetGameState() == TABLE_STATE_FREE)
	{
		LOG_TAB_DEBUG("��������,��ɢ˽�˷�:{}-{}", m_round, m_conf.dueround);
		return emTABLE_OVER_ROUND;
	}
	if (getSysTime() > m_conf.dueTime)
	{
		if (!IsStartGame())
		{
			LOG_TAB_DEBUG("δ���ַ�����ڽ�ɢ ˽�˷�����:{}--{}", getSysTime(), m_conf.dueTime);
			return emTABLE_OVER_DUETIME;
		}
		else
		{
			if (IsAllDisconnect(SECONDS_IN_30_MIN))
			{//�������60����,��ɢ
				LOG_TAB_DEBUG("�ѿ��ַ��� �������30���ӵ������Զ���ɢ");
				return emTABLE_OVER_DUETIME;
			}
			//������Ϸʱ���ɢ
			if ((getSysTime() - m_startTime) > s_MaxPlayTime)
			{
				LOG_TAB_DEBUG("���ֳ�����Ϸ��ɢ{}", s_MaxPlayTime);
				return emTABLE_OVER_DUETIME;
			}
		}
	}
	//���ֲ�����10����û�����ɢ
	if (m_conf.clubID > 0 && (getSysTime() - m_createTime) > SECONDS_IN_MIN*10)
	{
		if (IsEmptyTable())
		{
			LOG_TAB_DEBUG("���ֲ�����10����û�����ɢ");
			return emTABLE_OVER_DUETIME;
		}
	}

	if (m_voteDiss)
	{
		LOG_TAB_DEBUG("ͶƱ�����ɢ");
		return emTABLE_OVER_VOTE;
	}
	if (m_forceDiss)
	{
		LOG_TAB_DEBUG("ǿ�ƽ�ɢ����");
		return emTABLE_OVER_FROCE;
	}
	if (m_logicDiss)
	{
		LOG_TAB_DEBUG("�߼�����");
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
			//������Ϸ���������ͽ��
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
			LOG_TAB_DEBUG("�ر�˽�˷�");
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

// ˢ�����ӱ�������ֲ�
void CGamePrivateTable::FlushClubPrivateListToClient(bool bDelete)
{
	if (m_conf.clubID == 0)return;

	net::msg_table_list_rep repmsg;
	repmsg.set_game_type(m_pHostRoom->GetGameType());
	repmsg.set_is_alter(bDelete ? 2 : 1);
	repmsg.set_club_id(m_conf.clubID);

	net::table_face_info* pInfo = repmsg.add_tables();
	GetTableFaceInfo(pInfo);
	LOG_TAB_DEBUG("ͬ�����ֲ������б����ֲ�:{}", m_conf.clubID);

	CCenterClientMgr::Instance().SendBroadCastMsg(&repmsg, net::S2C_MSG_TABLE_LIST, m_conf.clubID);
}

// ��¼���ֲ������Ϣ
void CGamePrivateTable::LogClubGameInfo()
{
	CalcTableTaxScore();
	CalcClubScore();
	// ��¼���ֲ���������
	CDBMysqlMgr::Instance().ChangeClubDayCardInfo(GetClubID(), m_conf.rentFee);
	// ��¼��Ϸ��������Ӯ��
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
	LOG_TAB_DEBUG("AA֧������:{}", pPlayer->GetUID());
	int64_t card = m_conf.rentFee;
	if (CLobbyMgr::Instance().FrozenPlayerRoomCard(pPlayer->GetUID(), 0, card))
	{
		LOG_TAB_DEBUG("���᷿��{}--{}", pPlayer->GetUID(), card);
		return true;
	}
	return false;
}
void CGamePrivateTable::BackPayAARoomCard(CGamePlayer* pPlayer)
{
	LOG_TAB_DEBUG("AA֧���˻ط���:{}", pPlayer->GetUID());
	int64_t card = m_conf.rentFee;
	CLobbyMgr::Instance().UnfrozenPlayerRoomCard(pPlayer->GetUID(), 0, card);
	LOG_TAB_DEBUG("��Ϸδ��ʼ��ɢ����,�˻ط���{}--{}", pPlayer->GetUID(), card);
}
//֧������
void CGamePrivateTable::PayRoomCard()
{
	LOG_TAB_DEBUG("֧������");
	int64_t card = m_conf.rentFee;
	if (m_conf.rentType == 0)
	{//����֧��
		if (card != 0 && m_conf.hostID != 0)
		{
			CLobbyMgr::Instance().MoveFrozenPlayerRoomCard(m_conf.hostID, m_conf.clubID, emACCTRAN_OPER_TYPE_PROOM, GetTableID(), card);
			LOG_TAB_DEBUG("��Ϸ��������,���ķ���{}--{}", m_conf.hostID, card);
		}
	}
	else
	{// AA֧��
		for (uint16_t i = 0; i < m_vecPlayers.size(); ++i)
		{
			CGamePlayer* pGamePlayer = m_vecPlayers[i].pPlayer;
			if (pGamePlayer != NULL)
			{
				CLobbyMgr::Instance().MoveFrozenPlayerRoomCard(pGamePlayer->GetUID(), 0, emACCTRAN_OPER_TYPE_PROOM, GetTableID(), card);
				LOG_TAB_DEBUG("��Ϸ��������,���ķ���{}--{}", pGamePlayer->GetUID(), card);
			}
		}
	}

}
//�˻ط���
void CGamePrivateTable::BackRoomCard()
{
	LOG_TAB_DEBUG("�˻ط���");
	int64_t card = m_conf.rentFee;
	if (m_conf.rentType == 0)
	{//����֧��
		if (card != 0 && m_conf.hostID != 0)
		{
			CLobbyMgr::Instance().UnfrozenPlayerRoomCard(m_conf.hostID, m_conf.clubID, card);
			LOG_TAB_DEBUG("��Ϸδ��ʼ����ǿ�ƽ�ɢ����,�˻ط���{}--{}", m_conf.hostID, card);
		}
	}
	else
	{// AA֧��
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
//�Ƿ���ֳ�
bool CGamePrivateTable::IsScoreMode()
{
	if (m_conf.createType == emPRIVATE_CREATE_SCORE)
	{
		return true;
	}
	return false;
}
//��ʼ�����ֳ�����
void CGamePrivateTable::ResetClubScore(CGamePlayer* pPlayer)
{
	if (!IsScoreMode())return;

	SetPlayerBuyIn(pPlayer->GetUID(), pPlayer->GetClubScore());
}
//��ˮ
void CGamePrivateTable::CalcRoundTaxScore()
{
	if (!IsScoreMode())return;
	if (m_scoreSetting.taxType != emTAX_TYPE_ROUND)return;

	LOG_TAB_DEBUG("�۳�����ģʽ��ˮ:{} {} {}", m_scoreSetting.taxType, GetTableID(), GetClubID());
	vector<stCalcScoreInfo> winers;
	for (auto& p:m_mpPlayerScoreInfo)
	{
		if (p.second.roundWin > 0)
		{
			winers.push_back(p.second);
		}
	}
	//����
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
			LOG_TAB_DEBUG("�����ˮ����:{}--{}", win, m_scoreSetting.taxBaseWin);
			continue;
		}
		int64_t tax = ceil(((double) (win*m_scoreSetting.tax)/PRO_DENO_100));//�ٷֱȳ�ˮ
		auto  it  = m_mpPlayerScoreInfo.find(winers[i].uid);
		it->second.fee += tax;
		it->second.score -= tax;
		LOG_TAB_DEBUG("С�ֻ��ֳ�ˮ:{}--{}", it->second.roundWin, tax);
	}
}
void CGamePrivateTable::CalcTableTaxScore()
{
	if (!IsScoreMode())return;
	if (m_scoreSetting.taxType != emTAX_TYPE_TABLE)return;

	LOG_TAB_DEBUG("�۳�����ģʽ��ˮ:{} {} {}", m_scoreSetting.taxType, GetTableID(), GetClubID());
	vector<stCalcScoreInfo> winers;
	for (auto& p:m_mpPlayerScoreInfo)
	{
		if (p.second.WinScore() > 0)
		{
			winers.push_back(p.second);
		}
	}
	//����
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
			LOG_TAB_DEBUG("�����ˮ����:{}--{}", win, m_scoreSetting.taxBaseWin);
			continue;
		}
		int64_t tax = ceil(((double) (win*m_scoreSetting.tax)/PRO_DENO_100));//�ٷֱȳ�ˮ
		auto  it  = m_mpPlayerScoreInfo.find(winers[i].uid);
		it->second.fee = tax;
		it->second.score -= tax;
		LOG_TAB_DEBUG("���ֳ�ˮ:uid {} buyin {}--score {}-- fee {}", it->second.uid, it->second.buyIn, it->second.score, it->second.fee);
	}

}
//ˢ�»�����Ϣ�����ֲ�
void CGamePrivateTable::CalcClubScore()
{
	if (!IsScoreMode())return;
	LOG_TAB_DEBUG("�������ģʽ���ֲ����� {}", GetClubID());
	for (auto& p:m_mpPlayerScoreInfo)
	{
		auto win = p.second.WinScore();
		LOG_TAB_DEBUG("������ֲ�����:uid {}--cur {},buyIn {}", p.second.uid, p.second.score, p.second.buyIn);
		CDBMysqlMgr::Instance().ChangeClubScore(GetClubID(), p.second.uid, win, p.second.fee);
	}
}
//�������Ƿ��ܼ���
void CGamePrivateTable::CheckPlayerCanPlay()
{
	if (!IsScoreMode())return;
	if (CanMinus())return;
	LOG_TAB_DEBUG("������ģʽ����Ƿ��ܼ�����Ϸ");
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
			LOG_TAB_DEBUG("��ţֻʣһ���޷�����");
			m_voteDiss = true;
		}
	}
	else
	{
		if (bNotReadCount >= 1)
		{
			LOG_TAB_DEBUG("�����޷�׼����ɢ");
			m_voteDiss = true;
		}
	}
}
//�ܷ���ׯ
bool CGamePrivateTable::CanBankerScore(uint32_t uid)
{
	if (IsScoreMode() && !CanMinus() && GetPlayerGameScore(uid) < m_scoreSetting.minBankerScore)
	{
		LOG_TAB_DEBUG("���ֲ��㲻����ׯ:{}--{}--{}", uid, m_scoreSetting.minBankerScore, GetPlayerGameScore(uid));
		return false;
	}
	return true;
}
//���û��ֳ�����
bool CGamePrivateTable::SetMatchParam()
{
	if (!IsScoreMode())return false;

	json jvalue;
	LOG_TAB_DEBUG("���������ֲ�������:{}---{}", m_conf.playType, m_conf.matchParam);
	if (!ParseJsonFromString(jvalue, m_conf.matchParam))
	{
		LOG_TAB_DEBUG("������������������:{}---{}", m_conf.playType, m_conf.matchParam);
		return false;
	}
	try
	{
		//���ֳ���Ϣ
		m_scoreSetting.canMinus       = jvalue.value("isFufen", 0);         // �Ƿ�ɸ��� 1-�ɸ��� 0-���ɸ���
		m_scoreSetting.taxType        = jvalue.value("taxType", 0);         // ��ˮ��ʽ
		m_scoreSetting.tax            = jvalue.value("biaoqing", 0);        // ��ˮ����
		m_scoreSetting.taxWinNum      = jvalue.value("dayingjia", 1);       // ��ˮ����(����3��ȫ��)
		if (m_scoreSetting.taxWinNum > 3)
		{// ����3�����ȫ��
			m_scoreSetting.taxWinNum = m_conf.seatNum;
		}
		m_scoreSetting.taxBaseWin     = jvalue.value("maxScore", 0);        // ��ˮ����(���ڶ��ٷֲų�)
		m_scoreSetting.minBankerScore = jvalue.value("bankerScore", 0);     // ������ׯ����
		m_scoreSetting.scoreExchange  = jvalue.value("jifen", 1);           // ���ֶһ�����
		m_conf.enterMin               = jvalue.value("gameScore", 0);       // ������Ϸ����
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

































