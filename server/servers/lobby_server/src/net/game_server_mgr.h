
#pragma once

#include <vector>
#include "svrlib.h"
#include <unordered_map>
#include <memory>
#include "packet/inner_protobuf_pkg.h"
#include "servers_msg.pb.h"
#include "server_connect/server_client.h"

using namespace std;
using namespace svrlib;
using namespace Network;

// 游戏服务管理器
class CGameServerMgr : public CServerClientMgr,public AutoDeleteSingleton<CGameServerMgr> {
public:
    CGameServerMgr();

    virtual ~CGameServerMgr();

    virtual bool Init();

    virtual void ShutDown();
private:
    // 服务器上报信息
    int handle_msg_report();

};




































