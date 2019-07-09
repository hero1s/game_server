
#pragma once

#include "svrlib.h"
#include <string.h>
#include "network/NetworkObject.h"
#include "packet/inner_protobuf_pkg.h"
#include <unordered_map>
#include "network/IOCPServer.h"
#include "network/tcp_connector.h"
#include "servers_msg.pb.h"

#include "network_asio/tcp_client.h"
#include "network_asio/tcp_conn.h"
#include "network_asio/byte_buffer.h"

using namespace std;
using namespace svrlib;
using namespace Network;

class CSvrConnectorMgr;

// server connector����
class CSvrConnectorNetObj : public CTcpConnector {
public:
    CSvrConnectorNetObj(CSvrConnectorMgr& host);

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
    CSvrConnectorMgr& m_host;

};

/******************server connector������****************************/
class CSvrConnectorMgr : public CInnerMsgHanlde {
public:
    CSvrConnectorMgr();

    virtual ~CSvrConnectorMgr();

    void OnTimer();

    bool Init(int32_t ioKey, const net::svr::server_info& info, string ip, uint32_t port);

    void Register();

    void RegisterRep(uint16_t svrid, bool rep);

    void OnConnect(bool bSuccess, NetworkAsio::TCPConnPtr connPtr);

    void OnCloseClient(NetworkAsio::TCPConnPtr connPtr);

    bool IsRun();

    void SendMsg2Svr(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin = 0, uint8_t route = 0,
            uint32_t routeID = 0);

    bool IsExistSvr(uint16_t sid);

protected:
    //������ע��
    int handle_msg_register_svr_rep();

    //���·������б�
    int handle_msg_server_list_rep();

private:
    MemberTimerEvent<CSvrConnectorMgr, &CSvrConnectorMgr::OnTimer> m_timer;
    //CSvrConnectorNetObj* m_pNetObj;
    bool m_isRun;
    net::svr::server_info m_curSvrInfo;
    std::unordered_map<uint16_t,net::svr::server_info> m_allSvrList;

    std::shared_ptr<NetworkAsio::TCPClient> m_tcpClient = nullptr;
};



