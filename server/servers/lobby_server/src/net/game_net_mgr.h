
#pragma once

#include "network/IOCPServer.h"
#include "network/NetworkObject.h"
#include "packet/protobuf_pkg.h"
#include "packet/inner_protobuf_pkg.h"

using namespace Network;
using namespace svrlib;

// client ����
class CClientNetObj : public NetworkObject
{
public:
	CClientNetObj();
	virtual ~CClientNetObj();

private:
	uint32_t m_seqNum;//���к�
	bool IsCanHandle(const uint8_t* pMsg, uint16_t wSize);

	virtual uint16_t GetHeadLen() {
		return PACKET_HEADER_SIZE;
	};

	virtual uint16_t GetPacketLen(const uint8_t *pData, uint16_t wLen) {
		return pkg_client::GetProtobufPacketLen(pData, wLen);
	};

protected:
	virtual void OnDisconnect();
	virtual int OnRecv(uint8_t* pMsg, uint16_t wSize);
	virtual void OnConnect(bool bSuccess);

};




















































































