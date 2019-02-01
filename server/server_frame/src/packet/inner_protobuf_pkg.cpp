//
// Created by Administrator on 2018/8/8.
//

#include <packet/protobuf_pkg.h>
#include <svrlib.h>
#include <packet/inner_protobuf_pkg.h>

using namespace svrlib;
using namespace Network;

namespace {

}

int GetInnerPacketLen(const uint8_t *pData, uint16_t wLen) {
    if (pData == NULL) {
        return -1;
    }
    if (wLen < INNER_HEADER_SIZE) {
        return -1;
    }
    inner_header_t *head = (inner_header_t *) pData;
    if (head->datalen < INNER_MAX_DATA_SIZE) {
        return head->datalen + INNER_HEADER_SIZE;
    }
    LOG_ERROR("max packet len:{},uid:{},cmd:{}", head->datalen, head->uin, head->cmd);
    return -1;
}

bool SendInnerMsg(NetworkObject *pNetObj, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin,
                  uint8_t route, uint32_t routeMain, uint32_t routeSub) {
    if (pNetObj == NULL) {
        return false;
    }
    string sstr;
    msg->SerializeToString(&sstr);
    return SendInnerMsg(pNetObj, sstr.c_str(), sstr.length(), msg_type, uin, route, routeMain, routeSub);
}

bool
SendInnerMsg(NetworkObject *pNetObj, const void *msg, uint16_t msg_len, uint16_t msg_type, uint32_t uin,
             uint8_t route, uint32_t routeMain, uint32_t routeSub) {
    if (pNetObj == NULL) {
        return false;
    }
    static inner_protobuf pkt;
    memset(&pkt, 0, sizeof(pkt));
    pkt.header.cmd = msg_type;
    pkt.header.uin = uin;
    pkt.header.route = route;
    pkt.header.routeMain = routeMain;
    pkt.header.routeSub = routeSub;

    if (msg_len >= INNER_MAX_DATA_SIZE) {
        LOG_ERROR("msg length more than max length:{}", msg_len);
        return false;
    }
    memcpy((void *) pkt.protobuf, msg, msg_len);
    pkt.header.datalen = msg_len;

    return pNetObj->Send((uint8_t *) &pkt.header, pkt.header.datalen + INNER_HEADER_SIZE);
}

int CInnerMsgHanlde::OnHandleClientMsg(NetworkObject *pNetObj, uint8_t *pData, size_t uiDataLen) {
    if (pData == NULL)
        return -1;
    inner_header_t *head = (inner_header_t *) pData;
    if (head->datalen > (uiDataLen - INNER_HEADER_SIZE)) {
        LOG_ERROR("msg length is not right:{}--{}", uiDataLen, head->datalen);
        return -1;
    }
    _pNetObj = pNetObj;
    _head = head;
    _pkt_buf = pData + INNER_HEADER_SIZE;
    _buf_len = head->datalen;
    _cmd = head->cmd;

    return OnRecvClientMsg();

}






