
#pragma once

#include <vector>
#include "svrlib.h"
#include <unordered_map>
#include <memory>
#include "packet/inner_protobuf_pkg.h"
#include "servers_msg.pb.h"
#include "server_connect/server_client.h"
#include "player.h"

using namespace std;
using namespace svrlib;
using namespace Network;

// ��Ϸ���������
class CGameServerMgr : public CServerClientMgr,public AutoDeleteSingleton<CGameServerMgr> {
public:
    CGameServerMgr();

    virtual ~CGameServerMgr();

    virtual int OnRecvClientMsg();

    virtual bool Init();

    virtual void ShutDown();
private:
    // ת�����ͻ���
    int route_to_client();
    // �������ϱ���Ϣ
    int handle_msg_report();
    // ���ش���
    int handle_msg_leave_svr();

    std::shared_ptr<CPlayer> GetPlayer();
};




































