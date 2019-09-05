
#pragma once

#include "svrlib.h"
#include <string.h>
#include "packet/inner_protobuf_pkg.h"
#include <unordered_map>
#include "servers_msg.pb.h"
#include "server_connect/server_connector.h"

using namespace std;
using namespace svrlib;
using namespace Network;

/******************dbagent������****************************/
class CDBAgentClientMgr : public CSvrConnectorMgr, public AutoDeleteSingleton<CDBAgentClientMgr> {
public:
    CDBAgentClientMgr();

    virtual ~CDBAgentClientMgr();

    // �첽ִ��sql
    void AsyncExecSql(uint8_t dbType, string &sqlStr,bool isCompress = true);

protected:




};


