
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

// ������������
class CGateSvrMgr : public CServerClientMgr,public AutoDeleteSingleton<CGateSvrMgr> {
public:
    CGateSvrMgr();

    virtual ~CGateSvrMgr();

    virtual void OnTimer();

    virtual bool Init();

    virtual void ShutDown();

    // ���·������б��ȫ��������
    void UpdateServerList();

public:
    virtual int OnRecvClientMsg();

protected:
    //·�ɷַ���Ϣ
    int OnRouteDispMsg();

    //������ע��
    int handle_msg_register_svr();

private:


};




































