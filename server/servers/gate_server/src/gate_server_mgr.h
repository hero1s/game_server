
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
class CGateSvrMgr : public CServerClientMgr,public AutoDeleteSingleton<CGateSvrMgr> {
public:
    CGateSvrMgr();

    virtual ~CGateSvrMgr();

    virtual void OnTimer();

    virtual bool Init();

    virtual void ShutDown();

    // 更新服务器列表给全部服务器
    void UpdateServerList();

public:
    virtual int OnRecvClientMsg();

protected:
    //路由分发消息
    int OnRouteDispMsg();

    //服务器注册
    int handle_msg_register_svr();

private:


};




































