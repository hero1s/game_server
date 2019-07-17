
#include "game_player.h"
#include "helper/bufferStream.h"
#include "stdafx.h"
#include <time.h>
#include "game_table.h"
#include "game_room.h"
#include "game_room_mgr.h"

using namespace svrlib;
using namespace std;
using namespace Network;

namespace
{

}

CGamePlayer::CGamePlayer(uint8_t type)
		:CPlayerBase(type)
{
	m_pGameTable     = NULL;
	m_pGameRoom      = NULL;
	m_msgHeartTime   = getSysTime();
	m_autoReady      = false;
	m_playDisconnect = false;
	m_clubScore      = 0;
}

CGamePlayer::~CGamePlayer()
{

}

bool CGamePlayer::SendMsgToClient(const google::protobuf::Message* msg, uint16_t msg_type)
{
	if (IsPlayDisconnect() && msg_type > 3000)
	{
		LOG_DEBUG("��������δ��ɲ�����Ϸ��Ϣ{}--{}", GetUID(), msg_type);
		return false;
	}

	CLobbyMgr::Instance().SendMsg2Client(msg, msg_type, GetUID(), GetLoginLobbySvrID());
	return true;
}

void CGamePlayer::OnLoginOut()
{
	LOG_DEBUG("GamePlayer OnLoginOut:{}", GetUID());
	SetPlayerState(PLAYER_STATE_LOGINOUT);
	if (m_pGameTable != NULL)
	{
		m_pGameTable->LeaveTable(this);
	}
	if (m_pGameRoom != NULL)
	{
		m_pGameRoom->LeaveRoom(this);
	}
	SetGameSvrID(0, true);
	SetGamePlayType(0, true);
	NotifyLeaveGameSvr();
	NotifyLobbyChangePlayStatus();
	ClearClubScore();
}

void CGamePlayer::OnLogin()
{
	LOG_DEBUG("GamePlayer OnLogin:{}---{}", GetUID(), GetCurClubID());
	SetNetState(1);
	SetGameSvrID(CApplication::Instance().GetServerID(), true);
}

void CGamePlayer::ReLogin()
{
	LOG_DEBUG("GamePlayer ReLogin:{}", GetUID());
	SetNetState(1);
	SetGameSvrID(CApplication::Instance().GetServerID(), true);
}

// ֪ͨ���ش���(�˳���Ϸ)
void CGamePlayer::NotifyLeaveGameSvr()
{
	LOG_DEBUG("notify leave game:{}", GetUID());
	net::msg_leave_svr msg;
	msg.set_uid(GetUID());

	SendMsgToClient(&msg, S2L_MSG_LEAVE_SVR);
}

// ��������޸���ֵ
void CGamePlayer::NotifyLobbyChangeAccValue(int32_t operType, int32_t subType, int64_t coin, int64_t safeCoin, const string& chessid)
{
	CLobbyMgr::Instance().NotifyLobbyChangeAccValue(GetUID(), operType, subType, coin, safeCoin, chessid);
}
// ˢ�������Ϸ״̬������
void CGamePlayer::NotifyLobbyChangePlayStatus()
{
	net::msg_notify_change_play_status msg;
	msg.set_uid(GetUID());
	msg.set_cur_room_id(0);
	msg.set_cur_table_id(0);
	if (m_pGameRoom != 0)
	{
		msg.set_cur_room_id(m_pGameRoom->GetRoomID());
	}
	if (m_pGameTable != 0)
	{
		msg.set_cur_table_id(m_pGameTable->GetTableID());
	}
	SendMsgToClient(&msg, S2L_MSG_NOTIFY_CHANGE_PLAY_STATUS);
}

// �޸�����˺���ֵ�������޸ģ�
void CGamePlayer::SyncChangeAccountValue(uint16_t operType, uint16_t subType, int64_t coin, int64_t safecoin,
                                         const string& chessid)
{
	coin     = ChangeAccountValue(emACC_VALUE_COIN, coin);
	safecoin = ChangeAccountValue(emACC_VALUE_SAFECOIN, safecoin);
	NotifyLobbyChangeAccValue(operType, subType, coin, safecoin, chessid);
}

// �ܷ��˳�
bool CGamePlayer::CanBackLobby()
{
	if (m_pGameTable != NULL && !m_pGameTable->CanLeaveTable(this))
	{
		return false;
	}

	return true;
}

// ��������ʱ��
void CGamePlayer::ResetHeart()
{
	m_msgHeartTime = getSysTime();
}

// ����ʱ��
uint32_t CGamePlayer::GetDisconnectTime()
{
	if (m_netState != 0)return 0;

	return (getSysTime() - m_msgHeartTime);
}

