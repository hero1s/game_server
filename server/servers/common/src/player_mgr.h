

#pragma once

#include "utility/shm_cache_mgr.h"

#include "player_base.h"
#include <unordered_map>
#include "svrlib.h"

#include <string>
#include <memory>

// 在线玩家管理器
using namespace svrlib;
using namespace std;
using namespace Network;

class CPlayerMgr : public AutoDeleteSingleton<CPlayerMgr>
{
public:
	CPlayerMgr();

	~CPlayerMgr();

	void OnTimer();

	bool Init();

	void ShutDown();

	void OnTimeTick();

	bool IsOnline(uint32_t uid);

	CPlayerBase* GetPlayer(uint32_t uid);

	bool AddPlayer(CPlayerBase* pPlayer);

	bool RemovePlayer(CPlayerBase* pPlayer);

	void SendMsgToAll(const google::protobuf::Message* msg, uint16_t msg_type);

	void SendMsgToAll(const void* msg, uint16_t msg_len, uint16_t msg_type);

	bool SendMsgToPlayer(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uid);

	bool SendMsgToPlayer(const void* msg, uint16_t msg_len, uint16_t msg_type, uint32_t uid);

	uint32_t GetOnlines();

	void GetAllPlayers(vector<CPlayerBase*>& refVec);

	void RecoverPlayer(CPlayerBase* pPlayer);
protected:
	void CheckRecoverPlayer();

private:
	unordered_map<uint32_t, CPlayerBase*>                m_mpPlayers;
	MemberTimerEvent<CPlayerMgr, &CPlayerMgr::OnTimer> m_timer;

};

class CPlayerCacheMgr : public CDataCacheMgr<4096>, public AutoDeleteSingleton<CPlayerCacheMgr> {
public:
	CPlayerCacheMgr(){};
	virtual ~CPlayerCacheMgr(){};

};


