
#include "game_net_mgr.h"
#include "msg_define.pb.h"
#include "game_server_config.h"
#include "msg_client_handle.h"
#include "helper/helper.h"
#include "player_mgr.h"
#include "player.h"

using namespace svrlib;
using namespace std;
using namespace Network;

CClientNetObj::CClientNetObj()
{
	m_seqNum = 0;
	SetUID(0);
	m_maxTickPacket = 20;
}

CClientNetObj::~CClientNetObj()
{

}
bool CClientNetObj::IsCanHandle(const uint8_t* pMsg, uint16_t wSize)
{
	packet_header_t* head = (packet_header_t*) pMsg;
	if (head->uin > m_seqNum)
	{
		m_seqNum = head->uin;
	}
	else
	{
		LOG_ERROR("the seqnum is error:{}--{}", m_seqNum, head->uin);
		return false;// 跳过校验
	}
	if (GetUID() == 0 && head->cmd != net::C2S_MSG_LOGIN)
	{
		LOG_ERROR("no logining and send play msg :{} ", head->cmd);
		return false;
	}
	return true;
}

void CClientNetObj::OnDisconnect()
{
	LOG_ERROR("client ondisconnect:{}--{}", GetUID(), GetIP().c_str());
	uint32_t uid = GetUID();
	CPlayer* pPlayer = (CPlayer*) CPlayerMgr::Instance().GetPlayer(uid);
	if (pPlayer != NULL)
	{
		// 不直接断线，保留一定时间
		pPlayer->SetSession(NULL);
		SetUID(0);
	}
}

int CClientNetObj::OnRecv(uint8_t* pMsg, uint16_t wSize)
{
	//test
	LOG_DEBUG("data:{}",string((char*)(pMsg),wSize));
	Send(pMsg,wSize);
	return 0;

	if (pMsg == NULL)
		return -1;
	//if (IsCanHandle(pMsg, wSize) == false)
	//	return -1;

	return CHandleClientMsg::Instance().OnHandleClientMsg(this, pMsg, wSize);
}

void CClientNetObj::OnConnect(bool bSuccess)
{
	LOG_DEBUG("Client OnConnect,{},{}", bSuccess,GetIP());
}















