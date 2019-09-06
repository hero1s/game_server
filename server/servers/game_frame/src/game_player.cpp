
#include "game_player.h"
#include "helper/bufferStream.h"
#include "time/time.hpp"
#include "net/lobby_mgr.h"

using namespace svrlib;
using namespace std;
using namespace Network;

namespace
{

}

CGamePlayer::CGamePlayer(PLAYER_TYPE type)
		:CPlayerBase(type)
{
	m_msgHeartTime   = time::getSysTime();
	m_playDisconnect = false;
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
//	if (m_pGameTable != NULL)
//	{
//		m_pGameTable->LeaveTable(this);
//	}
//	if (m_pGameRoom != NULL)
//	{
//		m_pGameRoom->LeaveRoom(this);
//	}

	NotifyLeaveGameSvr();

}

void CGamePlayer::OnLogin()
{
	LOG_DEBUG("GamePlayer OnLogin:{}", GetUID());
	SetNetState(1);
}

void CGamePlayer::ReLogin()
{
	LOG_DEBUG("GamePlayer ReLogin:{}", GetUID());
	SetNetState(1);
}

// ֪ͨ���ش���(�˳���Ϸ)
void CGamePlayer::NotifyLeaveGameSvr()
{
	LOG_DEBUG("notify leave game:{}", GetUID());
	net::svr::msg_leave_svr msg;
	msg.set_uid(GetUID());

	SendMsgToClient(&msg, net::svr::GS2L_MSG_LEAVE_SVR);
}

// �ܷ��˳�
bool CGamePlayer::CanBackLobby()
{
//	if (m_pGameTable != NULL && !m_pGameTable->CanLeaveTable(this))
//	{
//		return false;
//	}

	return true;
}

// ��������ʱ��
void CGamePlayer::ResetHeart()
{
	m_msgHeartTime = time::getSysTime();
}

// ����ʱ��
uint32_t CGamePlayer::GetDisconnectTime()
{
	if (m_netState != 0)return 0;

	return (time::getSysTime() - m_msgHeartTime);
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
//		if (m_pGameTable != NULL)
//		{
//			uint8_t tableType = m_pGameTable->GetTableType();
//			switch (tableType)
//			{
//			case emTABLE_TYPE_SYSTEM:// ϵͳ����
//			{
//				TryLeaveCurTable();
//				break;
//			}
//			case emTABLE_TYPE_PLAYER:// ��������
//			{
//				if (m_pGameTable->WantNeedRecover())
//				{
//					TryLeaveCurTable();
//				}
//				break;
//			}
//			default:
//				break;
//			}
//		}
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
	if (GetDisconnectTime() > TimeConstants::MINUTE)
	{//����1�����߳�
//		if (m_pGameTable != NULL)
//		{
//			return false;
//		}
		return true;
	}
	return false;
}

// �Ƿ�������Ϸ��
bool CGamePlayer::IsInGamePlaying()
{
//	if (m_pGameTable == NULL)
//		return false;
//	if (m_pGameTable->GetGameState() != TABLE_STATE_FREE || m_pGameTable->IsReady(this))
//		return true;

	return false;
}
// ��Ϣ����
int CGamePlayer::OnMessage(uint16_t cmdID, const uint8_t* pkt_buf, uint16_t buf_len)
{
	//	CGameTable* pGameTable = GetTable();
//	if (pGameTable != NULL)
//	{
//		pGameTable->OnMessage(this, _head->cmd, _pkt_buf, _buf_len);
//	}
	return 0;
}
uint8_t CGamePlayer::GetNetState()
{
	return m_netState;
}

void CGamePlayer::SetNetState(uint8_t state)
{
	m_netState = state;
//	if (m_pGameTable != NULL)
//	{
//		m_pGameTable->OnActionUserNetState(this, m_netState, false);
//	}
//	else
//	{
//		SetPlayDisconnect(false);
//	}
//	NotifyLobbyChangePlayStatus();
}

void CGamePlayer::SetPlayDisconnect(bool bFlag)
{
	m_playDisconnect = bFlag;
}

bool CGamePlayer::IsPlayDisconnect()
{
	return m_playDisconnect;
}




















