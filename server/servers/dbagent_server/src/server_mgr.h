
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
using namespace NetworkAsio;

// ������������
class CServerMgr : public CServerClientMgr, public AutoDeleteSingleton<CServerMgr> {
public:
    CServerMgr();
    virtual ~CServerMgr();

protected:
    //�첽ִ��sql
    int handle_async_exec_sql();

    //�����������
    int handle_load_player_data();

    //�����������
    int handle_save_player_data();


private:


};




































