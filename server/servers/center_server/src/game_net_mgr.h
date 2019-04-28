
#pragma once

#include "network/IOCPServer.h"
#include "network/NetworkObject.h"
#include "packet/inner_protobuf_pkg.h"

using namespace Network;
using namespace svrlib;

// server Á¬½Ó
class CCenterNetObj : public NetworkObject, public CNetworkObjPoolTemplete<CCenterNetObj> {
public:
    CCenterNetObj();

    virtual ~CCenterNetObj();

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
    virtual void OnDisconnect();

    virtual int OnRecv(uint8_t *pMsg, uint16_t wSize);

    virtual void OnConnect(bool bSuccess, uint32_t dwNetworkIndex);

};























































































