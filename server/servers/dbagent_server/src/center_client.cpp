
#include "center_client.h"
#include "game_define.h"
#include "dbmysql_mgr.h"
#include "utility/profile_manager.h"
#include "snappy/snappy.h"

using namespace std;
using namespace svrlib;

//----------------------------------------------------------------------------------------------------------------------------

CDBACenterClientMgr::CDBACenterClientMgr()
{
    bind_handler(this, net::svr::S2DBA_MSG_ASYNC_EXEC_SQL, &CDBACenterClientMgr::handle_async_exec_sql);
    bind_handler(this, net::svr::S2DBA_LOAD_PLAYER_DATA, &CDBACenterClientMgr::handle_load_player_data);
    bind_handler(this, net::svr::S2DBA_SAVE_PLAYER_DATA, &CDBACenterClientMgr::handle_save_player_data);
}

CDBACenterClientMgr::~CDBACenterClientMgr()
{
}
int CDBACenterClientMgr::OnRecvClientMsg()
{
    if (CProtobufHandleBase::OnRecvClientMsg() == 1) {
        LOG_ERROR("中心服消息未处理:{}",m_head->msgID);
        return 0;
    }
    return 0;
}
//异步执行sql
int CDBACenterClientMgr::handle_async_exec_sql()
{
    net::svr::msg_async_exec_sql msg;
    PARSE_MSG(msg);
    if (msg.is_compress() > 0) {
        std::string outdata;
        snappy::Uncompress(msg.sql_str().c_str(), msg.sql_str().size(), &outdata);
        LOG_DEBUG("async exec sql:{}--{}", msg.db_type(), outdata);
        CDBMysqlMgr::Instance().AddAsyncSql(msg.db_type(), outdata);
    } else {
        LOG_DEBUG("async exec sql:{}--{}", msg.db_type(), msg.sql_str());
        CDBMysqlMgr::Instance().AddAsyncSql(msg.db_type(), msg.sql_str());
    }
    return 0;
}

//请求玩家数据
int CDBACenterClientMgr::handle_load_player_data()
{
    net::svr::msg_load_player_data msg;
    PARSE_MSG(msg);
    LOG_DEBUG("load player data:{}--{}", msg.uid(), msg.data_type());
    uint32_t uid = msg.uid();
    uint32_t dataType = msg.data_type();

    INNERHEAD msgHead = *m_head;

    if (dataType >= emACCDATA_TYPE_MAX) {
        LOG_ERROR("player data type more than max type:{}", dataType);
        return 0;
    }

    std::string data;
    if (CPlayerCacheMgr::Instance().GetPlayerCacheData(msg.uid(), msg.data_type(), data)) {
        LOG_DEBUG("load player data from cache success:{},{},size:{}", msg.uid(), msg.data_type(), data.length());

        net::svr::msg_load_player_data_rep rep;
        rep.set_uid(msg.uid());
        rep.set_data_type(msg.data_type());
        rep.set_load_data(data);

        this->SendMsg2Svr(&rep, net::svr::DBA2S_LOAD_PLAYER_DATA_REP, uid,emSERVER_TYPE_DBA, CApplication::Instance().GetServerID(), msgHead.s_ser_type, msgHead.s_ser_id);
    } else {

        CDBMysqlMgr::Instance().AsyncLoadPlayerData(uid, dataType, [uid, msgHead, dataType, this](shared_ptr<CDBEventRep>& pRep) {
            LOG_DEBUG("OnLoadPlayerData:{},size:{}", uid, pRep->vecData.size());
            if (pRep->vecData.size() > 0) {
                auto& refRows = pRep->vecData[0];
                string strData = refRows["data"].as<string>();

                net::svr::msg_load_player_data_rep rep;
                rep.set_uid(uid);
                rep.set_data_type(dataType);
                rep.set_load_data(strData);

                this->SendMsg2Svr(&rep, net::svr::DBA2S_LOAD_PLAYER_DATA_REP, uid,emSERVER_TYPE_DBA, CApplication::Instance().GetServerID(), msgHead.s_ser_type, msgHead.s_ser_id);

                CPlayerCacheMgr::Instance().SetPlayerCacheData(uid, dataType, strData, false);
            }
        });
    }
    return 0;
}

//保存玩家数据
int CDBACenterClientMgr::handle_save_player_data()
{
    net::svr::msg_save_player_data msg;
    PARSE_MSG(msg);
    LOG_DEBUG("save player data {}--{},size:{}", msg.uid(), msg.data_type(), msg.save_data().length());

    CPlayerCacheMgr::Instance().SetPlayerCacheData(msg.uid(), msg.data_type(), msg.save_data(), true);

    return 0;
}
