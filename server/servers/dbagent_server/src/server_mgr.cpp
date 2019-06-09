

#include "server_mgr.h"
#include "game_define.h"
#include "dbmysql_mgr.h"

using namespace svrlib;
using namespace Network;

namespace {

}

//--------------------------------------------------------------------------------------------
CServerMgr::CServerMgr()
{

    bind_handler(this, net::svr::S2DBA_MSG_REGISTER_DBA, &CServerMgr::handle_msg_register_svr);
    bind_handler(this, net::svr::S2DBA_MSG_ASYNC_EXEC_SQL, &CServerMgr::handle_async_exec_sql);
    bind_handler(this, net::svr::S2DBA_LOAD_PLAYER_DATA, &CServerMgr::handle_load_player_data);
    bind_handler(this, net::svr::S2DBA_SAVE_PLAYER_DATA, &CServerMgr::handle_save_player_data);

}

CServerMgr::~CServerMgr() {
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
    if(dataType >= emACCDATA_TYPE_MAX){
        LOG_ERROR("player data type more than max type:{}",dataType);
        return 0;
    }

    std::string data;
    if (CPlayerCacheMgr::Instance().GetPlayerCacheData(msg.uid(), msg.data_type(), data)) {
        LOG_DEBUG("load player data from cache success:{},{},size:{}", msg.uid(), msg.data_type(), data.length());

        net::svr::msg_load_player_data_rep rep;
        rep.set_uid(msg.uid());
        rep.set_data_type(msg.data_type());
        rep.set_load_data(data);

        SendMsg2Server(sid, &rep, net::svr::DBA2S_LOAD_PLAYER_DATA_REP);
    } else {

        CDBMysqlMgr::Instance().AsyncLoadPlayerData(uid,dataType, [uid, sid, dataType,this](shared_ptr<CDBEventRep> &pRep) {
            LOG_DEBUG("OnLoadPlayerData:{}", uid);
            if (pRep->vecData.size() > 0) {
                auto &refRows = pRep->vecData[0];
                string strData = refRows["data"].as<string>();

                net::svr::msg_load_player_data_rep rep;
                rep.set_uid(uid);
                rep.set_data_type(dataType);
                rep.set_load_data(strData);

                this->SendMsg2Server(sid, &rep, net::svr::DBA2S_LOAD_PLAYER_DATA_REP);

                CPlayerCacheMgr::Instance().SetPlayerCacheData(uid,dataType,strData, false);
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

    CPlayerCacheMgr::Instance().SetPlayerCacheData(msg.uid(), msg.data_type(), msg.save_data(), true);

    return 0;
}






















