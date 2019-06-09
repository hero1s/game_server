
#pragma once

#include "svrlib.h"
#include <string.h>
#include "network/NetworkObject.h"
#include "packet/inner_protobuf_pkg.h"
#include <unordered_map>
#include "network/IOCPServer.h"
#include "network/tcp_connector.h"
#include "servers_msg.pb.h"

using namespace std;
using namespace svrlib;
using namespace Network;

class CSvrConnectorMgr;

// server connector连接
class CSvrConnectorNetObj : public CTcpConnector {
public:
    CSvrConnectorNetObj(CSvrConnectorMgr* pHost);

    virtual ~CSvrConnectorNetObj();

    virtual uint16_t GetHeadLen()
    {
        return INNER_HEADER_SIZE;
    };

    virtual uint16_t GetPacketLen(const uint8_t* pData, uint16_t wLen)
    {
        return pkg_inner::GetPacketLen(pData, wLen);
    };
protected:
    virtual void ConnectorOnDisconnect();

    virtual int OnRecv(uint8_t* pMsg, uint16_t wSize);

    virtual void ConnectorOnConnect(bool bSuccess);

protected:
    CSvrConnectorMgr* m_pHost;

};

/******************server connector管理器****************************/
class CSvrConnectorMgr : public CInnerMsgHanlde {
public:
    CSvrConnectorMgr();

    virtual ~CSvrConnectorMgr();

    void OnTimer();

    bool Init(int32_t ioKey, const net::svr::server_info& info, string ip, uint32_t port);

    void Register();

    void RegisterRep(uint16_t svrid, bool rep);

    void OnConnect(bool bSuccess, CSvrConnectorNetObj* pNetObj);

    void OnCloseClient(CSvrConnectorNetObj* pNetObj);

    bool IsRun();

public:
    void SendMsg2Svr(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin = 0, uint8_t route = 0,
            uint32_t routeID = 0);

private:
    MemberTimerEvent<CSvrConnectorMgr, &CSvrConnectorMgr::OnTimer> m_timer;
    CSvrConnectorNetObj* m_pNetObj;
    bool m_isRun;
    net::svr::server_info m_curSvrInfo;

};




