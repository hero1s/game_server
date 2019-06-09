
#pragma once

#include "svrlib.h"
#include <string.h>
#include "network/NetworkObject.h"
#include "packet/inner_protobuf_pkg.h"
#include <unordered_map>
#include "network/IOCPServer.h"
#include "network/tcp_connector.h"
#include "servers_msg.pb.h"
#include "server_connect/server_connect.h"

using namespace std;
using namespace svrlib;
using namespace Network;

/******************dbagent管理器****************************/
class CDBAgentClientMgr : public CSvrConnectorMgr, public AutoDeleteSingleton<CDBAgentClientMgr> {
public:
    CDBAgentClientMgr();

    virtual ~CDBAgentClientMgr();

    // 异步执行sql
    void AsyncExecSql(uint8_t dbType, string &sqlStr);

    // 请求玩家数据
    void LoadPlayerData(uint32_t uid, uint32_t data_type);

    // 保存玩家数据
    void SavePlayerData(uint32_t uid, uint32_t data_type, const string &saveData);

protected:
    //服务器注册
    int handle_msg_register_svr_rep();
    //请求数据返回
    int handle_msg_load_data_rep();



};


