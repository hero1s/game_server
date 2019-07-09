
//���������ӹ���
#pragma once

#include <vector>
#include "svrlib.h"
#include <unordered_map>
#include <memory>
#include "packet/inner_protobuf_pkg.h"
#include "servers_msg.pb.h"

#include "network_asio/tcp_client.h"
#include "network_asio/tcp_conn.h"
#include "network_asio/byte_buffer.h"

using namespace std;
using namespace svrlib;
using namespace Network;

class CServerClientMgr;

// server ����
class CSvrCliNetObj : public NetworkObject {
public:
    CSvrCliNetObj(CServerClientMgr &host);

    virtual ~CSvrCliNetObj();

    virtual uint16_t GetHeadLen() {
        return INNER_HEADER_SIZE;
    };

    virtual uint16_t GetPacketLen(const uint8_t *pData, uint16_t wLen) {
        return pkg_inner::GetPacketLen(pData, wLen);
    };
protected:
    virtual void OnDisconnect();

    virtual int OnRecv(uint8_t *pMsg, uint16_t wSize);

    virtual void OnConnect(bool bSuccess);

protected:
    CServerClientMgr &m_host;

};

// ����������
class CServerClient {
public:
    CServerClient(const net::svr::server_info &info, NetworkAsio::TCPConnPtr connPtr);

    virtual ~CServerClient();

    void SendMsg(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin);

    void SendMsg(const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin);

    NetworkAsio::TCPConnPtr GetNetObj();

    uint16_t GetSvrID();

    uint16_t GetSvrType();

    uint16_t GetGameType();

    uint16_t GetGameSubType();

public:
    net::svr::server_info m_info;
    //NetworkObject *m_pNetObj;
    NetworkAsio::TCPConnPtr m_connPtr = nullptr;
};

// ���������ӹ���
class CServerClientMgr : public CInnerMsgHanlde {
public:
    CServerClientMgr();

    virtual ~CServerClientMgr();

    virtual void OnTimer();

    virtual bool Init();

    virtual void ShutDown();

    bool AddServer(NetworkAsio::TCPConnPtr connPtr, const net::svr::server_info &info);

    void RemoveServer(NetworkAsio::TCPConnPtr connPtr);

    shared_ptr<CServerClient> GetServerBySocket(NetworkAsio::TCPConnPtr connPtr);

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
