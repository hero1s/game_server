
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


    // �첽ִ��sql
    void AsyncExecSql(uint8_t dbType, string &sqlStr,bool isCompress = true);

    // �����������
    void LoadPlayerData(uint32_t uid, uint32_t data_type);

    // �����������
    void SavePlayerData(uint32_t uid, uint32_t data_type, const string &saveData);

protected:
    // ת�����ͻ���
    int route_to_client();
    // ��ҵ�¼֪ͨ
    int handle_msg_player_login_lobby();
    // �������ϱ���Ϣ
    int handle_msg_report();
    // ���ش���
    int handle_msg_leave_svr();

    //�������ݷ���
    int handle_msg_load_data_rep();

    std::shared_ptr<CPlayer> GetPlayer();
};
/*
 * ����������-�������ķ��Ĺ�����Ϣ����
*/

