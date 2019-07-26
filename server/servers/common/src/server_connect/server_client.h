
//���������ӹ���
#pragma once

#include <vector>
#include "svrlib.h"
#include <unordered_map>
#include <memory>
#include "packet/inner_protobuf_pkg.h"
#include "servers_msg.pb.h"

using namespace std;
using namespace svrlib;
using namespace NetworkAsio;

class CServerClientMgr;

// ����������
class CServerClient {
public:
    CServerClient(const net::svr::server_info &info, const TCPConnPtr& conn);

    virtual ~CServerClient();

    void SendMsg(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin);

    void SendMsg(const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin);

    TCPConnPtr GetNetObj();

    uint16_t GetSvrID();

    uint16_t GetSvrType();

    uint16_t GetGameType();

    uint16_t GetGameSubType();

public:
    net::svr::server_info m_info;
    TCPConnPtr m_pConnPtr;
};

// ���������ӹ���
class CServerClientMgr : public CInnerMsgHanlde {
public:
    CServerClientMgr();

    virtual ~CServerClientMgr();

    virtual void OnTimer();

    virtual bool Init();

    virtual void ShutDown();

    bool AddServer(const TCPConnPtr& conn, const net::svr::server_info &info);

    void RemoveServer(const TCPConnPtr& conn);

    shared_ptr<CServerClient> GetServerBySocket(const TCPConnPtr& conn);

    shared_ptr<CServerClient> GetServerBySvrID(uint16_t svrID);

    // ָ����Ϸ��������Ϣ
    void SendMsg2Server(uint16_t svrID, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin = 0);

    void SendMsg2Server(uint16_t svrID, const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin = 0);

    // ��ָ��������Ϸ��������Ϣ
    void SendMsg2AllGameServer(uint16_t gameType, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin = 0);

    void SendMsg2AllGameServer(uint16_t gameType, const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin = 0);

    // ȫ���㲥
    void SendMsg2All(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin);

    void SendMsg2All(const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin);

    // ���·������б��ȫ��������
    void UpdateServerList();

    virtual int OnRecvClientMsg();

protected:
    //·�ɷַ���Ϣ
    int OnRouteDispMsg();

    //������ע��
    int handle_msg_register_svr();

protected:
    using MAP_SERVERS = unordered_map<uint32_t, shared_ptr<CServerClient>>;
    MAP_SERVERS m_mpServers;
    MemberTimerEvent<CServerClientMgr, &CServerClientMgr::OnTimer> m_timer;
    int32_t m_msgMinCount;//��Ϣ�������
    int32_t m_msgMaxCount;//��Ϣ��ֵ
    uint32_t m_lastCountTime;//������ʱ��

};
