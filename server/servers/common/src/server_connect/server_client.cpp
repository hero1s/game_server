
#include "server_client.h"
#include "msg_define.pb.h"
#include "game_define.h"

CServerClient::CServerClient(const net::svr::server_info &info, const TCPConnPtr &conn) {
    m_info = info;
    m_pConnPtr = conn;
    m_pConnPtr->SetUID(info.svrid());
}

CServerClient::~CServerClient() {

}

void CServerClient::SendMsg(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uid,
                            uint8_t s_ser_type, uint32_t s_ser_id, uint8_t d_ser_type, uint32_t d_ser_id) {
    pkg_inner::SendProtobufMsg(m_pConnPtr, msg, msg_type, uid, s_ser_type, s_ser_id, d_ser_type, d_ser_id);
}

void CServerClient::SendMsg(const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uid,
                            uint8_t s_ser_type, uint32_t s_ser_id, uint8_t d_ser_type, uint32_t d_ser_id) {
    pkg_inner::SendBuffMsg(m_pConnPtr, pkt_buf, buf_len, msg_type, uid, s_ser_type, s_ser_id, d_ser_type, d_ser_id);
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

string CServerClient::GetUUID() {
    return m_info.uuid();
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
    if (time::getSysTime() - m_lastCountTime > MINUTE) {
        m_msgMaxCount = std::max(m_msgMaxCount, m_msgMinCount);
        if (m_msgMinCount > 0) {
            LOG_DEBUG("route msg count :msg {},max msg {}", m_msgMinCount, m_msgMaxCount);
            m_msgMinCount = 0;
        }
        m_lastCountTime = time::getSysTime();
    }
    CApplication::Instance().Schedule(&m_timer, MINUTE * 1000);
}

bool CServerClientMgr::Init() {
    CApplication::Instance().Schedule(&m_timer, MINUTE * 1000);

    return true;
}

void CServerClientMgr::ShutDown() {
    m_timer.cancel();
}

bool CServerClientMgr::AddServer(const TCPConnPtr &conn, const net::svr::server_info &info) {
    auto pClient = std::make_shared<CServerClient>(info, conn);
    conn->SetUID(info.svrid());
    m_mpServers.insert(make_pair(info.svrid(), pClient));
    LOG_DEBUG("add server :svrid:{}--gameType:{},server size:{}", info.svrid(), info.game_type(), m_mpServers.size());
    UpdateServerList();
    return true;
}

void CServerClientMgr::RemoveServer(const TCPConnPtr &conn) {
    for (auto &it : m_mpServers) {
        auto pServer = it.second;
        if (pServer->GetNetObj()->GetUID() == conn->GetUID()) {
            LOG_DEBUG("remove server:{},server size:{}", it.first, m_mpServers.size());
            m_mpServers.erase(it.first);
            conn->SetUID(0);
            break;
        }
    }
    UpdateServerList();
}

shared_ptr<CServerClient> CServerClientMgr::GetServerBySocket(const TCPConnPtr &conn) {
    return GetServerBySvrID(conn->GetUID());
}

shared_ptr<CServerClient> CServerClientMgr::GetServerBySvrID(uint16_t svrID) {
    auto it = m_mpServers.find(svrID);
    if (it != m_mpServers.end()) {
        return it->second;
    }
    LOG_ERROR("server id is null:{}",svrID);
    return nullptr;
}

void
CServerClientMgr::SendMsg2Server(uint16_t svrID, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uid,
                                 uint8_t s_ser_type, uint32_t s_ser_id, uint8_t d_ser_type, uint32_t d_ser_id) {
    auto pServer = GetServerBySvrID(svrID);
    if (pServer == nullptr) {
        LOG_DEBUG("send msg svr is not exist ��{}", svrID);
        return;
    }
    LOG_DEBUG("SendMsg2Svr:msgType:{},uid:{},s_ser_type:{},s_ser_id:{},d_ser_type:{},d_ser_id:{}",msg_type, uid, s_ser_type, s_ser_id, d_ser_type, d_ser_id);
    pServer->SendMsg(msg, msg_type, uid, s_ser_type, s_ser_id, d_ser_type, d_ser_id);
}

void CServerClientMgr::SendMsg2Server(uint16_t svrID, const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uid,
                                      uint8_t s_ser_type, uint32_t s_ser_id, uint8_t d_ser_type, uint32_t d_ser_id) {
    auto pServer = GetServerBySvrID(svrID);
    if (pServer == nullptr) {
        LOG_DEBUG("send msg svr is not exist��{}", svrID);
        return;
    }
    LOG_DEBUG("SendMsg2Svr:msgType:{},uid:{},s_ser_type:{},s_ser_id:{},d_ser_type:{},d_ser_id:{}",msg_type, uid, s_ser_type, s_ser_id, d_ser_type, d_ser_id);
    pServer->SendMsg(pkt_buf, buf_len, msg_type, uid, s_ser_type, s_ser_id, d_ser_type, d_ser_id);
}

void CServerClientMgr::SendMsg2AllGameServer(uint16_t gameType, const google::protobuf::Message *msg, uint16_t msg_type,
                                             uint32_t uid, uint8_t s_ser_type, uint32_t s_ser_id, uint8_t d_ser_type,
                                             uint32_t d_ser_id) {
    for (auto &it : m_mpServers) {
        auto pServer = it.second;
        if (pServer->GetSvrType() == gameType) {
            pServer->SendMsg(msg, msg_type, uid, s_ser_type, s_ser_id, d_ser_type, d_ser_id);
        }
    }
}

void
CServerClientMgr::SendMsg2AllGameServer(uint16_t gameType, const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type,
                                        uint32_t uid,
                                        uint8_t s_ser_type, uint32_t s_ser_id, uint8_t d_ser_type, uint32_t d_ser_id) {
    for (auto &it : m_mpServers) {
        auto pServer = it.second;
        if (pServer->GetSvrType() == gameType) {
            pServer->SendMsg(pkt_buf, buf_len, msg_type, uid, s_ser_type, s_ser_id, d_ser_type, d_ser_id);
        }
    }
}

// ȫ���㲥
void CServerClientMgr::SendMsg2All(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uid,
                                   uint8_t s_ser_type, uint32_t s_ser_id, uint8_t d_ser_type, uint32_t d_ser_id) {
    for (auto &it : m_mpServers) {
        auto pServer = it.second;
        pServer->SendMsg(msg, msg_type, uid, s_ser_type, s_ser_id, d_ser_type, d_ser_id);
    }
}

void CServerClientMgr::SendMsg2All(const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uid,
                                   uint8_t s_ser_type, uint32_t s_ser_id, uint8_t d_ser_type, uint32_t d_ser_id) {
    for (auto &it : m_mpServers) {
        auto pServer = it.second;
        pServer->SendMsg(pkt_buf, buf_len, msg_type, uid, s_ser_type, s_ser_id, d_ser_type, d_ser_id);
    }
}

// ���·������б��ȫ��������
void CServerClientMgr::UpdateServerList() {
    net::svr::msg_server_list_rep svrList;
    for (auto &it : m_mpServers) {
        auto pServer = it.second;
        net::svr::server_info *info = svrList.add_server_list();
        *info = pServer->m_info;
    }

    SendMsg2All(&svrList, net::svr::S2S_MSG_SERVER_LIST_REP, 0,0,0,0,0);
}

// ��ȡ���з������б�
void CServerClientMgr::GetAllServerInfo(vector<shared_ptr<CServerClient>> &svrlist) {
    for (auto &it : m_mpServers) {
        svrlist.push_back(it.second);
    }
}

int CServerClientMgr::OnRecvClientMsg() {
    m_msgMinCount++;
    if (m_head->s_ser_type > 0) {
        return OnRouteDispMsg();
    }

    return CProtobufHandleBase::OnRecvClientMsg();
}

//·�ɷַ���Ϣ
int CServerClientMgr::OnRouteDispMsg() {
    if (m_head->d_ser_type != 0) {
        if (m_head->d_ser_id != 0) {
            LOG_DEBUG("disp to one server:{}--{}--{}", m_head->msgID, m_head->d_ser_type, m_head->d_ser_id);
            SendMsg2Server(m_head->d_ser_id, m_pkt_buf, m_buf_len, m_head->msgID, m_head->uid,m_head->s_ser_type,m_head->s_ser_id,m_head->d_ser_type,m_head->d_ser_id);
        } else {
            LOG_DEBUG("disp to all server {}", m_head->d_ser_type);
            SendMsg2AllGameServer(m_head->d_ser_type, m_pkt_buf, m_buf_len, m_head->msgID, m_head->uid,m_head->s_ser_type,m_head->s_ser_id,m_head->d_ser_type,m_head->d_ser_id);
        }
    } else {
        LOG_ERROR("disp error msg:{}", m_head->msgID);
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

    bool bRet = AddServer(m_connPtr, msg.info());
    if (!bRet) {
        LOG_ERROR("Register Server fail svrid:{}", msg.info().svrid());
    }
    repmsg.set_result(bRet);

    pkg_inner::SendProtobufMsg(m_connPtr, &repmsg, net::svr::S2S_MSG_REGISTER_REP, 0, 0, 0, 0, 0);

    return 0;
}






