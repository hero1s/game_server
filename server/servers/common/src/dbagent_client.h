
#pragma once

#include "svrlib.h"
#include <string.h>
#include "network/NetworkObject.h"
#include "packet/inner_protobuf_pkg.h"
#include <unordered_map>
#include "network/IOCPServer.h"
#include "network/tcp_connector.h"
#include "msg_define.pb.h"

using namespace std;
using namespace svrlib;
using namespace Network;

// dbagent connector连接
class CDBAgentNetObj : public CTcpConnector {
public:
    CDBAgentNetObj();

    virtual ~CDBAgentNetObj();

    virtual uint16_t GetHeadLen() {
        return INNER_HEADER_SIZE;
    };

    virtual uint16_t GetPacketLen(const uint8_t *pData, uint16_t wLen) {
        return pkg_inner::GetPacketLen(pData, wLen);
    };

    virtual uint16_t MaxTickPacket() {
        return 1000;
    }

protected:
    virtual void ConnectorOnDisconnect();

    virtual int OnRecv(uint8_t *pMsg, uint16_t wSize);

    virtual void ConnectorOnConnect(bool bSuccess, uint32_t dwNetworkIndex);

};

/******************dbagent管理器****************************/
class CDBAgentClientMgr : public CInnerMsgHanlde, public AutoDeleteSingleton<CDBAgentClientMgr> {
public:
    CDBAgentClientMgr();

    virtual ~CDBAgentClientMgr();

    void OnTimer();

    bool Init(int32_t ioKey, const net::svr::server_info &info, string ip, uint32_t port);

    void Register();

    void RegisterRep(uint16_t svrid, bool rep);

    // 大厅连接回调
    void OnConnect(bool bSuccess, CDBAgentNetObj *pNetObj);

    // 大厅断开
    void OnCloseClient(CDBAgentNetObj *pNetObj);

    bool IsRun();

public:
    void SendMsg2Center(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin, uint8_t route = 0,
                        uint32_t routeID = 0);

protected:
    //服务器注册
    int handle_msg_register_svr_rep();


private:
    MemberTimerEvent<CDBAgentClientMgr, &CDBAgentClientMgr::OnTimer> m_timer;
    CDBAgentNetObj *m_pNetObj;
    bool m_isRun;
    net::svr::server_info m_curSvrInfo;

};


