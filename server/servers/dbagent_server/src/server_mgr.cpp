

#include "server_mgr.h"
#include "msg_define.pb.h"
#include "game_define.h"

using namespace svrlib;
using namespace Network;

namespace {

}

CServerClient::CServerClient(const net::svr::server_info& info, NetworkObject* pNetObj)
{
    m_info = info;
    m_pNetObj = pNetObj;
    m_pNetObj->SetUID(info.svrid());
}

CServerClient::~CServerClient()
{

}

void CServerClient::SendMsg(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin)
{
    pkg_inner::SendProtobufMsg(m_pNetObj, msg, msg_type, uin,0,0);
}

void CServerClient::SendMsg(const uint8_t* pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin)
{
    pkg_inner::SendBuffMsg(m_pNetObj, pkt_buf, buf_len, msg_type, uin,0,0);
}

NetworkObject* CServerClient::GetNetObj()
{
    return m_pNetObj;
}

uint16_t CServerClient::GetSvrID()
{
    return m_info.svrid();
}

uint16_t CServerClient::GetSvrType()
{
    return m_info.svr_type();
}

uint16_t CServerClient::GetGameType()
{
    return m_info.game_type();
}

uint16_t CServerClient::GetGameSubType()
{
    return m_info.game_subtype();
}

//--------------------------------------------------------------------------------------------
CServerMgr::CServerMgr()
        :m_timer(this)
{
    m_mpServers.clear();
    m_msgMinCount = 0;
    m_lastCountTime = 0;

    bind_handler(this, net::S2DBA_MSG_REGISTER_DBA, &CServerMgr::handle_msg_register_svr);
}

CServerMgr::~CServerMgr()
{
}

void CServerMgr::OnTimer()
{
    if (getSysTime()-m_lastCountTime>MINUTE) {
        m_msgMaxCount = std::max(m_msgMaxCount, m_msgMinCount);
        if (m_msgMinCount>0) {
            LOG_DEBUG("route msg count :msg {},max msg {}", m_msgMinCount, m_msgMaxCount);
            m_msgMinCount = 0;
        }
        m_lastCountTime = getSysTime();
    }
    CApplication::Instance().schedule(&m_timer, MINUTE*1000);
}

bool CServerMgr::Init()
{
    CApplication::Instance().schedule(&m_timer, MINUTE*1000);

    return true;
}

void CServerMgr::ShutDown()
{
    m_timer.cancel();
}

bool CServerMgr::AddServer(NetworkObject* pNetObj, const net::svr::server_info& info)
{
    auto pClient = std::make_shared<CServerClient>(info, pNetObj);
    pNetObj->SetUID(info.svrid());

    m_mpServers.insert(make_pair(info.svrid(), pClient));

    LOG_DEBUG("add server :svrid:{}--gameType:{},server size:{}", info.svrid(), info.game_type(), m_mpServers.size());
    return true;
}

void CServerMgr::RemoveServer(NetworkObject* pNetObj)
{
    for (auto& it : m_mpServers) {
        auto pServer = it.second;
        if (pServer->GetNetObj()==pNetObj) {
            LOG_DEBUG("remove server:{},server size:{}", it.first, m_mpServers.size());
            m_mpServers.erase(it.first);
            pNetObj->SetUID(0);
            break;
        }
    }
}

shared_ptr<CServerClient> CServerMgr::GetServerBySocket(NetworkObject* pNetObj)
{
    return GetServerBySvrID(pNetObj->GetUID());
}

shared_ptr<CServerClient> CServerMgr::GetServerBySvrID(uint16_t svrID)
{
    auto it = m_mpServers.find(svrID);
    if (it!=m_mpServers.end()) {
        return it->second;
    }
    return nullptr;
}

void CServerMgr::SendMsg2Server(uint16_t svrID, const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin)
{
    auto pServer = GetServerBySvrID(svrID);
    if (pServer==nullptr) {
        LOG_DEBUG("send msg svr is not exist £º{}", svrID);
        return;
    }
    pServer->SendMsg(msg, msg_type, uin);
}

void
CServerMgr::SendMsg2Server(uint16_t svrID, const uint8_t* pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin)
{
    auto pServer = GetServerBySvrID(svrID);
    if (pServer==nullptr) {
        LOG_DEBUG("send msg svr is not exist£º{}", svrID);
        return;
    }
    pServer->SendMsg(pkt_buf, buf_len, msg_type, uin);
}

int CServerMgr::OnRecvClientMsg()
{
    m_msgMinCount++;
    return CProtobufHandleBase::OnRecvClientMsg();
}

//·þÎñÆ÷×¢²á
int CServerMgr::handle_msg_register_svr()
{
    net::svr::msg_register_dbagent_svr_req msg;
    PARSE_MSG(msg);

    LOG_DEBUG("Server Register svrid:{}--svrType {}--gameType:{}--subType:{}", msg.info().svrid(),
            msg.info().svr_type(),
            msg.info().game_type(), msg.info().game_subtype());
    net::svr::msg_register_dbagent_svr_rep repmsg;

    bool bRet = AddServer(_pNetObj, msg.info());
    if (!bRet) {
        LOG_ERROR("Register Server fail svrid:{}", msg.info().svrid());
    }
    repmsg.set_result(bRet);

    pkg_inner::SendProtobufMsg(_pNetObj, &repmsg, net::DBA2S_MSG_REGISTER_DBA_REP, 0,0,0);

    return 0;
}
























