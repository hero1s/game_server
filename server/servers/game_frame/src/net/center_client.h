
#pragma once

#include "game_player.h"
#include "packet/inner_protobuf_pkg.h"
#include "server_connect/server_connector.h"
#include "servers_msg.pb.h"
#include "svrlib.h"
#include <string.h>
#include <unordered_map>

using namespace std;
using namespace svrlib;
using namespace Network;

class CCenterClientMgr : public CSvrConnectorMgr, public AutoDeleteSingleton<CCenterClientMgr> {
public:
    CCenterClientMgr();

    virtual ~CCenterClientMgr();

    virtual int OnRecvClientMsg();

protected:
    // 转发给客户端
    int route_to_client();

    std::shared_ptr<CGamePlayer> GetPlayer();
};
/*
 * 游戏服务器连接中心服务器的管理及消息处理
*/
