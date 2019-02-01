
#include "stdafx.h"
#include "center_mgr.h"
#include "msg_define.pb.h"

using namespace svrlib;
using namespace Network;

namespace {

}

CServerClient::CServerClient(const net::server_info &info, NetworkObject *pNetObj) {
    m_info = info;
    m_pNetObj = pNetObj;
    m_pNetObj->SetUID(info.svrid());
}

CServerClient::~CServerClient() {

}

void CServerClient::SendMsg(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin) {
    SendInnerMsg(m_pNetObj, msg, msg_type, uin);
}

void CServerClient::SendMsg(const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin) {
    SendInnerMsg(m_pNetObj, pkt_buf, buf_len, msg_type, uin);
}

NetworkObject *CServerClient::GetNetObj() {
    return m_pNetObj;
}

uint16_t CServerClient::GetSvrID() {
    return m_info.svrid();
}

uint16_t CServerClient::GetSvrType() {
    return m_info.svr_type();
}

uint16_t CServerClient::GetGameType() {
    return m_info.game_type();
}

uint16_t CServerClient::GetGameSubType() {
    return m_info.game_subtype();
}

//--------------------------------------------------------------------------------------------
CCenterMgr::CCenterMgr()
        : m_timer(this) {
    m_mpServers.clear();
    m_msgMinCount = 0;
    m_lastCountTime = 0;

    bind_handler(this, net::S2CS_MSG_REGISTER_CENTER, &CCenterMgr::handle_msg_register_svr);
}

CCenterMgr::~CCenterMgr() {
}

void CCenterMgr::OnTimer() {
    if (getSysTime() - m_lastCountTime > SECONDS_IN_MIN)
    {
        m_msgMaxCount = MAX(m_msgMaxCount, m_msgMinCount);

        LOG_DEBUG("��ת��Ϣ����:msg {},��ֵ��Ϣ {}", m_msgMinCount, m_msgMaxCount);
        m_msgMinCount = 0;
        m_lastCountTime = getSysTime();
    }
    CApplication::Instance().schedule(&m_timer, 1000);
}

bool CCenterMgr::Init() {
    CApplication::Instance().schedule(&m_timer, 1000);

    return true;
}

void CCenterMgr::ShutDown() {
    m_timer.cancel();
}

bool CCenterMgr::AddServer(NetworkObject *pNetObj, const net::server_info &info) {
    auto pClient = std::make_shared<CServerClient>(info, pNetObj);
    pNetObj->SetUID(info.svrid());

    m_mpServers.insert(make_pair(info.svrid(), pClient));

    LOG_DEBUG("add server :svrid:{}--gameType:{},server size:{}", info.svrid(), info.game_type(), m_mpServers.size());
    UpdateServerList();
    return true;
}

void CCenterMgr::RemoveServer(NetworkObject *pNetObj) {
    for (auto &it : m_mpServers)
    {
        auto pServer = it.second;
        if (pServer->GetNetObj() == pNetObj)
        {
            LOG_DEBUG("remove server:{},server size:{}", it.first, m_mpServers.size());
            m_mpServers.erase(it.first);
            pNetObj->SetUID(0);
            break;
        }
    }
    UpdateServerList();
}

shared_ptr<CServerClient> CCenterMgr::GetServerBySocket(NetworkObject *pNetObj) {
    return GetServerBySvrID(pNetObj->GetUID());
}

shared_ptr<CServerClient> CCenterMgr::GetServerBySvrID(uint16_t svrID) {
    auto it = m_mpServers.find(svrID);
    if (it != m_mpServers.end())
    {
        return it->second;
    }
    return nullptr;
}

void CCenterMgr::SendMsg2Server(uint16_t svrID, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin) {
    auto pServer = GetServerBySvrID(svrID);
    if (pServer == nullptr)
    {
        LOG_DEBUG("���͵���Ϣ�����������ڣ�{}", svrID);
        return;
    }
    pServer->SendMsg(msg, msg_type, uin);
}

void CCenterMgr::SendMsg2Server(uint16_t svrID, const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin) {
    auto pServer = GetServerBySvrID(svrID);
    if (pServer == nullptr)
    {
        LOG_DEBUG("���͵���Ϣ�����������ڣ�{}", svrID);
        return;
    }
    pServer->SendMsg(pkt_buf, buf_len, msg_type, uin);
}

