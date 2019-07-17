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
//��Ϸ��Ϣ
int CGameCoinTable::OnMessage(CGamePlayer* pPlayer, uint16_t cmdID, const uint8_t* pkt_buf, uint16_t buf_len)
{

	return OnGameMessage(pPlayer, cmdID, pkt_buf, buf_len);
}

bool CGameCoinTable::EnterTable(CGamePlayer* pPlayer)
{
	if (CanEnterTable(pPlayer) != net::RESULT_CODE_SUCCESS)
		return false;
	if (NeedSitDown())
	{//��Ҫ�ֶ�����վ��
		pPlayer->SetTable(this);
		LOG_TAB_DEBUG("�������ӣ�room:{}--tb:{},uid:{}", m_pHostRoom->GetRoomID(), GetTableID(), pPlayer->GetUID());
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
				LOG_TAB_DEBUG("�������ӣ�room:{}--tb:{},uid:{}", m_pHostRoom->GetRoomID(), GetTableID(), pPlayer->GetUID());
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
	{   //��Ҫ�ֶ�����վ��
		for (uint8_t i = 0; i < m_vecPlayers.size(); ++i)
		{
			if (m_vecPlayers[i].pPlayer == pPlayer)
			{
				m_vecPlayers[i].Reset();

				OnActionUserStandUp(i, pPlayer);
				OnPlayerJoin(false, i, pPlayer);

				pPlayer->SetTable(NULL);
				LOG_TAB_DEBUG("�뿪����:room:{}--tb:{},uid:{}", m_pHostRoom->GetRoomID(), GetTableID(), pPlayer->GetUID());
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
				LOG_TAB_DEBUG("�뿪����:room:{}--tb:{},uid:{}", m_pHostRoom->GetRoomID(), GetTableID(), pPlayer->GetUID());

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

int32_t CGameCoinTable::CanEnterTable(CGamePlayer* pPlayer)
{
	if (pPlayer->GetTable() != NULL)
	{
		LOG_TAB_ERROR("����Ѵ������ӻ��������ѹر�");
		return net::RESULT_CODE_FAIL;
	}
	if (IsFullTable() /*|| GetGameState() != TABLE_STATE_FREE*/)
	{
		LOG_TAB_ERROR("��������");
		return net::RESULT_CODE_TABLE_FULL;
	}
	// �޶����
	if (GetPlayerCurScore(pPlayer) < GetEnterMin())
	{
		LOG_TAB_ERROR("���ֲ�������:{}--{}", GetPlayerCurScore(pPlayer), GetEnterMin());
		return net::RESULT_CODE_CION_ERROR;
	}

	return net::RESULT_CODE_SUCCESS;
}

bool CGameCoinTable::CanLeaveTable(CGamePlayer* pPlayer)
{
	//�Թ��߿����뿪(���ϰ��˳�������ע���toney)
	if (IsExistLooker(pPlayer->GetUID()) && IsExistPlayer(pPlayer->GetUID()) == false)return true;

	uint16_t chairID = GetChairID(pPlayer);
	//��ҳ�û�в��뼴���뿪
	if (GetPlayStatus(chairID) == 0)
		return true;

	//�ǹر�״̬����ֻ�ܿ���״̬�����뿪
	if (GetGameState() != TABLE_STATE_FREE)
	{
		//LOG_TAB_DEBUG("�ǹر�״̬����ֻ�ܿ��в����뿪:{}", pPlayer->GetUID());
		return false;
	}

	return false;
}
// �Ƿ������Ϸ
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
//�û����߻�����
bool CGameCoinTable::OnActionUserNetState(CGamePlayer* pPlayer, bool bConnected, bool isJoin)
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
bool CGameCoinTable::OnActionUserSitDown(uint16_t wChairID, CGamePlayer* pPlayer)
{
	SendSeatInfoToClient();
	return true;
}

//�û�����
bool CGameCoinTable::OnActionUserStandUp(uint16_t wChairID, CGamePlayer* pPlayer)
{
	SendSeatInfoToClient();
	return true;
}

//�û�ͬ��
bool CGameCoinTable::OnActionUserOnReady(uint16_t wChairID, CGamePlayer* pPlayer)
{
	return true;
}

//��ҽ�����뿪
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
// �Ƿ���Ҫ����
bool CGameCoinTable::NeedRecover()
{

	return false;
}

bool CGameCoinTable::WantNeedRecover()
{

	return false;
}
//û���ֵ�����Զ�վ��
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
// �۳���ʼ̨��
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

// �۳�����̨��
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

// �ϱ���Ϸս��
void CGameCoinTable::ReportGameResult(uint32_t uid, int64_t winScore)
{
	net::msg_report_game_result msg;
	msg.set_uid(uid);
	msg.set_consume(GetConsumeType());
	msg.set_game_type(m_pHostRoom->GetGameType());
	msg.set_win_score(winScore);
	CLobbyMgr::Instance().SendMsg2Client(&msg, net::S2L_MSG_REPORT_GAME_RESULT, uid, 0);
}

// ���������Ϣ
void CGameCoinTable::CalcPlayerGameInfo(uint32_t uid, int64_t winScore)
{
	LOG_TAB_DEBUG("calc player game info:{}  {}", uid, winScore);
	ReportGameResult(uid, winScore);
	CGamePlayer* pPlayer = (CGamePlayer*) CPlayerMgr::Instance().GetPlayer(uid);
	if (pPlayer != NULL)
	{
		//��ˮ
		DeducEndFee(uid, winScore);
		//�޸Ļ���
		ChangeScoreValueByUID(uid, winScore, emACCTRAN_OPER_TYPE_GAME, m_pHostRoom->GetGameType());
		//ˢ����־�Ʒ�
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


























































