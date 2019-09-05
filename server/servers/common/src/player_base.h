
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
    PLAYER_TYPE_ONLINE = 0,        // �������
    PLAYER_TYPE_ROBOT,             // ������
};
enum PLAYER_STATE {
    PLAYER_STATE_NULL,              // ��״̬
    PLAYER_STATE_LOAD_DATA,         // ��ȡ����
    PLAYER_STATE_PLAYING,           // ��Ϸ״̬
    PLAYER_STATE_LOGINOUT,          // ����
};

using uid_type = std::uint32_t;

class CPlayerBase {
public:
    CPlayerBase(PLAYER_TYPE type);

    virtual ~CPlayerBase();

    bool IsLoadData(uint8_t dataType);

    void SetLoadState(uint8_t dataType);

    //�Ƿ�������
    bool IsLoadOver();

    //���״̬
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

    // �Ƿ���Ҫ����
    virtual bool NeedRecover();

    // ���û���
    void SetNeedRecover(bool bNeed);

    // ��½key
    void SetLoginKey(const string &key);

    string GetLoginKey();
    // ��������
    void GetPlayerBaseData(net::base_info *pInfo);

    void SetPlayerBaseData(const net::base_info &info);
    // ������Ϸ������
    void SetPlayerGameData(const net::svr::msg_enter_into_game_svr& info);

    void GetPlayerGameData(net::svr::msg_enter_into_game_svr* pInfo);

    void SetOfflineTime(uint32_t _time) { m_baseInfo.offline_time = _time; };

    // ��¼lobby
    void SetLoginLobbySvrID(uint16_t svrID);
    uint16_t GetLoginLobbySvrID();
    // ��Ϸ��
    void SetGameSvrID(uint16_t svrID);
    uint16_t GetGameSvrID();

private:

protected:
    uid_type m_uid;
    PLAYER_TYPE m_bPlayerType;
    TCPConnPtr m_pSession;
    uint8_t m_bPlayerState;
    bool m_needRecover;                             // ��Ҫ���߻���
    string m_loginKey;                              // ��½key
    uint16_t m_loginLobbySvrID;                     // ��¼����������
    uint16_t m_curSvrID;                            // ��ǰ���ڷ�����ID
    stBaseInfo m_baseInfo;                          // ������Ϣ
    std::bitset<emACCDATA_TYPE_MAX> m_loadState;    // ���ݼ���״̬

};


