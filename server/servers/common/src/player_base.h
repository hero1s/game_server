
#pragma once

#include "svrlib.h"
#include "db_struct_define.h"
#include "msg_define.pb.h"
#include "network/tcp_conn.h"
#include<bitset>

using namespace svrlib;
using namespace std;
using namespace Network;

enum PLAYER_TYPE {
    PLAYER_TYPE_ONLINE = 0,        // 在线玩家
    PLAYER_TYPE_ROBOT,             // 机器人
};
enum PLAYER_STATE {
    PLAYER_STATE_NULL,              // 空状态
    PLAYER_STATE_LOAD_DATA,         // 拉取数据
    PLAYER_STATE_PLAYING,           // 游戏状态
    PLAYER_STATE_LOGINOUT,          // 下线
};

using uid_type = std::uint32_t;

class CPlayerBase {
public:
    CPlayerBase(PLAYER_TYPE type);

    virtual ~CPlayerBase();

    bool IsLoadData(uint8_t dataType);

    void SetLoadState(uint8_t dataType);

    //是否加载完成
    bool IsLoadOver();

    //玩家状态
    void SetPlayerState(uint8_t state);

    uint8_t GetPlayerState();

    bool IsPlaying();

    uid_type GetUID();

    void SetUID(uid_type uid);

    PLAYER_TYPE GetPlayerType();

    string GetPlayerName();

    uint8_t GetSex();

    void SetSession(const TCPConnPtr& conn);

    TCPConnPtr GetSession();

    void SetIP(uint32_t ip);

    uint32_t GetIP();

    string GetIPStr();

    virtual bool SendMsgToClient(const google::protobuf::Message *msg, uint16_t msg_type);

    virtual bool SendMsgToClient(const void *msg, uint16_t msg_len, uint16_t msg_type);

    virtual void OnLoginOut();

    virtual void OnLogin();

    virtual void OnGetAllData();

    virtual void ReLogin();

    virtual void OnTimeTick(uint64_t uTime, bool bNewDay);

    // 是否需要回收
    virtual bool NeedRecover();

    // 设置回收
    void SetNeedRecover(bool bNeed);

    // 登陆key
    void SetLoginKey(const string &key);

    string GetLoginKey();

    void GetPlayerBaseData(net::base_info *pInfo);

    void SetPlayerBaseData(const net::base_info &info);

    void SetOfflineTime(uint32_t _time) { m_baseInfo.offline_time = _time; };

private:

protected:
    uid_type m_uid;
    PLAYER_TYPE m_bPlayerType;
    TCPConnPtr m_pSession;
    uint8_t m_bPlayerState;
    bool m_needRecover;                             // 需要下线回收
    string m_loginKey;                              // 登陆key
    stBaseInfo m_baseInfo;                          // 基础信息
    std::bitset<emACCDATA_TYPE_MAX> m_loadState;    // 数据加载状态

};


