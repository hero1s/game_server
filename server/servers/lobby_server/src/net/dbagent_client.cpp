
#include "dbagent_client.h"
#include "data_cfg_mgr.h"
#include "player_mgr.h"
#include "player_base.h"
#include "player.h"

using namespace std;
using namespace svrlib;

//----------------------------------------------------------------------------------------------------------------------------

CDBAgentClientMgr::CDBAgentClientMgr()
        :CSvrConnectorMgr()
{

    bind_handler(this, net::svr::CS2S_MSG_REGISTER_REP, &CDBAgentClientMgr::handle_msg_register_svr_rep);
    bind_handler(this, net::svr::DBA2S_LOAD_PLAYER_DATA_REP, &CDBAgentClientMgr::handle_msg_load_data_rep);
}

CDBAgentClientMgr::~CDBAgentClientMgr()
{

}

// �첽ִ��sql
void CDBAgentClientMgr::AsyncExecSql(uint8_t dbType, string& sqlStr)
{
    net::svr::msg_async_exec_sql msg;
    msg.set_db_type(dbType);
    msg.set_sql_str(sqlStr);
    SendMsg2Svr(&msg, net::svr::S2DBA_MSG_ASYNC_EXEC_SQL);
}

// �����������
void CDBAgentClientMgr::LoadPlayerData(uint32_t uid, uint32_t data_type)
{
    net::svr::msg_load_player_data msg;
    msg.set_uid(uid);
    msg.set_data_type(data_type);
    SendMsg2Svr(&msg, net::svr::S2DBA_LOAD_PLAYER_DATA);
}

// �����������
void CDBAgentClientMgr::SavePlayerData(uint32_t uid, uint32_t data_type, const string& saveData)
{
    net::svr::msg_save_player_data msg;
    msg.set_uid(uid);
    msg.set_data_type(data_type);
    msg.set_save_data(saveData);
    SendMsg2Svr(&msg, net::svr::S2DBA_SAVE_PLAYER_DATA);
}

//������ע��
int CDBAgentClientMgr::handle_msg_register_svr_rep()
{
    net::svr::msg_register_svr_rep msg;
    PARSE_MSG(msg);

    LOG_DEBUG("dbagent server register result :{}", msg.result());
    if (msg.result()==1) {
        CDBAgentClientMgr::Instance().RegisterRep(_pNetObj->GetUID(), true);

        //test toney
        CPlayer* pPlayer = new CPlayer(PLAYER_TYPE_ONLINE);
        pPlayer->SetUID(110);
        CPlayerMgr::Instance().AddPlayer(pPlayer);
        pPlayer->OnLogin();

    }
    else {
        CDBAgentClientMgr::Instance().RegisterRep(_pNetObj->GetUID(), false);
        LOG_ERROR("dbagent server register fail {} -->:{}", _pNetObj->GetUID(), CApplication::Instance().GetServerID());
    }
    return 0;
}

//�������ݷ���
int CDBAgentClientMgr::handle_msg_load_data_rep()
{
    net::svr::msg_load_player_data_rep msg;
    PARSE_MSG(msg);

    LOG_DEBUG("DBAgent load player data rep:{}", msg.uid());
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(CPlayerMgr::Instance().GetPlayer(msg.uid()));
    if (pPlayer!=NULL && pPlayer->GetPlayerState()==PLAYER_STATE_LOAD_DATA) {
        net::base_info baseInfo;//��������
        if (baseInfo.ParseFromString(msg.load_data())) {
            LOG_DEBUG("load base info success :{},datalen:{}", msg.uid(), msg.load_data().length());
            DUMP_PROTO_MSG_INFO(baseInfo);
        }
        else {
            LOG_ERROR("base info parase error :{} {}", msg.uid(), msg.load_data().length());
        }

        pPlayer->SetPlayerBaseData(baseInfo);
        pPlayer->SetOfflineTime(0);
        pPlayer->SetLoadState(emACCDATA_TYPE_BASE);
        pPlayer->SetLoadState(emACCDATA_TYPE_MISS);//test
        if (pPlayer->IsLoadOver()) {
            pPlayer->OnGetAllData();
        }
    }
    else {
        LOG_DEBUG("the player is not find:{}", msg.uid());
    }

    return 0;
}








