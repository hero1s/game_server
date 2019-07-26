
#include "server_client.h"
#include "msg_define.pb.h"
#include "game_define.h"

CServerClient::CServerClient(const net::svr::server_info &info, const TCPConnPtr& conn) {
    m_info = info;
    m_pConnPtr = conn;
    m_pConnPtr->SetUID(info.svrid());
}

CServerClient::~CServerClient() {

}

void CServerClient::SendMsg(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin) {
    pkg_inner::SendProtobufMsg(m_pConnPtr, msg, msg_type, uin, 0, 0);
}

void CServerClient::SendMsg(const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin) {
    pkg_inner::SendBuffMsg(m_pConnPtr, pkt_buf, buf_len, msg_type, uin, 0, 0);
}

TCPConnPtr CServerClient::GetNetObj() {
    return m_pConnPtr;
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
CServerClientMgr::CServerClientMgr()
        : m_timer(this) {
    m_mpServers.clear();
    m_msgMinCount = 0;
    m_lastCountTime = 0;

    bind_handler(this, net::svr::S2S_MSG_REGISTER, &CServerClientMgr::handle_msg_register_svr);
}

CServerClientMgr::~CServerClientMgr() {
}

void CServerClientMgr::OnTimer() {
    if (getSysTime() - m_lastCountTime > MINUTE)
    {
        m_msgMaxCount = std::max(m_msgMaxCount, m_msgMinCount);
        if (m_msgMinCount > 0)
        {
            LOG_DEBUG("route msg count :msg {},max msg {}", m_msgMinCount, m_msgMaxCount);
            m_msgMinCount = 0;
        }
        m_lastCountTime = getSysTime();
    }
    CApplication::Instance().schedule(&m_timer, MINUTE * 1000);
}

bool CServerClientMgr::Init() {
    CApplication::Instance().schedule(&m_timer, MINUTE * 1000);

    return true;
}

void CServerClientMgr::ShutDown() {
    m_timer.cancel();
}

bool CServerClientMgr::AddServer(const TCPConnPtr& conn, const net::svr::server_info &info) {
    auto pClient = std::make_shared<CServerClient>(info, conn);
    conn->SetUID(info.svrid());
    m_mpServers.insert(make_pair(info.svrid(), pClient));
    LOG_DEBUG("add server :svrid:{}--gameType:{},server size:{}", info.svrid(), info.game_type(), m_mpServers.size());
    UpdateServerList();
    return true;
}

void CServerClientMgr::RemoveServer(const TCPConnPtr& conn) {
    for (auto &it : m_mpServers)
    {
        auto pServer = it.second;
        if (pServer->GetNetObj()->GetUID() == conn->GetUID())
        {
            LOG_DEBUG("remove server:{},server size:{}", it.first, m_mpServers.size());
            m_mpServers.erase(it.first);
            conn->SetUID(0);
            break;
        }
    }
    UpdateServerList();
}

shared_ptr<CServerClient> CServerClientMgr::GetServerBySocket(const TCPConnPtr& conn) {
    return GetServerBySvrID(conn->GetUID());
}

shared_ptr<CServerClient> CServerClientMgr::GetServerBySvrID(uint16_t svrID) {
    auto it = m_mpServers.find(svrID);
    if (it != m_mpServers.end())
    {
        return it->second;
    }
    return nullptr;
}

void CServerClientMgr::SendMsg2Server(uint16_t svrID, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin) {
    auto pServer = GetServerBySvrID(svrID);
    if (pServer == nullptr)
    {
        LOG_DEBUG("send msg svr is not exist ��{}", svrID);
        return;
    }
    pServer->SendMsg(msg, msg_type, uin);
}

void CServerClientMgr::SendMsg2Server(uint16_t svrID, const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin) {
    auto pServer = GetServerBySvrID(svrID);
    if (pServer == nullptr)
    {
        LOG_DEBUG("send msg svr is not exist��{}", svrID);
        return;
    }
    pServer->SendMsg(pkt_buf, buf_len, msg_type, uin);
}

void CServerClientMgr::SendMsg2AllGameServer(uint16_t gameType, const google::protobuf::Message *msg, uint16_t msg_type,
                                             uint32_t uin) {
    for (auto &it : m_mpServers)
    {
        auto pServer = it.second;
        if (pServer->GetSvrType() == gameType)
        {
            pServer->SendMsg(msg, msg_type, uin);
        }
    }
}

void CServerClientMgr::SendMsg2AllGameServer(uint16_t gameType, const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type,
                                             uint32_t uin) {
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
void CServerClientMgr::SendMsg2All(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin) {
    for (auto &it : m_mpServers)
    {
        auto pServer = it.second;
        pServer->SendMsg(msg, msg_type, uin);
    }
}

void CServerClientMgr::SendMsg2All(const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin) {
    for (auto &it : m_mpServers)
    {
        auto pServer = it.second;
        pServer->SendMsg(pkt_buf, buf_len, msg_type, uin);
    }
}

// ���·������б��ȫ��������
void CServerClientMgr::UpdateServerList() {
    net::svr::msg_server_list_rep svrList;
    for (auto &it : m_mpServers)
    {
        auto pServer = it.second;
        net::svr::server_info *info = svrList.add_server_list();
        *info = pServer->m_info;
    }

    SendMsg2All(&svrList, net::svr::S2S_MSG_SERVER_LIST_REP, 0);
}

int CServerClientMgr::OnRecvClientMsg() {
    m_msgMinCount++;
    if (_head->route > 0)
    {
        return OnRouteDispMsg();
    }

    return CProtobufHandleBase::OnRecvClientMsg();
}

//·�ɷַ���Ϣ
int CServerClientMgr::OnRouteDispMsg() {
    switch (_head->route)
    {
        case emROUTE_TYPE_ALL_GAME:
        {
            //LOG_DEBUG("ת��ȫ����Ϸ��{}", head->cmd);
            SendMsg2AllGameServer(_head->routeID, _pkt_buf, _buf_len, _head->cmd, _head->uin);
            break;
        }
        case emROUTE_TYPE_ONE_GAME:
        {
            //LOG_DEBUG("ת��������Ϸ��{}--{}--{}", head->cmd, head->routeMain, head->routeSub);
            SendMsg2Server(_head->routeID, _pkt_buf, _buf_len, _head->cmd, _head->uin);
            break;
        }
        default:
        {
            LOG_ERROR("route type error :{}", _head->route);
            break;
        }
    }
    return 0;
}

//������ע��
int CServerClientMgr::handle_msg_register_svr() {
    net::svr::msg_register_svr_req msg;
    PARSE_MSG(msg);

    LOG_DEBUG("Server Register svrid:{}--svrType {}--gameType:{}--subType:{}", msg.info().svrid(),
              msg.info().svr_type(),
              msg.info().game_type(), msg.info().game_subtype());
    net::svr::msg_register_svr_rep repmsg;

    bool bRet = AddServer(_connPtr, msg.info());
    if (!bRet)
    {
        LOG_ERROR("Register Server fail svrid:{}", msg.info().svrid());
    }
    repmsg.set_result(bRet);

    pkg_inner::SendProtobufMsg(_connPtr, &repmsg, net::svr::S2S_MSG_REGISTER_REP, 0, 0, 0);

    return 0;
}






