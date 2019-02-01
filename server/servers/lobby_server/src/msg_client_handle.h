
#pragma once

#include "packet/protobuf_pkg.h"
#include "crypt/md5.h"

class CPlayer;

using namespace Network;

class CHandleClientMsg : public CProtobufMsgHanlde, public AutoDeleteSingleton<CHandleClientMsg>
{
public:
	CHandleClientMsg();
	virtual ~CHandleClientMsg();

	virtual int OnRecvClientMsg(NetworkObject* pNetObj, const uint8_t* pkt_buf, uint16_t buf_len, PACKETHEAD* head);

protected:
	// ÐÄÌø°ü
	int handle_msg_heart(NetworkObject* pNetObj, const uint8_t* pkt_buf, uint16_t buf_len, PACKETHEAD* head);
	// µÇÂ¼
	int handle_msg_login(NetworkObject* pNetObj, const uint8_t* pkt_buf, uint16_t buf_len, PACKETHEAD* head);

	CPlayer* GetPlayer(NetworkObject* pNetObj);
};



















