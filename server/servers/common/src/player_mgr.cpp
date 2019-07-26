

#include "player_mgr.h"
#include "svrlib.h"

using namespace svrlib;
using namespace std;
using namespace NetworkAsio;

CPlayerMgr::CPlayerMgr()
		:m_timer(this)
{
}

CPlayerMgr::~CPlayerMgr()
{
}

void CPlayerMgr::OnTimer()
{
	OnTimeTick();
	CheckRecoverPlayer();
	CApplication::Instance().schedule(&m_timer, 1000);
}

bool CPlayerMgr::Init()
{
	CApplication::Instance().schedule(&m_timer, 1000);
	return true;
}

void CPlayerMgr::ShutDown()
{
	m_timer.cancel();
	vector<CPlayerBase*> vecPlayers;
	GetAllPlayers(vecPlayers);
	for (uint32_t i = 0; i < vecPlayers.size(); ++i)
	{
		RecoverPlayer(vecPlayers[i]);
	}
	vecPlayers.clear();
}

void CPlayerMgr::OnTimeTick()
{
	static uint64_t uProcessTime = 0;
	uint64_t        uTime        = getSysTime();
	if (!uProcessTime)
		uProcessTime = uTime;

	bool bNewDay = (diffTimeDay(uProcessTime, uTime) != 0);
	for (auto& Iter : m_mpPlayers)
	{
		CPlayerBase* pPlayer = Iter.second;
		pPlayer->OnTimeTick(uTime, bNewDay);
	}
	uProcessTime = uTime;
}

bool CPlayerMgr::IsOnline(uint32_t uid)
{
	auto Iter = m_mpPlayers.find(uid);
	if (Iter != m_mpPlayers.end())
	{
		return true;
	}
	return false;
}

CPlayerBase* CPlayerMgr::GetPlayer(uint32_t dwUin)
{
	auto Iter = m_mpPlayers.find(dwUin);
	if (Iter != m_mpPlayers.end())
	{
		return Iter->second;
	}
	return NULL;
}

bool CPlayerMgr::AddPlayer(CPlayerBase* pPlayer)
{
	if (IsOnline(pPlayer->GetUID()))
	{
		LOG_ERROR("addplayer is error");
		return false;
	}
	m_mpPlayers.insert(make_pair(pPlayer->GetUID(), pPlayer));
	return true;
}

bool CPlayerMgr::RemovePlayer(CPlayerBase* pPlayer)
{
	m_mpPlayers.erase(pPlayer->GetUID());
	return true;
}

void CPlayerMgr::SendMsgToAll(const google::protobuf::Message* msg, uint16_t msg_type)
{
	for (auto& Iter : m_mpPlayers)
	{
		CPlayerBase* pPlayer = Iter.second;
		if (pPlayer->IsPlaying())
		{
			pPlayer->SendMsgToClient(msg, msg_type);
		}
	}
}

void CPlayerMgr::SendMsgToAll(const void* msg, uint16_t msg_len, uint16_t msg_type)
{
	for (auto& Iter : m_mpPlayers)
	{
		CPlayerBase* pPlayer = Iter.second;
		if (pPlayer->IsPlaying())
		{
			pPlayer->SendMsgToClient(msg, msg_len, msg_type);
		}
	}
}

bool CPlayerMgr::SendMsgToPlayer(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uid)
{
	CPlayerBase* pPlayer = GetPlayer(uid);
	if (pPlayer != NULL)
	{
		pPlayer->SendMsgToClient(msg, msg_type);
		return true;
	}
	return false;
}

bool CPlayerMgr::SendMsgToPlayer(const void* msg, uint16_t msg_len, uint16_t msg_type, uint32_t uid)
{
	CPlayerBase* pPlayer = GetPlayer(uid);
	if (pPlayer != NULL)
	{
		pPlayer->SendMsgToClient(msg, msg_len, msg_type);
		return true;
	}
	return false;
}

uint32_t CPlayerMgr::GetOnlines()
{
	return m_mpPlayers.size();
}

void CPlayerMgr::GetAllPlayers(vector<CPlayerBase*>& refVec)
{
	for (auto& Iter : m_mpPlayers)
	{
		CPlayerBase* pPlayer = Iter.second;
		refVec.push_back(pPlayer);
	}
}
void CPlayerMgr::RecoverPlayer(CPlayerBase* pPlayer)
{
	LOG_DEBUG("recover player£º{}", pPlayer->GetUID());
	pPlayer->OnLoginOut();
	RemovePlayer(pPlayer);
	SAFE_DELETE(pPlayer);
}
void CPlayerMgr::CheckRecoverPlayer()
{
	vector<CPlayerBase*> vecPlayers;
	CPlayerMgr::Instance().GetAllPlayers(vecPlayers);
	for (auto pPlayer : vecPlayers)
	{
		if (pPlayer->NeedRecover())
		{
			RecoverPlayer(pPlayer);
		}
	}
	vecPlayers.clear();
}

















