
#include "server_client.h"
#include "msg_define.pb.h"
#include "game_define.h"

// server 连接
CSvrCliNetObj::CSvrCliNetObj(CServerClientMgr& host)
:m_host(host)
{

}

CSvrCliNetObj::~CSvrCliNetObj()
{

}
void CSvrCliNetObj::OnDisconnect()
{
    LOG_ERROR("server on disconnect:{}--{}", GetUID(), this->GetIP());
    m_host.RemoveServer(this);
}

int CSvrCliNetObj::OnRecv(uint8_t* pMsg, uint16_t wSize)
{
    AutoProfile ap(__FUNCTION__);
    return m_host.OnHandleClientMsg(this, pMsg, wSize);
}

void CSvrCliNetObj::OnConnect(bool bSuccess)
{
    LOG_DEBUG("server OnConnect,{},{}", bSuccess,this->GetIP());
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
CServerClientMgr::CServerClientMgr()
        :m_timer(this)
{
    m_mpServers.clear();
    m_msgMinCount = 0;
    m_lastCountTime = 0;
}

CServerClientMgr::~CServerClientMgr()
{
}

void CServerClientMgr::OnTimer()
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

bool CServerClientMgr::Init()
{
    CApplication::Instance().schedule(&m_timer, MINUTE*1000);

    return true;
}

void CServerClientMgr::ShutDown()
{
    m_timer.cancel();
}

bool CServerClientMgr::AddServer(NetworkObject* pNetObj, const net::svr::server_info& info)
{
    auto pClient = std::make_shared<CServerClient>(info, pNetObj);
    pNetObj->SetUID(info.svrid());
    m_mpServers.insert(make_pair(info.svrid(), pClient));
    LOG_DEBUG("add server :svrid:{}--gameType:{},server size:{}", info.svrid(), info.game_type(), m_mpServers.size());
    UpdateServerList();
    return true;
}

void CServerClientMgr::RemoveServer(NetworkObject* pNetObj)
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
    UpdateServerList();
}

shared_ptr<CServerClient> CServerClientMgr::GetServerBySocket(NetworkObject* pNetObj)
{
    return GetServerBySvrID(pNetObj->GetUID());
}

shared_ptr<CServerClient> CServerClientMgr::GetServerBySvrID(uint16_t svrID)
{
    auto it = m_mpServers.find(svrID);
    if (it!=m_mpServers.end()) {
        return it->second;
    }
    return nullptr;
}

void CServerClientMgr::SendMsg2Server(uint16_t svrID, const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin)
{
    auto pServer = GetServerBySvrID(svrID);
    if (pServer==nullptr) {
        LOG_DEBUG("send msg svr is not exist ：{}", svrID);
        return;
    }
    pServer->SendMsg(msg, msg_type, uin);
}

void
CServerClientMgr::SendMsg2Server(uint16_t svrID, const uint8_t* pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin)
{
    auto pServer = GetServerBySvrID(svrID);
    if (pServer==nullptr) {
        LOG_DEBUG("send msg svr is not exist：{}", svrID);
        return;
    }
    pServer->SendMsg(pkt_buf, buf_len, msg_type, uin);
}

void
CServerClientMgr::SendMsg2AllGameServer(uint16_t gameType, const google::protobuf::Message* msg, uint16_t msg_type,
        uint32_t uin)
{
    for (auto& it : m_mpServers) {
        auto pServer = it.second;
        if (pServer->GetSvrType()==gameType) {
            pServer->SendMsg(msg, msg_type, uin);
        }
    }
}

void
CServerClientMgr::SendMsg2AllGameServer(uint16_t gameType, const uint8_t* pkt_buf, uint16_t buf_len, uint16_t msg_type,
        uint32_t uin)
{
    for (auto& it : m_mpServers) {
        auto pServer = it.second;
        if (pServer->GetSvrType()==gameType) {
            pServer->SendMsg(pkt_buf, buf_len, msg_type, uin);
        }
    }
}

// 全服广播
void CServerClientMgr::SendMsg2All(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin)
{
    for (auto& it : m_mpServers) {
        auto pServer = it.second;
        pServer->SendMsg(msg, msg_type, uin);
    }
}

void CServerClientMgr::SendMsg2All(const uint8_t* pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin)
{
    for (auto& it : m_mpServers) {
        auto pServer = it.second;
        pServer->SendMsg(pkt_buf, buf_len, msg_type, uin);
    }
}








