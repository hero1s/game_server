
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

// 服务器管理器
class CServerMgr : public CServerClientMgr, public CInnerMsgHanlde, public AutoDeleteSingleton<CServerMgr> {
public:
    CServerMgr();
    ~CServerMgr();

public:
    virtual int OnRecvClientMsg();
protected:
    //服务器注册
    int handle_msg_register_svr();

    //异步执行sql
    int handle_async_exec_sql();

    //请求玩家数据
    int handle_load_player_data();

    //保存玩家数据
    int handle_save_player_data();


private:


};




































