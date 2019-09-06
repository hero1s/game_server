
#pragma once

#include "utility/noncopyable.hpp"
#include "svrlib.h"
#include <string.h>
#include "servers_msg.pb.h"
#include "server_connect/server_connector.h"
#include "packet/inner_protobuf_pkg.h"
#include <unordered_map>

using namespace std;
using namespace svrlib;
using namespace Network;

//��������
class CLobbyClient : public CSvrConnectorMgr {
public:
	CLobbyClient();

	virtual ~CLobbyClient();

};

/**
 * �������������
 */
class CLobbyMgr : public AutoDeleteSingleton<CLobbyMgr>
{
public:
	CLobbyMgr();

	void OnTimer();

	bool Init();

	// ������Ϣ���ͻ���
	bool SendMsg2Client(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin, uint16_t svrid);
	// ������Ϣ������
	bool SendMsg2Lobby(const google::protobuf::Message* msg, uint16_t msg_type, uint16_t svrid);
	// ������Ϣ��ȫ������
	bool SendMsg2AllLobby(const google::protobuf::Message* msg, uint16_t msg_type);
private:
	std::shared_ptr<CLobbyClient> GetLobbySvrBySvrID(uint16_t svrid);

	void ReportInfo2Lobby();

private:
	using MAP_LOBBY = unordered_map<uint32_t, std::shared_ptr<CLobbyClient>>;
	MAP_LOBBY                                        m_lobbySvrs;// ����������
	MemberTimerEvent<CLobbyMgr, &CLobbyMgr::OnTimer> m_timer;

};










