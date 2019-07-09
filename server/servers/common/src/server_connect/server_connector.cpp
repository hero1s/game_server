
#include "server_connector.h"
#include "data_cfg_mgr.h"
#include "player_mgr.h"
#include "player_base.h"

using namespace std;
using namespace svrlib;

CSvrConnectorNetObj::CSvrConnectorNetObj(CSvrConnectorMgr &host)
        : m_host(host) {

}

CSvrConnectorNetObj::~CSvrConnectorNetObj() {

}

void CSvrConnectorNetObj::ConnectorOnDisconnect() {
    LOG_DEBUG("center ondisconnect");
    m_host.OnCloseClient(this);
}

int CSvrConnectorNetObj::OnRecv(uint8_t *pMsg, uint16_t wSize) {
    return m_host.OnHandleClientMsg(this, pMsg, wSize);
}

void CSvrConnectorNetObj::ConnectorOnConnect(bool bSuccess) {
    LOG_DEBUG("center server OnConnect :{}", bSuccess);
    m_host.OnConnect(bSuccess, this);
}

//----------------------------------------------------------------------------------------------------------------------------

CSvrConnectorMgr::CSvrConnectorMgr()
        : m_timer(this) {
    //m_pNetObj = NULL;
    m_isRun = false;
    bind_handler(this, net::svr::S2S_MSG_REGISTER_REP, &CSvrConnectorMgr::handle_msg_register_svr_rep);
    bind_handler(this, net::svr::S2S_MSG_SERVER_LIST_REP, &CSvrConnectorMgr::handle_msg_server_list_rep);
}

CSvrConnectorMgr::~CSvrConnectorMgr() {

}

void CSvrConnectorMgr::OnTimer() {
    CApplication::Instance().schedule(&m_timer, 3000);
}

bool CSvrConnectorMgr::Init(int32_t ioKey, const net::svr::server_info &info, string ip, uint32_t port) {
    IOCPServer &iocpServer = CApplication::Instance().GetIOCPServer();

    //CSvrConnectorNetObj *pNetObj = new CSvrConnectorNetObj(*this);
    //pNetObj->SetUID(info.svrid());
    //pNetObj->Init(&iocpServer, ioKey, ip, port);

    m_curSvrInfo = info;
    //m_pNetObj = pNetObj;
    m_isRun = false;

    CApplication::Instance().schedule(&m_timer, 3000);


    m_tcpClient = std::make_shared<NetworkAsio::TCPClient>(CApplication::Instance().GetAsioContext(), ip, port,
                                                           "server_connector");
    m_tcpClient->Connect();
    m_tcpClient->SetAutoReconnect(true);
    m_tcpClient->SetConnCallback([this](const NetworkAsio::TCPConnPtr &conn) {
        if (conn->IsConnected()) {
            LOG_DEBUG("{}, connection accepted.", conn->GetName());
            //conn->Send("client say hello!");
            this->OnConnect(true, conn);
        } else if (conn->IsDisconnecting()) {
            LOG_DEBUG("{}, connection disconnecting.", conn->GetName());
            this->OnCloseClient(conn);
        }
    });
    m_tcpClient->SetMessageCallback([this](const NetworkAsio::TCPConnPtr &conn, NetworkAsio::ByteBuffer &buffer) {
        LOG_DEBUG("recv msg, {}", std::string(buffer.Data(), buffer.Size()));
        //conn->Send("client say hello!");
        this->OnHandleClientMsg(conn, (uint8_t*)buffer.Data(), buffer.Size());
    });

    return true;
}

void CSvrConnectorMgr::Register() {
    net::svr::msg_register_svr_req msg;
    net::svr::server_info *info = msg.mutable_info();
    *info = m_curSvrInfo;

    SendMsg2Svr(&msg, net::svr::S2S_MSG_REGISTER, 0);
    LOG_DEBUG("register server svrid:{} svrtype:{}--gameType:{}", msg.info().svrid(), msg.info().svr_type(),
              msg.info().game_type());
}

void CSvrConnectorMgr::RegisterRep(uint16_t svrid, bool rep) {
    LOG_DEBUG("register center server Rep svrid:{}--res:{}", svrid, rep);

    m_isRun = rep;
}

void CSvrConnectorMgr::OnConnect(bool bSuccess, NetworkAsio::TCPConnPtr connPtr) {
    LOG_ERROR("center on connect {},{},{}", bSuccess, connPtr->GetRemoteAddress(), connPtr->GetRemotePort());
    if (bSuccess) {
        m_isRun = true;
        Register();
    }
}

void CSvrConnectorMgr::OnCloseClient(NetworkAsio::TCPConnPtr connPtr) {
    LOG_ERROR("center OnClose:{},{}", connPtr->GetRemoteAddress(), connPtr->GetRemotePort());
    m_isRun = false;
}

bool CSvrConnectorMgr::IsRun() {
    return m_isRun;
}

void CSvrConnectorMgr::SendMsg2Svr(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin, uint8_t route,
                                   uint32_t routeID) {
    if (!m_isRun || !m_tcpClient)return;
    pkg_inner::SendProtobufMsg(m_tcpClient, msg, msg_type, uin, route, routeID);
}

bool CSvrConnectorMgr::IsExistSvr(uint16_t sid) {
    return m_allSvrList.find(sid) != m_allSvrList.end();
}

//服务器注册
int CSvrConnectorMgr::handle_msg_register_svr_rep() {
    net::svr::msg_register_svr_rep msg;
    PARSE_MSG(msg);

    LOG_DEBUG("server register result :{}", msg.result());
    if (msg.result() == 1) {
        RegisterRep(_connPtr->GetUID(), true);
    } else {
        RegisterRep(_connPtr->GetUID(), false);
        LOG_ERROR("server register fail {} -->:{}", _connPtr->GetUID(), CApplication::Instance().GetServerID());
    }
    return 0;
}

//更新服务器列表
int CSvrConnectorMgr::handle_msg_server_list_rep() {
    net::svr::msg_server_list_rep msg;
    PARSE_MSG(msg);

    LOG_DEBUG("center server rep svrlist :{}", msg.server_list_size());
    m_allSvrList.clear();
    for (int i = 0; i < msg.server_list_size(); ++i) {
        m_allSvrList.insert(make_pair(msg.server_list(i).svrid(), msg.server_list(i)));
    }

    return 0;

}









