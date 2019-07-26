
#pragma once

#include "svrlib.h"
#include <string.h>
#include "network_asio/tcp_conn.h"
#include "packet/inner_protobuf_pkg.h"
#include <unordered_map>
#include "network_asio/tcp_client.h"
#include "servers_msg.pb.h"

using namespace std;
using namespace svrlib;
using namespace NetworkAsio;

class CSvrConnectorMgr;

/******************server connector������****************************/
class CSvrConnectorMgr : public CInnerMsgHanlde {
public:
    CSvrConnectorMgr();

    virtual ~CSvrConnectorMgr();

    void OnTimer();

    bool Init(const net::svr::server_info &info, string ip, uint32_t port);

    void Register();

    void RegisterRep(uint16_t svrid, bool rep);

    void OnConnect(bool bSuccess, const TCPConnPtr& conn);

    void OnCloseClient(const TCPConnPtr& conn);

    bool IsRun();

    void SendMsg2Svr(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin = 0, uint8_t route = 0,
                     uint32_t routeID = 0);

    bool IsExistSvr(uint16_t sid);

protected:
    //������ע��
    int handle_msg_register_svr_rep();

    //���·������б�
    int handle_msg_server_list_rep();

private:
    MemberTimerEvent<CSvrConnectorMgr, &CSvrConnectorMgr::OnTimer> m_timer;
    std::shared_ptr<TCPClient> m_pClientPtr;
    bool m_isRun;
    net::svr::server_info m_curSvrInfo;
    std::unordered_map<uint16_t, net::svr::server_info> m_allSvrList;

};




