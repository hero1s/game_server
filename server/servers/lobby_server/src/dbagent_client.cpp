
#include "dbagent_client.h"
#include "data_cfg_mgr.h"
#include "player_mgr.h"
#include "player_base.h"
#include "player.h"

using namespace std;
using namespace svrlib;

CDBAgentNetObj::CDBAgentNetObj() {

}

CDBAgentNetObj::~CDBAgentNetObj() {

}

void CDBAgentNetObj::ConnectorOnDisconnect() {
    LOG_DEBUG("dbagent ondisconnect");
    CDBAgentClientMgr::Instance().OnCloseClient(this);
}

int CDBAgentNetObj::OnRecv(uint8_t *pMsg, uint16_t wSize) {
    return CDBAgentClientMgr::Instance().OnHandleClientMsg(this, pMsg, wSize);
}

void CDBAgentNetObj::ConnectorOnConnect(bool bSuccess, uint32_t dwNetworkIndex) {
    LOG_DEBUG("dbagent server OnConnect :{},{}", bSuccess, dwNetworkIndex);
    CDBAgentClientMgr::Instance().OnConnect(bSuccess, this);
}

//----------------------------------------------------------------------------------------------------------------------------

CDBAgentClientMgr::CDBAgentClientMgr()
        : m_timer(this) {
    m_pNetObj = NULL;
    m_isRun = false;

    bind_handler(this, net::svr::DBA2S_MSG_REGISTER_DBA_REP, &CDBAgentClientMgr::handle_msg_register_svr_rep);
    bind_handler(this, net::svr::DBA2S_LOAD_PLAYER_DATA_REP, &CDBAgentClientMgr::handle_msg_load_data_rep);
}

CDBAgentClientMgr::~CDBAgentClientMgr() {

}

void CDBAgentClientMgr::OnTimer() {
    CApplication::Instance().schedule(&m_timer, 3000);
}

bool CDBAgentClientMgr::Init(int32_t ioKey, const net::svr::server_info &info, string ip, uint32_t port) {
    IOCPServer &iocpServer = CApplication::Instance().GetIOCPServer();

    CDBAgentNetObj *pNetObj = new CDBAgentNetObj();
    pNetObj->SetUID(info.svrid());
    pNetObj->Init(&iocpServer, ioKey, ip, port);

    m_curSvrInfo = info;
    m_pNetObj = pNetObj;
    m_isRun = false;

    CApplication::Instance().schedule(&m_timer, 3000);

    return true;
}

void CDBAgentClientMgr::Register() {
    net::svr::msg_register_dbagent_svr_req msg;
    net::svr::server_info *info = msg.mutable_info();
    *info = m_curSvrInfo;

    SendMsg2DBAgent(&msg, net::svr::S2DBA_MSG_REGISTER_DBA);
    LOG_DEBUG("register dbagent server svrid:{} svrtype:{}--gameType:{}", msg.info().svrid(), msg.info().svr_type(),
              msg.info().game_type());
}

void CDBAgentClientMgr::RegisterRep(uint16_t svrid, bool rep) {
    LOG_DEBUG("register dbagent server Rep svrid:{}--res:{}", svrid, rep);

    m_isRun = rep;
}

void CDBAgentClientMgr::OnConnect(bool bSuccess, CDBAgentNetObj *pNetObj) {
    LOG_ERROR("dbagent on connect {},{}", bSuccess, pNetObj->GetUID());
    if (bSuccess) {
        m_isRun = true;
        Register();
    }
}

void CDBAgentClientMgr::OnCloseClient(CDBAgentNetObj *pNetObj) {
    LOG_ERROR("dbagent OnClose:{}", pNetObj->GetUID());
    m_isRun = false;
}

bool CDBAgentClientMgr::IsRun() {
    return m_isRun;
}

// 异步执行sql
void CDBAgentClientMgr::AsyncExecSql(uint8_t dbType, string &sqlStr) {
    net::svr::msg_async_exec_sql msg;
    msg.set_db_type(dbType);
    msg.set_sql_str(sqlStr);
    SendMsg2DBAgent(&msg, net::svr::S2DBA_MSG_ASYNC_EXEC_SQL);
}

// 请求玩家数据
void CDBAgentClientMgr::LoadPlayerData(uint32_t uid, uint32_t data_type) {
    net::svr::msg_load_player_data msg;
    msg.set_uid(uid);
    msg.set_data_type(data_type);
    SendMsg2DBAgent(&msg, net::svr::S2DBA_LOAD_PLAYER_DATA);
}

// 保存玩家数据
void CDBAgentClientMgr::SavePlayerData(uint32_t uid, uint32_t data_type, const string &saveData) {
    net::svr::msg_save_player_data msg;
    msg.set_uid(uid);
    msg.set_data_type(data_type);
    msg.set_save_data(saveData);
    SendMsg2DBAgent(&msg, net::svr::S2DBA_SAVE_PLAYER_DATA);
}

void
CDBAgentClientMgr::SendMsg2DBAgent(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin, uint8_t route,
                                   uint32_t routeID) {
    if (!m_isRun || m_pNetObj == NULL)return;
    pkg_inner::SendProtobufMsg(m_pNetObj, msg, msg_type, uin, route, routeID);
}

//服务器注册
int CDBAgentClientMgr::handle_msg_register_svr_rep() {
    net::svr::msg_register_dbagent_svr_rep msg;
    PARSE_MSG(msg);

    LOG_DEBUG("dbagent server register result :{}", msg.result());
    if (msg.result() == 1) {
        CDBAgentClientMgr::Instance().RegisterRep(_pNetObj->GetUID(), true);

        //test toney
        CPlayer *pPlayer = new CPlayer(PLAYER_TYPE_ONLINE);
        pPlayer->SetUID(110);
        CPlayerMgr::Instance().AddPlayer(pPlayer);
        pPlayer->OnLogin();

    } else {
        CDBAgentClientMgr::Instance().RegisterRep(_pNetObj->GetUID(), false);
        LOG_ERROR("dbagent server register fail {} -->:{}", _pNetObj->GetUID(), CApplication::Instance().GetServerID());
    }
    return 0;
}

//请求数据返回
int CDBAgentClientMgr::handle_msg_load_data_rep() {
    net::svr::msg_load_player_data_rep msg;
    PARSE_MSG(msg);

    LOG_DEBUG("DBAgent load player data rep:{}",msg.uid());
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(CPlayerMgr::Instance().GetPlayer(msg.uid()));
    if (pPlayer != NULL && pPlayer->GetPlayerState() == PLAYER_STATE_LOAD_DATA)
    {
        net::base_info baseInfo;//基础数据
        if(baseInfo.ParseFromString(msg.load_data())){
            LOG_DEBUG("load base info success :{},datalen:{}",msg.uid(),msg.load_data().length());
            DUMP_PROTO_MSG_INFO(baseInfo);
        }else{
            LOG_ERROR("base info parase error :{} {}",msg.uid(),msg.load_data().length());
        }

        pPlayer->SetPlayerBaseData(baseInfo);
        pPlayer->SetOfflineTime(0);
        pPlayer->SetLoadState(emACCDATA_TYPE_BASE);
        pPlayer->SetLoadState(emACCDATA_TYPE_MISS);//test
        if (pPlayer->IsLoadOver())
        {
            pPlayer->OnGetAllData();
        }
    }
    else
    {
        LOG_DEBUG("the player is not find:{}", msg.uid());
    }

    return 0;
}









