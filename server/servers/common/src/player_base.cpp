

#include "player_base.h"
#include "data_cfg_mgr.h"
#include "common_logic.h"
#include "redis_mgr.h"
#include "helper/helper.h"
#include "packet/protobuf_pkg.h"

using namespace std;
using namespace svrlib;
using namespace Network;

namespace {

};

CPlayerBase::CPlayerBase(PLAYER_TYPE type)
        : m_bPlayerType(type), m_pSession(nullptr), m_bPlayerState(PLAYER_STATE_NULL) {
    m_uid = 0;
    m_loadState.reset();
    m_needRecover = false;
}

CPlayerBase::~CPlayerBase() {
    if (m_pSession != NULL) {
        m_pSession->SetUID(0);
        m_pSession->Close();
        m_pSession = NULL;
    }
}

bool CPlayerBase::IsLoadData(uint8_t dataType) {
    if (dataType < m_loadState.size()) {
        return m_loadState[dataType];
    }
    return false;
}

void CPlayerBase::SetLoadState(uint8_t dataType) {
    if (dataType < m_loadState.size()) {
        m_loadState.set(dataType);
    }
}

bool CPlayerBase::IsLoadOver() {
    return m_loadState.all();
}

void CPlayerBase::SetPlayerState(uint8_t state) {
    m_bPlayerState = state;
}

uint8_t CPlayerBase::GetPlayerState() {
    return m_bPlayerState;
}

bool CPlayerBase::IsPlaying() {
    return m_bPlayerState == PLAYER_STATE_PLAYING;
}

uid_type CPlayerBase::GetUID() {
    return m_uid;
}

void CPlayerBase::SetUID(uid_type uid) {
    m_uid = uid;
}

PLAYER_TYPE CPlayerBase::GetPlayerType() {
    return m_bPlayerType;
}

string CPlayerBase::GetPlayerName() {
    return m_baseInfo.name;
}

uint8_t CPlayerBase::GetSex() {
    return m_baseInfo.sex;
}

void CPlayerBase::SetSession(const TCPConnPtr& conn) {
    m_pSession = conn;
}

TCPConnPtr CPlayerBase::GetSession() {
    return m_pSession;
}

void CPlayerBase::SetIP(uint32_t ip) {
    m_baseInfo.login_ip = ip;
}

uint32_t CPlayerBase::GetIP() {
    return m_baseInfo.login_ip;
}

string CPlayerBase::GetIPStr() {
    return CHelper::ValueToIP(m_baseInfo.login_ip);
}

bool CPlayerBase::SendMsgToClient(const google::protobuf::Message *msg, uint16_t msg_type) {
    if (m_pSession) {
        //LOG_DEBUG("{} 发送消息:{}--len:{}",GetUID(),msg_type,msg->ByteSize());
        return pkg_client::SendProtobufMsg(m_pSession, msg, msg_type);
    }
    return false;
}

bool
CPlayerBase::SendMsgToClient(const void *msg, uint16_t msg_len, uint16_t msg_type) {
    if (m_pSession) {
        //LOG_DEBUG("{} 发送消息:{}--len:{}",GetUID(),msg_type,msg_len);
        return pkg_client::SendBuffMsg(m_pSession, msg, msg_len, msg_type);
    }
    return false;
}

void CPlayerBase::OnLoginOut() {

}

void CPlayerBase::OnLogin() {

}

void CPlayerBase::OnGetAllData() {

}

void CPlayerBase::ReLogin() {

}

void CPlayerBase::OnTimeTick(uint64_t uTime, bool bNewDay) {

}

// 是否需要回收
bool CPlayerBase::NeedRecover() {

    return false;
}

// 设置回收
void CPlayerBase::SetNeedRecover(bool bNeed) {
    m_needRecover = bNeed;
}

// 登陆key
void CPlayerBase::SetLoginKey(const string &key) {
    m_loginKey = key;
}

string CPlayerBase::GetLoginKey() {
    return m_loginKey;
}

void CPlayerBase::GetPlayerBaseData(net::base_info *pInfo) {
    pInfo->set_uid(GetUID());
    pInfo->set_name(m_baseInfo.name);
    pInfo->set_sex(m_baseInfo.sex);
    pInfo->set_coin(m_baseInfo.coin);
    pInfo->set_vip(m_baseInfo.vip);
    pInfo->set_clogin(m_baseInfo.clogin);
    pInfo->set_weeklogin(m_baseInfo.weeklogin);
    pInfo->set_login_ip(m_baseInfo.login_ip);
    pInfo->set_all_login_days(m_baseInfo.all_login_days);
    pInfo->set_offline_time(m_baseInfo.offline_time);
}

void CPlayerBase::SetPlayerBaseData(const net::base_info &info, bool bSetUid) {
    if(bSetUid)
    {
        SetUID(info.uid());
    }
    m_baseInfo.name = info.name();
    m_baseInfo.sex = info.sex();
    m_baseInfo.coin = info.coin();
    m_baseInfo.vip = info.vip();
    m_baseInfo.clogin = info.clogin();
    m_baseInfo.weeklogin = info.weeklogin();
    m_baseInfo.login_ip = info.login_ip();
    m_baseInfo.all_login_days = info.all_login_days();
    m_baseInfo.offline_time = info.offline_time();
}

void CPlayerBase::SetPlayerGameData(const net::svr::msg_enter_into_game_svr& info)
{
    SetPlayerBaseData(info.base_data());
}

void CPlayerBase::GetPlayerGameData(net::svr::msg_enter_into_game_svr* pInfo)
{
    GetPlayerBaseData(pInfo->mutable_base_data());
}

void CPlayerBase::SetLoginLobbySvrID(uint16_t svrID)
{
    m_loginLobbySvrID = svrID;
}

uint16_t CPlayerBase::GetLoginLobbySvrID()
{
    return m_loginLobbySvrID;
}
// 游戏服
void CPlayerBase::SetGameSvrID(uint16_t svrID)
{
    m_curSvrID = svrID;
}
uint16_t CPlayerBase::GetGameSvrID()
{
    return m_curSvrID;
}

































