
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

/******************dbagent������****************************/
class CDBAgentClientMgr : public CSvrConnectorMgr, public AutoDeleteSingleton<CDBAgentClientMgr> {
public:
    CDBAgentClientMgr();

    virtual ~CDBAgentClientMgr();

    // �첽ִ��sql
    void AsyncExecSql(uint8_t dbType, string &sqlStr);

    // �����������
    void LoadPlayerData(uint32_t uid, uint32_t data_type);

    // �����������
    void SavePlayerData(uint32_t uid, uint32_t data_type, const string &saveData);

protected:
    //������ע��
    int handle_msg_register_svr_rep();
    //�������ݷ���
    int handle_msg_load_data_rep();



};


