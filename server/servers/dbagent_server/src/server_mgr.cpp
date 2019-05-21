

#include "server_mgr.h"
#include "game_define.h"
#include "dbmysql_mgr.h"

using namespace svrlib;
using namespace Network;

namespace {

}

CServerClient::CServerClient(const net::svr::server_info &info, NetworkObject *pNetObj) {
    m_info = info;
    m_pNetObj = pNetObj;
    m_pNetObj->SetUID(info.svrid());
}

CServerClient::~CServerClient() {

}

void CServerClient::SendMsg(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin) {
    pkg_inner::SendProtobufMsg(m_pNetObj, msg, msg_type, uin, 0, 0);
}

void CServerClient::SendMsg(const uint8_t *pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin) {
    pkg_inner::SendBuffMsg(m_pNetObj, pkt_buf, buf_len, msg_type, uin, 0, 0);
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
CServerMgr::CServerMgr()
        : m_timer(this) {
    m_mpServers.clear();
    m_msgMinCount = 0;
    m_lastCountTime = 0;

    bind_handler(this, net::svr::S2DBA_MSG_REGISTER_DBA, &CServerMgr::handle_msg_register_svr);
    bind_handler(this, net::svr::S2DBA_MSG_ASYNC_EXEC_SQL, &CServerMgr::handle_async_exec_sql);
    bind_handler(this, net::svr::S2DBA_LOAD_PLAYER_DATA, &CServerMgr::handle_load_player_data);
    bind_handler(this, net::svr::S2DBA_SAVE_PLAYER_DATA, &CServerMgr::handle_save_player_data);

}

CServerMgr::~CServerMgr() {
}

void CServerMgr::OnTimer() {
    if (getSysTime() - m_lastCountTime > MINUTE) {
        m_msgMaxCount = std::max(m_msgMaxCount, m_msgMinCount);
        if (m_msgMinCount > 0) {
            LOG_DEBUG("route msg count :msg {},max msg {}", m_msgMinCount, m_msgMaxCount);
            m_msgMinCount = 0;
        }
        m_lastCountTime = getSysTime();
    }
    CApplication::Instance().schedule(&m_timer, MINUTE * 1000);
}

bool CServerMgr::Init() {
    CApplication::Instance().schedule(&m_timer, MINUTE * 1000);

    return true;
}

void CServerMgr::ShutDown() {
    m_timer.cancel();
}

bool CServerMgr::AddServer(NetworkObject *pNetObj, const net::svr::server_info &info) {
    auto pClient = std::make_shared<CServerClient>(info, pNetObj);
    pNetObj->SetUID(info.svrid());

    m_mpServers.insert(make_pair(info.svrid(), pClient));

    LOG_DEBUG("add server :svrid:{}--gameType:{},server size:{}", info.svrid(), info.game_type(), m_mpServers.size());
    return true;
}

void CServerMgr::RemoveServer(NetworkObject *pNetObj) {
    for (auto &it : m_mpServers) {
        auto pServer = it.second;
        if (pServer->GetNetObj() == pNetObj) {
            LOG_DEBUG("remove server:{},server size:{}", it.first, m_mpServers.size());
            m_mpServers.erase(it.first);
            pNetObj->SetUID(0);
            break;
        }
    }
}

shared_ptr<CServerClient> CServerMgr::GetServerBySocket(NetworkObject *pNetObj) {
    return GetServerBySvrID(pNetObj->GetUID());
}

shared_ptr<CServerClient> CServerMgr::GetServerBySvrID(uint16_t svrID) {
    auto it = m_mpServers.find(svrID);
    if (it != m_mpServers.end()) {
        return it->second;
    }
    return nullptr;
}

void CServerMgr::SendMsg2Server(uint16_t svrID, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin) {
    auto pServer = GetServerBySvrID(svrID);
    if (pServer == nullptr) {
        LOG_DEBUG("send msg svr is not exist ：{}", svrID);
        return;
    }
    pServer->SendMsg(msg, msg_type, uin);
}

int CServerMgr::OnRecvClientMsg() {
    m_msgMinCount++;
    return CProtobufHandleBase::OnRecvClientMsg();
}

//服务器注册
int CServerMgr::handle_msg_register_svr() {
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

    pkg_inner::SendProtobufMsg(_pNetObj, &repmsg, net::svr::DBA2S_MSG_REGISTER_DBA_REP, 0, 0, 0);

    return 0;
}

//异步执行sql
int CServerMgr::handle_async_exec_sql() {
    net::svr::msg_async_exec_sql msg;
    PARSE_MSG(msg);
    LOG_DEBUG("async exec sql:{}--{}", msg.db_type(), msg.sql_str());
    CDBMysqlMgr::Instance().AddAsyncSql(msg.db_type(), msg.sql_str());
    return 0;
}

//请求玩家数据
int CServerMgr::handle_load_player_data() {
    net::svr::msg_load_player_data msg;
    PARSE_MSG(msg);
    LOG_DEBUG("load player data:{}--{}", msg.uid(), msg.data_type());
    uint32_t uid = msg.uid();
    uint32_t dataType = msg.data_type();
    uint32_t sid = _pNetObj->GetUID();

    std::string data;
    if (CPlayerCacheMgr::Instance().GetPlayerCacheData(msg.uid(), msg.data_type(), data)) {
        LOG_DEBUG("load player data from cache success:{},{},size:{}", msg.uid(), msg.data_type(), data.length());

        net::svr::msg_load_player_data_rep rep;
        rep.set_uid(msg.uid());
        rep.set_data_type(msg.data_type());
        rep.set_load_data(data);

        SendMsg2Server(sid, &rep, net::svr::DBA2S_LOAD_PLAYER_DATA_REP);
    } else {

        CDBMysqlMgr::Instance().AsyncLoadPlayerData(uid, [uid, sid, dataType,this](shared_ptr<CDBEventRep> &pRep) {
            LOG_DEBUG("OnLoadPlayerData:{}", uid);
            if (pRep->vecData.size() > 0) {
                auto &refRows = pRep->vecData[0];
                net::base_info baseInfo;//基础数据
                string baseData = refRows["Base"].as<string>();
                if (baseInfo.ParseFromString(baseData)) {
                    LOG_DEBUG("load base info success :{},datalen:{}", uid, baseData.length());
                    DUMP_PROTO_MSG_INFO(baseInfo);
                } else {
                    LOG_ERROR("base info parase error :{} {}", uid, baseData.length());
                }
                uint32_t offlinetime = refRows["OfflineTime"];

                net::svr::msg_load_player_data_rep rep;
                rep.set_uid(uid);
                rep.set_data_type(dataType);
                rep.set_load_data(baseData);

                this->SendMsg2Server(sid, &rep, net::svr::DBA2S_LOAD_PLAYER_DATA_REP);
            }
        });
    }
    return 0;
}

//保存玩家数据
int CServerMgr::handle_save_player_data() {
    net::svr::msg_save_player_data msg;
    PARSE_MSG(msg);
    LOG_DEBUG("save player data {}--{},size:{}", msg.uid(), msg.data_type(), msg.save_data().length());

    CPlayerCacheMgr::Instance().SetPlayerCacheData(msg.uid(), msg.data_type(), msg.save_data());

    return 0;
}






















