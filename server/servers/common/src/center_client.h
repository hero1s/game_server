
#pragma once

#include "svrlib.h"
#include <string.h>
#include "network/NetworkObject.h"
#include "packet/inner_protobuf_pkg.h"
#include <unordered_map>
#include "network/IOCPServer.h"
#include "network/tcp_connector.h"
#include "msg_define.pb.h"

using namespace std;
using namespace svrlib;
using namespace Network;

// center connector����
class CCenterNetObj : public CTcpConnector
{
public:
	CCenterNetObj();

	virtual ~CCenterNetObj();

protected:
	virtual void OnAccept(uint32_t dwNetworkIndex);

	virtual void ConnectorOnDisconnect();

	virtual int OnRecv(uint8_t* pMsg, uint16_t wSize);

	virtual void ConnectorOnConnect(bool bSuccess, uint32_t dwNetworkIndex);
	virtual void OnLogString(const char* pszLog);

};

/******************���ķ�������****************************/
class CCenterClientMgr : public CInnerMsgHanlde, public AutoDeleteSingleton<CCenterClientMgr>
{
public:
	CCenterClientMgr();
	virtual ~CCenterClientMgr();

	void OnTimer();
	bool Init(int32_t ioKey,const net::server_info& info,string ip,uint32_t port);
	void Register();
	void RegisterRep(uint16_t svrid, bool rep);
	// �������ӻص�
	void OnConnect(bool bSuccess, CCenterNetObj* pNetObj);
	// �����Ͽ�
	void OnCloseClient(CCenterNetObj* pNetObj);
	bool IsRun();

	// ������Ϣ����handle
	void SetMsgHandle(CInnerMsgHanlde* pMsgHandle);
public:
	void SendMsg2Center(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin, uint8_t route = 0, uint32_t routeMain = 0, uint32_t routeSub = 0);

// ��Ϣ����
public:
	virtual int OnRecvClientMsg(NetworkObject* pNetObj, const uint8_t* pkt_buf, uint16_t buf_len, INNERHEAD* head);

protected:
	//������ע��
	int handle_msg_register_svr_rep(NetworkObject* pNetObj, const uint8_t* pkt_buf, uint16_t buf_len, INNERHEAD* head);
	//���·������б�
	int handle_msg_server_list_rep(NetworkObject* pNetObj, const uint8_t* pkt_buf, uint16_t buf_len, INNERHEAD* head);


private:
	MemberTimerEvent<CCenterClientMgr, &CCenterClientMgr::OnTimer> m_timer;
	CCenterNetObj* m_pNetObj;
	bool  m_isRun;
	net::server_info m_curSvrInfo;
	CInnerMsgHanlde* m_pMsgHandle;

};


