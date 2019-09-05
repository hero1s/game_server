
#include "dbagent_client.h"
#include "snappy/snappy.h"

using namespace std;
using namespace svrlib;

//----------------------------------------------------------------------------------------------------------------------------

CDBAgentClientMgr::CDBAgentClientMgr()
        : CSvrConnectorMgr() {

}

CDBAgentClientMgr::~CDBAgentClientMgr() {

}

// “Ï≤Ω÷¥––sql
void CDBAgentClientMgr::AsyncExecSql(uint8_t dbType, string &sqlStr, bool isCompress) {
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
    SendMsg2Svr(&msg, net::svr::S2DBA_MSG_ASYNC_EXEC_SQL);
}












