

#pragma once

#include "player_base.h"
#include <unordered_map>
#include "svrlib.h"

#include <string>
#include <memory>

// 在线玩家管理器
using namespace svrlib;
using namespace std;
using namespace Network;

class CPlayerMgr : public AutoDeleteSingleton<CPlayerMgr> {
public:
    CPlayerMgr();

    ~CPlayerMgr();

    void OnTimer();

    bool Init();

    void ShutDown();

    void OnTimeTick();

    bool IsOnline(uid_type uid);

    std::shared_ptr<CPlayerBase> GetPlayer(uid_type uid);

    bool AddPlayer(std::shared_ptr<CPlayerBase> pPlayer);

    bool RemovePlayer(std::shared_ptr<CPlayerBase> pPlayer);

    void SendMsgToAll(const google::protobuf::Message *msg, uint16_t msg_type);

    void SendMsgToAll(const void *msg, uint16_t msg_len, uint16_t msg_type);

    bool SendMsgToPlayer(const google::protobuf::Message *msg, uint16_t msg_type, uid_type uid);

    bool SendMsgToPlayer(const void *msg, uint16_t msg_len, uint16_t msg_type, uid_type uid);

    uint32_t GetOnlines();

    void GetAllPlayers(vector<std::shared_ptr<CPlayerBase>> &refVec);

    void RecoverPlayer(std::shared_ptr<CPlayerBase> pPlayer);

protected:
    void CheckRecoverPlayer();

private:
    unordered_map<uid_type, std::shared_ptr<CPlayerBase>> m_mpPlayers;
    MemberTimerEvent<CPlayerMgr, &CPlayerMgr::OnTimer> m_timer;

};




