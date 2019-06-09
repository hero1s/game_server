
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

class CCenterClientMgr : public CSvrConnectorMgr, public AutoDeleteSingleton<CCenterClientMgr> {
public:
    CCenterClientMgr();

    virtual ~CCenterClientMgr();
protected:
    //服务器注册
    int handle_msg_register_svr_rep();

    //更新服务器列表
    int handle_msg_server_list_rep();

private:


};


