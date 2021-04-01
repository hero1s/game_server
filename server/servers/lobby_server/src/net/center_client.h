
#pragma once

#include "svrlib.h"
#include <string.h>
#include "packet/inner_protobuf_pkg.h"
#include <unordered_map>
#include "servers_msg.pb.h"
#include "server_connect/server_connector.h"
#include "player.h"

using namespace std;
using namespace svrlib;
using namespace Network;

class CCenterClientMgr : public CSvrConnectorMgr, public AutoDeleteSingleton<CCenterClientMgr> {
public:
    CCenterClientMgr();

    virtual ~CCenterClientMgr();

    virtual int OnRecvClientMsg();


    // 异步执行sql
    void AsyncExecSql(uint8_t dbType, string &sqlStr,bool isCompress = true);

    // 请求玩家数据
    void LoadPlayerData(uint32_t uid, uint32_t data_type);

    // 保存玩家数据
    void SavePlayerData(uint32_t uid, uint32_t data_type, const string &saveData);

protected:
    // 转发给客户端
    int route_to_client();
    // 玩家登录通知
    int handle_msg_player_login_lobby();
    // 服务器上报信息
    int handle_msg_report();
    // 返回大厅
    int handle_msg_leave_svr();

    //请求数据返回
    int handle_msg_load_data_rep();

    std::shared_ptr<CPlayer> GetPlayer();
};
/*
 * 大厅服务器-》》中心服的管理及消息处理
*/