void
CCenterMgr::SendMsg2AllGameServer(uint16_t gameType, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin) {
    for (auto &it : m_mpServers)
    {
        auto pServer = it.second;
        if (pServer->GetSvrType() == gameType)
        {
            pServer->SendMsg(msg, msg_type, uin);
        }
    }
}

void
CCenterMgr::SendMsg2AllGameServer(uint16_t gameType, const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin) {
    for (auto &it : m_mpServers)
    {
        auto pServer = it.second;
        if (pServer->GetSvrType() == gameType)
        {
            pServer->SendMsg(pkt_buf, buf_len, msg_type, uin);
        }
    }
}

// ȫ���㲥
void CCenterMgr::SendMsg2All(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin) {
    for (auto &it : m_mpServers)
    {
        auto pServer = it.second;
        pServer->SendMsg(msg, msg_type, uin);
    }
}

void CCenterMgr::SendMsg2All(const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin) {
    for (auto &it : m_mpServers)
    {
        auto pServer = it.second;
        pServer->SendMsg(pkt_buf, buf_len, msg_type, uin);
    }
}

// ���·������б��ȫ��������
void CCenterMgr::UpdateServerList() {
    net::msg_server_list_rep svrList;
    for (auto &it : m_mpServers)
    {
        auto pServer = it.second;
        net::server_info *info = svrList.add_server_list();
        *info = pServer->m_info;
    }

    SendMsg2All(&svrList,net::CS2S_MSG_SERVER_LIST_REP,0);
}

int CCenterMgr::OnRecvClientMsg(NetworkObject *pNetObj, const uint8_t *pkt_buf, uint16_t buf_len, INNERHEAD *head) {
    m_msgMinCount++;
    if (head->route > 0)
    {
        return OnRouteDispMsg(pNetObj, pkt_buf, buf_len, head);
    }

    auto it = m_handlers.find(head->cmd);
    if (it != m_handlers.end())
    {
        it->second(pNetObj, pkt_buf, buf_len, head);
    }
    else
    {
        LOG_ERROR("δ�����Э�鶨��:{}", head->cmd);
    }

    return 0;
}

//·�ɷַ���Ϣ
int CCenterMgr::OnRouteDispMsg(NetworkObject *pNetObj, const uint8_t *pkt_buf, uint16_t buf_len, INNERHEAD *head) {
    switch (head->route)
    {
        case emROUTE_TYPE_ALL_GAME:
        {
            //LOG_DEBUG("ת��ȫ����Ϸ��{}", head->cmd);
            SendMsg2AllGameServer(head->routeMain, pkt_buf, buf_len, head->cmd, head->uin);
            break;
        }
        case emROUTE_TYPE_ONE_GAME:
        {
            //LOG_DEBUG("ת��������Ϸ��{}--{}--{}", head->cmd, head->routeMain, head->routeSub);
            SendMsg2Server(head->routeMain, pkt_buf, buf_len, head->cmd, head->uin);
            break;
        }
        default:
        {
            LOG_ERROR("·�����ʹ���:{}", head->route);
            break;
        }
    }
    return 0;
}

//������ע��
int CCenterMgr::handle_msg_register_svr(NetworkObject *pNetObj, const uint8_t *pkt_buf, uint16_t buf_len, INNERHEAD *head) {
    net::msg_register_center_svr_req msg;
    PARSE_MSG_FROM_ARRAY(msg);

    LOG_DEBUG("Server Register svrid:{}--svrType {}--gameType:{}--subType:{}", msg.info().svrid(), msg.info().svr_type(),
              msg.info().game_type(), msg.info().game_subtype());
    net::msg_register_center_svr_rep repmsg;

    bool bRet = CCenterMgr::Instance().AddServer(pNetObj,msg.info());
    if (!bRet)
    {
        LOG_ERROR("Register Server fail svrid:{}", msg.info().svrid());
    }
    repmsg.set_result(bRet);

    SendInnerMsg(pNetObj, &repmsg, net::CS2S_MSG_REGISTER_CENTER_REP, 0);

    return 0;
}
























