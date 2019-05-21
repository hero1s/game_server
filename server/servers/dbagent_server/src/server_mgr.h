
#pragma once

#include <vector>
#include "svrlib.h"
#include <unordered_map>
#include <memory>
#include "packet/inner_protobuf_pkg.h"
#include "msg_define.pb.h"

using namespace std;
using namespace svrlib;
using namespace Network;

// ����������
class CServerClient
{
public:
	CServerClient(const net::svr::server_info& info, NetworkObject* pNetObj);
	virtual ~CServerClient();

	void SendMsg(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin);
	void SendMsg(const uint8_t* pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin);

	NetworkObject* GetNetObj();
	uint16_t GetSvrID();
	uint16_t GetSvrType();
	uint16_t GetGameType();
	uint16_t GetGameSubType();

public:
    net::svr::server_info m_info;
	NetworkObject* m_pNetObj;
};
// ������������
class CServerMgr : public CInnerMsgHanlde, public AutoDeleteSingleton<CServerMgr>
{
public:
	CServerMgr();

	~CServerMgr();

	void OnTimer();

	bool Init();

	void ShutDown();

	bool AddServer(NetworkObject* pNetObj, const net::svr::server_info& info);
	void RemoveServer(NetworkObject* pNetObj);

	shared_ptr<CServerClient> GetServerBySocket(NetworkObject* pNetObj);

    shared_ptr<CServerClient> GetServerBySvrID(uint16_t svrID);
	// ָ����Ϸ��������Ϣ
	void SendMsg2Server(uint16_t svrID, const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin);
	void SendMsg2Server(uint16_t svrID, const uint8_t* pkt_buf, uint16_t buf_len, uint16_t msg_type, uint32_t uin);

public:
	virtual int OnRecvClientMsg();

protected:
	//������ע��
	int handle_msg_register_svr();

private:
	using MAP_SERVERS = unordered_map<uint32_t, shared_ptr<CServerClient>>;
	MAP_SERVERS                                        m_mpServers;
	MemberTimerEvent<CServerMgr, &CServerMgr::OnTimer> m_timer;
	int32_t                                              m_msgMinCount;//��Ϣ�������
	int32_t                                              m_msgMaxCount;//��Ϣ��ֵ
	uint32_t                                             m_lastCountTime;//������ʱ��

};



































