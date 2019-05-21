
#pragma once

#include "svrlib.h"
#include <string.h>
#include "network/NetworkObject.h"
#include "packet/inner_protobuf_pkg.h"
#include <unordered_map>
#include "network/IOCPServer.h"
#include "network/tcp_connector.h"
#include "servers_msg.pb.h"

using namespace std;
using namespace svrlib;
using namespace Network;

// dbagent connector����
class CDBAgentNetObj : public CTcpConnector {
public:
    CDBAgentNetObj();

    virtual ~CDBAgentNetObj();

    virtual uint16_t GetHeadLen() {
        return INNER_HEADER_SIZE;
    };

    virtual uint16_t GetPacketLen(const uint8_t *pData, uint16_t wLen) {
        return pkg_inner::GetPacketLen(pData, wLen);
    };

    virtual uint16_t MaxTickPacket() {
        return 1000;
    }

protected:
    virtual void ConnectorOnDisconnect();

    virtual int OnRecv(uint8_t *pMsg, uint16_t wSize);

    virtual void ConnectorOnConnect(bool bSuccess, uint32_t dwNetworkIndex);

};

/******************dbagent������****************************/
class CDBAgentClientMgr : public CInnerMsgHanlde, public AutoDeleteSingleton<CDBAgentClientMgr> {
public:
    CDBAgentClientMgr();

    virtual ~CDBAgentClientMgr();

    void OnTimer();

    bool Init(int32_t ioKey, const net::svr::server_info &info, string ip, uint32_t port);

    void Register();

    void RegisterRep(uint16_t svrid, bool rep);

    // �������ӻص�
    void OnConnect(bool bSuccess, CDBAgentNetObj *pNetObj);

    // �����Ͽ�
    void OnCloseClient(CDBAgentNetObj *pNetObj);

    bool IsRun();

    // �첽ִ��sql
    void AsyncExecSql(uint8_t dbType, string &sqlStr);

    // �����������
    void LoadPlayerData(uint32_t uid, uint32_t data_type);

    // �����������
    void SavePlayerData(uint32_t uid, uint32_t data_type, const string &saveData);


public:
    void SendMsg2DBAgent(const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin = 0, uint8_t route = 0,
                         uint32_t routeID = 0);

protected:
    //������ע��
    int handle_msg_register_svr_rep();
    //�������ݷ���
    int handle_msg_load_data_rep();


private:
    MemberTimerEvent<CDBAgentClientMgr, &CDBAgentClientMgr::OnTimer> m_timer;
    CDBAgentNetObj *m_pNetObj;
    bool m_isRun;
    net::svr::server_info m_curSvrInfo;

};


