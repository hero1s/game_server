
#pragma once

#include "network/IOCPServer.h"
#include "network/NetworkObject.h"
#include "packet/protobuf_pkg.h"
#include "packet/inner_protobuf_pkg.h"

using namespace Network;
using namespace svrlib;

// client ¡¨Ω”
class CClientNetObj : public NetworkObject, public CNetworkObjPoolTemplete<CClientNetObj>
{
public:
	CClientNetObj();
	virtual ~CClientNetObj();

private:
	uint32_t m_seqNum;//–Ú¡–∫≈
	bool IsCanHandle(const uint8_t* pMsg, uint16_t wSize);
protected:
	virtual void OnAccept(uint32_t dwNetworkIndex);
	virtual void OnDisconnect();
	virtual int OnRecv(uint8_t* pMsg, uint16_t wSize);
	virtual void OnConnect(bool bSuccess, uint32_t dwNetworkIndex);
	virtual void OnLogString(const char* pszLog);

};





















































































