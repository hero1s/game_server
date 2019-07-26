
#pragma once

#include "packet/protobuf_pkg.h"
#include "crypt/md5.h"

class CPlayer;

using namespace NetworkAsio;

class CHandleClientMsg : public CProtobufMsgHanlde, public AutoDeleteSingleton<CHandleClientMsg>
{
public:
	CHandleClientMsg();
	virtual ~CHandleClientMsg();

protected:
	// ÐÄÌø°ü
	int handle_msg_heart();
	// µÇÂ¼
	int handle_msg_login();

	CPlayer* GetPlayer(const TCPConnPtr& connPtr);
};



















