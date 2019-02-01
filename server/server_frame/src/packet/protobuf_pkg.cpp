#include <packet/protobuf_pkg.h>
#include <svrlib.h>
#include "packet/protobuf_pkg.h"
#include "zlib.h"
#include "crypt/xor256.h"
#include "helper/helper.h"

using namespace svrlib;
using namespace Network;

namespace {
    static uint8_t s_crypt_buff[PACKET_MAX_DATA_SIZE];       // Ω‚√‹buff
    static string s_crypt_key = "e2345678";                 // º”√‹√ÿ‘ø
}

int GetProtobufPacketLen(const uint8_t *pData, uint16_t wLen) {
    if (pData == NULL) {
        return -1;
    }
    if (wLen < PACKET_HEADER_SIZE) {
        return -1;
    }
    packet_header_t *head = (packet_header_t *) pData;
    if (head->datalen < PACKET_MAX_DATA_SIZE) {
        return head->datalen + PACKET_HEADER_SIZE;
    }
    LOG_ERROR("max packet len:{},uid:{},cmd:{},crypt:{},compress:{}", head->datalen, head->uin, head->cmd,
              head->encrypt, head->compress);
    return -1;
}

bool SendProtobufMsg(NetworkObject *pNetObj, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin,
                     uint8_t compress, uint8_t crypt) {
    if (pNetObj == NULL) {
        return false;
    }
    string sstr;
    msg->SerializeToString(&sstr);
    return SendProtobufMsg(pNetObj, sstr.c_str(), sstr.length(), msg_type, uin, compress, crypt);
}

bool
SendProtobufMsg(NetworkObject *pNetObj, const void *msg, uint16_t msg_len, uint16_t msg_type, uint32_t uin,
                uint8_t compress,
                uint8_t crypt) {
    if (pNetObj == NULL) {
        return false;
    }
    static packet_protobuf pkt;
    memset(&pkt, 0, sizeof(pkt));
    pkt.header.cmd = msg_type;
    pkt.header.uin = uin;
    pkt.header.encrypt = crypt;
    pkt.header.compress = compress;
    //—πÀı
    if (compress) {
        uint8_t *pLocalBuff = NULL;
        uint32_t localLen = 0;
        if ((localLen = CHelper::Compress((const char *) msg, msg_len, &pLocalBuff)) > 0) {
            if (localLen >= PACKET_MAX_DATA_SIZE) {
                LOG_ERROR("compress msg length more than max msg length:{}", localLen);
                return false;
            }
            memcpy(pkt.protobuf, pLocalBuff, localLen);
            pkt.header.datalen = localLen;
        } else {
            LOG_ERROR("msg compress fail:{}", localLen);
            return false;
        }
    } else {
        if (msg_len >= PACKET_MAX_DATA_SIZE) {
            LOG_ERROR("msg length more than max length:{}", msg_len);
            return false;
        }
        memcpy((void *) pkt.protobuf, msg, msg_len);
        pkt.header.datalen = msg_len;
    }
    //º”√‹
    if (pkt.header.encrypt) {
        if (pkt.header.datalen > 0) {
            CXOR256 crypt(s_crypt_key.c_str(), s_crypt_key.length());
            crypt.Crypt((char *) pkt.protobuf, (char *) s_crypt_buff, pkt.header.datalen);
            memcpy(pkt.protobuf, s_crypt_buff, pkt.header.datalen);
            //LOG_DEBUG("xor256 crypt:%d", pkt.header.datalen);
        }
    }

    //LOG_DEBUG("Socket Send Msg To Client-cmd:%d--len:%d",pkt.header.cmd,pkt.header.datalen);
    return pNetObj->Send((uint8_t *) &pkt.header, pkt.header.datalen + PACKET_HEADER_SIZE);

}

void SetEncryptKey(string key) {
    s_crypt_key = key;
}

int CProtobufMsgHanlde::OnHandleClientMsg(NetworkObject *pNetObj, uint8_t *pData, size_t uiDataLen) {
    if (pData == NULL)
        return -1;
    packet_header_t *head = (packet_header_t *) pData;
    if (head->datalen > (uiDataLen - PACKET_HEADER_SIZE)) {
        LOG_ERROR("msg length is not right:{}--{}", uiDataLen, head->datalen);
        return -1;
    }
    if (head->compress) {
        LOG_ERROR("can't handle compress msg");
        return -1;
    }
    if (head->encrypt) {//Ω‚√‹
        CXOR256 crypt(s_crypt_key.c_str(), s_crypt_key.length());
        crypt.Crypt((char *) pData + PACKET_HEADER_SIZE, (char *) s_crypt_buff, head->datalen);
        memcpy((pData + PACKET_HEADER_SIZE), s_crypt_buff, head->datalen);
        //LOG_DEBUG("xor256 crypt:%d", head->datalen);
    }
    _pNetObj = pNetObj;
    _head = head;
    _pkt_buf = pData + PACKET_HEADER_SIZE;
    _buf_len = head->datalen;
    _cmd = head->cmd;

    return OnRecvClientMsg();
}






