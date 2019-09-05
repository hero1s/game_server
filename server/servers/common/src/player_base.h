
#pragma once

#include "svrlib.h"
#include "db_struct_define.h"
#include "msg_define.pb.h"
#include "servers_msg.pb.h"
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
    // 基础数据
    void GetPlayerBaseData(net::base_info *pInfo);

    void SetPlayerBaseData(const net::base_info &info);
    // 进入游戏服数据
    void SetPlayerGameData(const net::svr::msg_enter_into_game_svr& info);

    void GetPlayerGameData(net::svr::msg_enter_into_game_svr* pInfo);

    void SetOfflineTime(uint32_t _time) { m_baseInfo.offline_time = _time; };

    // 登录lobby
    void SetLoginLobbySvrID(uint16_t svrID);
    uint16_t GetLoginLobbySvrID();
    // 游戏服
    void SetGameSvrID(uint16_t svrID);
    uint16_t GetGameSvrID();

private:

protected:
    uid_type m_uid;
    PLAYER_TYPE m_bPlayerType;
    TCPConnPtr m_pSession;
    uint8_t m_bPlayerState;
    bool m_needRecover;                             // 需要下线回收
    string m_loginKey;                              // 登陆key
    uint16_t m_loginLobbySvrID;                     // 登录大厅服务器
    uint16_t m_curSvrID;                            // 当前所在服务器ID
    stBaseInfo m_baseInfo;                          // 基础信息
    std::bitset<emACCDATA_TYPE_MAX> m_loadState;    // 数据加载状态

};


