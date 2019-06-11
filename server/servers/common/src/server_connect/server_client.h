
//服务器连接管理
#pragma once

#include <vector>
#include "svrlib.h"
#include <unordered_map>
#include <memory>
#include "packet/inner_protobuf_pkg.h"
#include "servers_msg.pb.h"

using namespace std;
using namespace svrlib;
using namespace Network;

class CServerClientMgr;
// server 连接
class CSvrCliNetObj : public NetworkObject {
public:
    CSvrCliNetObj(CServerClientMgr& host);

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
    CServerClientMgr& m_host;

};

// 服务器连接
class CServerClient {
public:
    CServerClient(const net::svr::server_info& info, NetworkObject* pNetObj);

    virtual ~CServerClient();

    void SendMsg(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin);

    void SendMsg(const uint8_t* pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin);

    NetworkObject* GetNetObj();

    uint16_t GetSvrID();

    uint16_t GetSvrType();

    uint16_t GetGameType();

    uint16_t GetGameSubType();

public:
    net::svr::server_info m_info;
    NetworkObject* m_pNetObj;
};
// 服务器连接管理
class CServerClientMgr : public CInnerMsgHanlde{
public:
    CServerClientMgr();

    virtual ~CServerClientMgr();

    virtual void OnTimer();

    virtual bool Init();

    virtual void ShutDown();

    bool AddServer(NetworkObject* pNetObj, const net::svr::server_info& info);

    void RemoveServer(NetworkObject* pNetObj);

    shared_ptr<CServerClient> GetServerBySocket(NetworkObject* pNetObj);

    shared_ptr<CServerClient> GetServerBySvrID(uint16_t svrID);

    // 指定游戏服发送消息
    void SendMsg2Server(uint16_t svrID, const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin=0);

    void SendMsg2Server(uint16_t svrID, const uint8_t* pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin=0);

    // 给指定类型游戏服发送消息
    void
    SendMsg2AllGameServer(uint16_t gameType, const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin=0);

    void
    SendMsg2AllGameServer(uint16_t gameType, const uint8_t* pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin=0);

    // 全服广播
    void SendMsg2All(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin);

    void SendMsg2All(const uint8_t* pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin);

    // 更新服务器列表给全部服务器
    void UpdateServerList();

    virtual int OnRecvClientMsg();

protected:
    //路由分发消息
    int OnRouteDispMsg();

    //服务器注册
    int handle_msg_register_svr();
protected:
    using MAP_SERVERS = unordered_map<uint32_t, shared_ptr<CServerClient>>;
    MAP_SERVERS m_mpServers;
    MemberTimerEvent<CServerClientMgr, &CServerClientMgr::OnTimer> m_timer;
    int32_t m_msgMinCount;//消息计数监控
    int32_t m_msgMaxCount;//消息峰值
    uint32_t m_lastCountTime;//最后计数时间

};
