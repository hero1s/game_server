
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

// center connector连接
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

/******************中心服管理器****************************/
class CCenterClientMgr : public CInnerMsgHanlde, public AutoDeleteSingleton<CCenterClientMgr>
{
public:
	CCenterClientMgr();
	virtual ~CCenterClientMgr();

	void OnTimer();
	bool Init(int32_t ioKey,const net::server_info& info,string ip,uint32_t port);
	void Register();
	void RegisterRep(uint16_t svrid, bool rep);
	// 大厅连接回调
	void OnConnect(bool bSuccess, CCenterNetObj* pNetObj);
	// 大厅断开
	void OnCloseClient(CCenterNetObj* pNetObj);
	bool IsRun();

	// 设置消息处理handle
	void SetMsgHandle(CInnerMsgHanlde* pMsgHandle);
public:
	void SendMsg2Center(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin, uint8_t route = 0, uint32_t routeMain = 0, uint32_t routeSub = 0);

// 消息处理
public:
	virtual int OnRecvClientMsg(NetworkObject* pNetObj, const uint8_t* pkt_buf, uint16_t buf_len, INNERHEAD* head);

protected:
	//服务器注册
	int handle_msg_register_svr_rep(NetworkObject* pNetObj, const uint8_t* pkt_buf, uint16_t buf_len, INNERHEAD* head);
	//更新服务器列表
	int handle_msg_server_list_rep(NetworkObject* pNetObj, const uint8_t* pkt_buf, uint16_t buf_len, INNERHEAD* head);


private:
	MemberTimerEvent<CCenterClientMgr, &CCenterClientMgr::OnTimer> m_timer;
	CCenterNetObj* m_pNetObj;
	bool  m_isRun;
	net::server_info m_curSvrInfo;
	CInnerMsgHanlde* m_pMsgHandle;

};


