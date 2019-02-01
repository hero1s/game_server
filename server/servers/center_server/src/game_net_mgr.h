
#pragma once

#include "network/IOCPServer.h"
#include "network/NetworkObject.h"
#include "packet/inner_protobuf_pkg.h"

using namespace Network;
using namespace svrlib;

// server Á¬½Ó
class CCenterNetObj : public NetworkObject, public CNetworkObjPoolTemplete<CCenterNetObj>
{
public:
	CCenterNetObj();
	virtual ~CCenterNetObj();

protected:
	virtual void OnAccept(uint32_t dwNetworkIndex);
	virtual void OnDisconnect();
	virtual int OnRecv(uint8_t* pMsg, uint16_t wSize);
	virtual void OnConnect(bool bSuccess, uint32_t dwNetworkIndex);
	virtual void OnLogString(const char* pszLog);

};























































































