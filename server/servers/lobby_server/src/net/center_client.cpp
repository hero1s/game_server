
#include "center_client.h"
#include "data_cfg_mgr.h"
#include "player_mgr.h"
#include "player_base.h"
#include "snappy/snappy.h"

using namespace std;
using namespace svrlib;

//----------------------------------------------------------------------------------------------------------------------------

CCenterClientMgr::CCenterClientMgr()
{
    bind_handler(this, net::svr::GS2L_MSG_NOTIFY_PLAYER_LOBBY_LOGIN, &CCenterClientMgr::handle_msg_player_login_lobby);

    bind_handler(this, net::svr::GS2L_MSG_REPORT, &CCenterClientMgr::handle_msg_report);
    bind_handler(this, net::svr::GS2L_MSG_LEAVE_SVR, &CCenterClientMgr::handle_msg_leave_svr);

    bind_handler(this, net::svr::DBA2S_LOAD_PLAYER_DATA_REP, &CCenterClientMgr::handle_msg_load_data_rep);
}

CCenterClientMgr::~CCenterClientMgr() {

}
int CCenterClientMgr::OnRecvClientMsg()
{
    if (CProtobufHandleBase::OnRecvClientMsg()==1)
    {
        return route_to_client();
    }
    return 0;
}

// 异步执行sql
void CCenterClientMgr::AsyncExecSql(uint8_t dbType, string &sqlStr, bool isCompress) {
    LOG_DEBUG("async exec sql:{},{}", dbType, sqlStr);
    net::svr::msg_async_exec_sql msg;
    msg.set_db_type(dbType);
    msg.set_is_compress(isCompress ? 1 : 0);
    if(isCompress){
        string outSql;
        snappy::Compress(sqlStr.c_str(),sqlStr.length(),&outSql);
        msg.set_sql_str(outSql);
    }else{
        msg.set_sql_str(sqlStr);
    }
    SendMsg2Svr(&msg, net::svr::S2DBA_MSG_ASYNC_EXEC_SQL,0,emSERVER_TYPE_LOBBY, CApplication::Instance().GetServerID(),emSERVER_TYPE_DBA,0);
}

// 请求玩家数据
void CCenterClientMgr::LoadPlayerData(uint32_t uid, uint32_t data_type) {
    LOG_DEBUG("req load player data:uid {},type {}", uid, data_type);
    net::svr::msg_load_player_data msg;
    msg.set_uid(uid);
    msg.set_data_type(data_type);
    SendMsg2Svr(&msg, net::svr::S2DBA_LOAD_PLAYER_DATA,0,emSERVER_TYPE_LOBBY, CApplication::Instance().GetServerID(),emSERVER_TYPE_DBA,0);
}

// 保存玩家数据
void CCenterClientMgr::SavePlayerData(uint32_t uid, uint32_t data_type, const string &saveData) {
    LOG_DEBUG("save player data:uid {},type {},len {}", uid, data_type, saveData.length());
    net::svr::msg_save_player_data msg;
    msg.set_uid(uid);
    msg.set_data_type(data_type);
    msg.set_save_data(saveData);
    SendMsg2Svr(&msg, net::svr::S2DBA_SAVE_PLAYER_DATA,0,emSERVER_TYPE_LOBBY, CApplication::Instance().GetServerID(),emSERVER_TYPE_DBA,0);
}

// 转发给客户端
int CCenterClientMgr::route_to_client()
{
    LOG_DEBUG("中心服转发给客户端消息:uid:{}--cmd:{}",m_head->uid,m_head->msgID);
    auto pPlayer = GetPlayer();
    if (pPlayer != nullptr)
    {
        pPlayer->SendMsgToClient(m_pkt_buf, m_buf_len, m_head->msgID);
    }
    else
    {
        LOG_DEBUG("转发消息客户端不存在，通知游戏服断线:{}", m_head->uid);
        net::svr::msg_notify_net_state msg;
        msg.set_uid(m_head->uid);
        msg.set_state(0);
        msg.set_newip(0);
        msg.set_no_player(1);
        pkg_inner::SendProtobufMsg(m_connPtr, &msg, net::svr::L2GS_MSG_NOTIFY_NET_STATE, m_head->uid, emSERVER_TYPE_LOBBY,
                CApplication::Instance().GetServerID(),m_head->s_ser_type,m_head->s_ser_id);
    }
    return 0;
}
// 玩家登录通知
int CCenterClientMgr::handle_msg_player_login_lobby()
{
    net::svr::msg_notify_player_lobby_login loginmsg;
    PARSE_MSG(loginmsg);

    LOG_DEBUG("登录通知:lobby:{}--uid:{}", loginmsg.lobby_id(), loginmsg.uid());
    auto pPlayer = std::dynamic_pointer_cast<CPlayer>(CPlayerMgr::Instance().GetPlayer(loginmsg.uid()));
    if (pPlayer != nullptr && loginmsg.lobby_id() != CApplication::Instance().GetServerID())
    {
        LOG_DEBUG("玩家已经别的大厅服登录，回收玩家:{}", loginmsg.uid());
        pPlayer->SetNeedRecover(true);
        net::cli::msg_loginout_rep loginoutmsg;
        loginoutmsg.set_reason(net::RESULT_CODE_LOGIN_OTHER);
        pPlayer->SendMsgToClient(&loginoutmsg, net::S2C_MSG_LOGINOUT_REP);
    }
    return 0;
}

// 服务器上报信息
int CCenterClientMgr::handle_msg_report()
{
    net::svr::msg_report_svr_info msg;
    PARSE_MSG(msg);

    uint32_t players = msg.onlines();

    LOG_DEBUG("游戏服上报信息:sid {}--{}", m_connPtr->GetUID(), players);

    return 0;
}

// 返回大厅
int CCenterClientMgr::handle_msg_leave_svr()
{
    net::svr::msg_leave_svr msg;
    PARSE_MSG(msg);

    uint32_t uid = msg.uid();
    LOG_DEBUG("通知返回大厅:{}", uid);
    auto pPlayer = GetPlayer();
    if (pPlayer != nullptr) {
        pPlayer->BackLobby();
        pPlayer->NotifyClientBackLobby(RESULT_CODE_SUCCESS, RESULT_CODE_SUCCESS);
    } else {
        LOG_DEBUG("返回大厅玩家不存在:{}", uid);
    }
    return 0;
}


//请求数据返回
int CCenterClientMgr::handle_msg_load_data_rep() {
    net::svr::msg_load_player_data_rep msg;
    PARSE_MSG(msg);

    LOG_DEBUG("DBAgent load player data rep:{}", msg.uid());
    auto pPlayer = std::dynamic_pointer_cast<CPlayer>(CPlayerMgr::Instance().GetPlayer(msg.uid()));
    if (pPlayer != nullptr && pPlayer->GetPlayerState() == PLAYER_STATE_LOAD_DATA)
    {
        net::base_info baseInfo;//基础数据
        if (baseInfo.ParseFromString(msg.load_data()))
        {
            LOG_DEBUG("load base info success :{},datalen:{}", msg.uid(), msg.load_data().length());
            DUMP_PROTO_MSG_INFO(baseInfo);
        }
        else
        {
            LOG_ERROR("base info parase error :{} {}", msg.uid(), msg.load_data().length());
        }

        pPlayer->SetPlayerBaseData(baseInfo, false);
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




std::shared_ptr<CPlayer> CCenterClientMgr::GetPlayer(){
    auto pPlayer = std::dynamic_pointer_cast<CPlayer>(CPlayerMgr::Instance().GetPlayer(m_head->uid));
    return pPlayer;
}










