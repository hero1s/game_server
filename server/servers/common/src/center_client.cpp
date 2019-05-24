
#include "center_client.h"
#include "data_cfg_mgr.h"
#include "player_mgr.h"
#include "player_base.h"

using namespace std;
using namespace svrlib;

CCenterNetObj::CCenterNetObj() {

}

CCenterNetObj::~CCenterNetObj() {

}

void CCenterNetObj::ConnectorOnDisconnect() {
    LOG_DEBUG("center ondisconnect");
    CCenterClientMgr::Instance().OnCloseClient(this);
}

int CCenterNetObj::OnRecv(uint8_t *pMsg, uint16_t wSize) {
    return CCenterClientMgr::Instance().OnHandleClientMsg(this, pMsg, wSize);
}

void CCenterNetObj::ConnectorOnConnect(bool bSuccess) {
    LOG_DEBUG("center server OnConnect :{}", bSuccess);
    CCenterClientMgr::Instance().OnConnect(bSuccess, this);
}

//----------------------------------------------------------------------------------------------------------------------------

CCenterClientMgr::CCenterClientMgr() {
    m_pNetObj = NULL;
    m_isRun = false;

    bind_handler(this, net::svr::CS2S_MSG_REGISTER_CENTER_REP, &CCenterClientMgr::handle_msg_register_svr_rep);
    bind_handler(this, net::svr::CS2S_MSG_SERVER_LIST_REP, &CCenterClientMgr::handle_msg_server_list_rep);
}

CCenterClientMgr::~CCenterClientMgr() {
    if(m_pTimer)m_pTimer->cancel();
}

void CCenterClientMgr::OnTimer() {

    m_pTimer->expires_from_now(std::chrono::milliseconds(3000));
    m_pTimer->async_wait(std::bind(&CCenterClientMgr::OnTimer, this));
}

bool CCenterClientMgr::Init(int32_t ioKey, const net::svr::server_info &info, string ip, uint32_t port) {
    IOCPServer &iocpServer = CApplication::Instance().GetIOCPServer();
    asio::io_context &context = CApplication::Instance().GetAsioContext();

    CCenterNetObj *pNetObj = new CCenterNetObj();
    pNetObj->SetUID(info.svrid());
    pNetObj->Init(context, &iocpServer, ioKey, ip, port);

    m_curSvrInfo = info;
    m_pNetObj = pNetObj;
    m_isRun = false;

    m_pTimer = make_shared<asio::system_timer>(CApplication::Instance().GetAsioContext());
    m_pTimer->expires_from_now(std::chrono::milliseconds(3000));
    m_pTimer->async_wait(std::bind(&CCenterClientMgr::OnTimer, this));

    return true;
}

void CCenterClientMgr::Register() {
    net::svr::msg_register_center_svr_req msg;
    net::svr::server_info *info = msg.mutable_info();
    *info = m_curSvrInfo;

    SendMsg2Center(&msg, net::svr::S2CS_MSG_REGISTER_CENTER, 0);
    LOG_DEBUG("register center server svrid:{} svrtype:{}--gameType:{}", msg.info().svrid(), msg.info().svr_type(),
              msg.info().game_type());
}

void CCenterClientMgr::RegisterRep(uint16_t svrid, bool rep) {
    LOG_DEBUG("register center server Rep svrid:{}--res:{}", svrid, rep);

    m_isRun = rep;
}

void CCenterClientMgr::OnConnect(bool bSuccess, CCenterNetObj *pNetObj) {
    LOG_ERROR("center on connect {},{}", bSuccess, pNetObj->GetUID());
    if (bSuccess) {
        m_isRun = true;
        Register();
    }
}

void CCenterClientMgr::OnCloseClient(CCenterNetObj *pNetObj) {
    LOG_ERROR("center OnClose:{}", pNetObj->GetUID());
    m_isRun = false;
}

bool CCenterClientMgr::IsRun() {
    return m_isRun;
}

void
CCenterClientMgr::SendMsg2Center(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin, uint8_t route,
                                 uint32_t routeID) {
    if (!m_isRun || m_pNetObj == NULL)return;
    pkg_inner::SendProtobufMsg(m_pNetObj, msg, msg_type, uin, route, routeID);
}

//服务器注册
int CCenterClientMgr::handle_msg_register_svr_rep() {
    net::svr::msg_register_center_svr_rep msg;
    PARSE_MSG(msg);

    LOG_DEBUG("center server register result :{}", msg.result());
    if (msg.result() == 1) {
        CCenterClientMgr::Instance().RegisterRep(_pNetObj->GetUID(), true);
    } else {
        CCenterClientMgr::Instance().RegisterRep(_pNetObj->GetUID(), false);
        LOG_ERROR("center server register fail {} -->:{}", _pNetObj->GetUID(), CApplication::Instance().GetServerID());
    }
    return 0;
}

//更新服务器列表
int CCenterClientMgr::handle_msg_server_list_rep() {
    net::svr::msg_server_list_rep msg;
    PARSE_MSG(msg);

    LOG_DEBUG("center server rep svrlist :{}", msg.server_list_size());


    return 0;

}









