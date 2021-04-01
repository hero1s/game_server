
#pragma once

#include "packet/inner_protobuf_pkg.h"
#include "server_connect/server_connector.h"
#include "servers_msg.pb.h"
#include "svrlib.h"
#include <string.h>
#include <unordered_map>

using namespace std;
using namespace svrlib;
using namespace Network;

class CDBACenterClientMgr : public CSvrConnectorMgr, public AutoDeleteSingleton<CDBACenterClientMgr> {
public:
    CDBACenterClientMgr();

    virtual ~CDBACenterClientMgr();

    virtual int OnRecvClientMsg();

protected:
    //�첽ִ��sql
    int handle_async_exec_sql();

    //�����������
    int handle_load_player_data();

    //�����������
    int handle_save_player_data();
};