void CGamePlayer::OnTimeTick(uint64_t uTime, bool bNewDay)
{
	//ά��״̬
	if (CApplication::Instance().GetStatus() == emSERVER_STATE_REPAIR)
	{
		//TryLeaveCurTable();
	}
	//����״̬
	if (GetNetState() == 0)
	{
		if (m_pGameTable != NULL)
		{
			uint8_t tableType = m_pGameTable->GetTableType();
			switch (tableType)
			{
			case emTABLE_TYPE_SYSTEM:// ϵͳ����
			{
				TryLeaveCurTable();
				break;
			}
			case emTABLE_TYPE_PLAYER:// ��������
			{
				if (m_pGameTable->WantNeedRecover())
				{
					TryLeaveCurTable();
				}
				break;
			}
			default:
				break;
			}
		}
	}

}

// �Ƿ���Ҫ����
bool CGamePlayer::NeedRecover()
{
	//ά��״̬�����˳�
	if (CApplication::Instance().GetStatus() == emSERVER_STATE_REPAIR)
	{
		if (CanBackLobby())
		{
			LOG_ERROR("ά��״̬�����ش���");
			return true;
		}
	}
	if (GetDisconnectTime() > SECONDS_IN_MIN)
	{//����1�����߳�
		if (m_pGameTable != NULL)
		{
			return false;
		}
		return true;
	}
	return false;
}

// �Ƿ�������Ϸ��
bool CGamePlayer::IsInGamePlaying()
{
	if (m_pGameTable == NULL)
		return false;
	if (m_pGameTable->GetGameState() != TABLE_STATE_FREE || m_pGameTable->IsReady(this))
		return true;

	return false;
}

// ������ֵ������
void CGamePlayer::FlushAccValue2Table()
{
	if (m_pGameTable != NULL)
	{
		m_pGameTable->SendSeatInfoToClient(NULL);
	}
}

// �뿪��ǰ����
CGameTable* CGamePlayer::TryLeaveCurTable()
{
	if (m_pGameTable == NULL)
		return NULL;
	if (m_pGameTable->CanLeaveTable(this))
	{
		m_pGameTable->LeaveTable(this, true, 1);
		return m_pGameTable;
	}
	return m_pGameTable;
}
// ������ֲ���������
void CGamePlayer::ClearClubScore()
{
	if (GetClubScore() != 0)
	{
		CDBMysqlMgr::Instance().UnFrozenClubScore(GetCurClubID(), GetUID(), 0);
		SetClubScore(0);
	}
}

uint8_t CGamePlayer::GetNetState()
{
	return m_netState;
}

void CGamePlayer::SetNetState(uint8_t state)
{
	m_netState = state;
	if (m_pGameTable != NULL)
	{
		m_pGameTable->OnActionUserNetState(this, m_netState, false);
	}
	else
	{
		SetPlayDisconnect(false);
	}
	NotifyLobbyChangePlayStatus();
}
void CGamePlayer::SetNetStateNoPlayer()
{
	LOG_DEBUG("���ô�������Ҳ�����:{}", GetUID());
	m_netState = 0;
	SetPlayDisconnect(true);
	TryLeaveCurTable();
}
CGameRoom* CGamePlayer::GetRoom()
{
	return m_pGameRoom;
}

void CGamePlayer::SetRoom(CGameRoom* pRoom)
{
	m_pGameRoom = pRoom;
}

uint16_t CGamePlayer::GetRoomID()
{
	if (m_pGameRoom != NULL)
	{
		return m_pGameRoom->GetRoomID();
	}
	return 0;
}

CGameTable* CGamePlayer::GetTable()
{
	return m_pGameTable;
}

void CGamePlayer::SetTable(CGameTable* pTable)
{
	m_pGameTable = pTable;
	if (m_pGameTable == NULL)
	{
		//SetPlayDisconnect(false);
	}
}

int64_t CGamePlayer::GetTableID()
{
	if (m_pGameTable != NULL)
	{
		return m_pGameTable->GetTableID();
	}
	return 0;
}

void CGamePlayer::SetAutoReady(bool bAuto)
{
	m_autoReady = bAuto;
}

bool CGamePlayer::IsAutoReady()
{
	return m_autoReady;
}

void CGamePlayer::SetPlayDisconnect(bool bFlag)
{
	m_playDisconnect = bFlag;
}

bool CGamePlayer::IsPlayDisconnect()
{
	return m_playDisconnect;
}
// ���ֲ�����
void CGamePlayer::SetClubScore(int64_t score)
{
	m_clubScore = score;
}
int64_t CGamePlayer::GetClubScore()
{
	return m_clubScore;
}



















