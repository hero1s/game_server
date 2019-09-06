
#include "player.h"
#include "helper/bufferStream.h"
#include <time.h>
#include "player_mgr.h"
#include "common_logic.h"
#include "net/dbagent_client.h"
#include "net/game_server_mgr.h"

using namespace svrlib;
using namespace std;
using namespace Network;

namespace {
    static const uint32_t s_OfflineTime = MINUTE*10; // 离线10分钟下线
};

CPlayer::CPlayer(PLAYER_TYPE type)
        :CPlayerBase(type)
{
    m_disconnectTime = 0;
    m_reloginTime = 0;
    m_netDelay = 0;
    m_limitTime.fill(0);
    m_dayEventReg = ebus::EventBus::AddHandler<ebus::NewDayEvent>(*this);
}

CPlayer::~CPlayer()
{
    m_dayEventReg->removeHandler();
    SAFE_DELETE(m_dayEventReg);
}

void CPlayer::OnLoginOut()
{
    LOG_DEBUG("player login out:{}", GetUID());
    if (IsPlaying()) {
        // 保存数据
        SavePlayerBaseInfo();
    }
    SetPlayerState(PLAYER_STATE_LOGINOUT);
    if (CApplication::Instance().GetStatus()==emSERVER_STATE_REPAIR) {
        NotifyLoginOut(0);
    }

    SOL_CALL_LUA(CApplication::Instance().GetSolLuaState()["login_out"](this));
}

void CPlayer::OnLogin()
{
    LOG_DEBUG("OnLogin:{}", GetUID());
    net::cli::msg_login_rep repmsg;
    repmsg.set_result(net::RESULT_CODE_SUCCESS);
    repmsg.set_server_time(time::getSysTime());
    SendMsgToClient(&repmsg, net::S2C_MSG_LOGIN_REP);
    uint32_t uid = GetUID();

    CDBAgentClientMgr::Instance().LoadPlayerData(uid, emACCDATA_TYPE_BASE);

    SetPlayerState(PLAYER_STATE_LOAD_DATA);

    m_loadTime = time::getSysTime();
    m_reloginTime = time::getSysTime();

}

void CPlayer::OnGetAllData()
{
    LOG_DEBUG("all data loaded over :{}", GetUID());
    SetPlayerState(PLAYER_STATE_PLAYING);
    BuildInit();

    // 发送数据到客户端
    SendAllPlayerData2Client();
    NotifyEnterGame();

    SOL_CALL_LUA(CApplication::Instance().GetSolLuaState()["login_on"](this));

    SavePlayerBaseInfo();//test
}

void CPlayer::ReLogin()
{
    LOG_DEBUG("player relogin:{}", GetUID());
    net::cli::msg_login_rep repmsg;
    repmsg.set_result(net::RESULT_CODE_SUCCESS);
    repmsg.set_server_time(time::getSysTime());
    SendMsgToClient(&repmsg, net::S2C_MSG_LOGIN_REP);

    SendAllPlayerData2Client();
    NotifyEnterGame();

    m_reloginTime = time::getSysTime();
}

void CPlayer::OnTimeTick(uint64_t uTime, bool bNewDay)
{
    if (!IsPlaying()) {
        return;
    }
    if (CCommonLogic::IsNeedReset(m_baseInfo.offline_time, uTime)) {
        DailyCleanup(1);
    }
    // 更新离线时间
    m_baseInfo.offline_time = uTime;
    // 新的一天
    if (bNewDay) {
        tm local_time;
        time::localtime(uTime, &local_time);

        // 跨周0星期天，1星期1
        if (local_time.tm_wday==0)
            WeeklyCleanup();
        // 跨月
        if (local_time.tm_mday==1)
            MonthlyCleanup();

        SendAllPlayerData2Client();
    }
    if (m_pSession==NULL) {
        if (m_disconnectTime==0) {
            m_disconnectTime = time::getSysTime();
        }
    }
    else {
        m_disconnectTime = 0;
    }
}

void CPlayer::onEvent(ebus::NewDayEvent& e)
{
    LOG_DEBUG("new day event {}", GetUID());
    DailyCleanup(1);
    if (e.isNewWeek()) {
        LOG_DEBUG("new week event {}", GetUID());
    }
    if (e.isNewMonth()) {
        LOG_DEBUG("new month event {}", GetUID());
    }
}

// 是否需要回收
bool CPlayer::NeedRecover()
{
    //服务器维护状态
    if (CApplication::Instance().GetStatus()==emSERVER_STATE_REPAIR) {
        if (1) {
            LOG_ERROR("server is repair status,not playing killed loginout");
            return true;
        }
    }
    if (m_needRecover) {
        return true;
    }
    if (GetPlayerState()==PLAYER_STATE_LOAD_DATA && (time::getSysTime()-m_loadTime)>MINUTE) {
        LOG_ERROR("load player data time out :{}", GetUID());
        return true;
    }
    if (GetPlayerState()==PLAYER_STATE_LOAD_DATA)
        return false;

    if (m_pSession==NULL) {
        if ((time::getSysTime()-m_disconnectTime)>s_OfflineTime)// 不在游戏中，或者超时下线
        {
            LOG_DEBUG("not playing,time out loginout {} time {}", GetUID(), s_OfflineTime);
            return true;
        }
    }

    return false;
}

// 返回大厅回调
void CPlayer::BackLobby()
{
    SetGameSvrID(0);
}

bool CPlayer::CanModifyData()
{
    if (m_bPlayerState>=PLAYER_STATE_PLAYING)
        return true;
    return false;
}

//--- 每日清理
void CPlayer::DailyCleanup(int32_t iOfflineDay)
{
    LOG_DEBUG("daily cleanup:{}", GetUID());
    if (iOfflineDay>1) {
        m_baseInfo.clogin = 1;
    }
    else {
        m_baseInfo.clogin++;
    }

    m_baseInfo.weeklogin++;

    SOL_CALL_LUA(CApplication::Instance().GetSolLuaState()["new_day"](this));
}

//--- 每周清理
void CPlayer::WeeklyCleanup()
{

    //UnsetRewardBitFlag(net::REWARD_WLOGIN3);
    //UnsetRewardBitFlag(net::REWARD_WLOGIN5);
    //UnsetRewardBitFlag(net::REWARD_WLOGIN6);
    m_baseInfo.weeklogin = 1;

    SOL_CALL_LUA(CApplication::Instance().GetSolLuaState()["new_week"](this));
}

//--- 每月清理
void CPlayer::MonthlyCleanup()
{

    SOL_CALL_LUA(CApplication::Instance().GetSolLuaState()["new_month"](this));
}

void CPlayer::NotifyEnterGame()
{
    net::cli::msg_enter_game_rep msg;
    msg.set_result(0);

    SendMsgToClient(&msg, net::S2C_MSG_ENTER_GAME);
    LOG_DEBUG("notify enter game:{}", GetUID());
}

void CPlayer::NotifyLoginOut(uint32_t code, string deviceid)
{
    LOG_DEBUG("notify leave game :{}--{}", GetUID(), code);
    net::cli::msg_loginout_rep loginoutmsg;
    loginoutmsg.set_reason(code);
    loginoutmsg.set_deviceid(deviceid);
    SendMsgToClient(&loginoutmsg, net::S2C_MSG_LOGINOUT_REP);
}

bool CPlayer::SendAllPlayerData2Client()
{
    SendAccData2Client();

    return true;
}

bool CPlayer::SendAccData2Client()
{
    LOG_DEBUG("send player account data:{}", GetUID());

    net::cli::msg_player_data_rep msg;
    GetPlayerBaseData(msg.mutable_base_data());
    SendMsgToClient(&msg, net::S2C_MSG_PLAYER_INFO);

    return true;
}

// 通知返回大厅
void CPlayer::NotifyClientBackLobby(uint8_t result, uint8_t reason)
{
    net::cli::msg_back_lobby_rep rep;
    rep.set_result(result);
    rep.set_reason(reason);
    SendMsgToClient(&rep, net::S2C_MSG_BACK_LOBBY_REP);
}

// 构建初始化
void CPlayer::BuildInit()
{
    LOG_DEBUG("player build init :{}", GetUID());

    // 检测日常
    uint32_t uBuildTime = time::getSysTime();
    uint32_t offlinetime = m_baseInfo.offline_time;
    uint32_t uOfflineSecond = ((offlinetime && uBuildTime>offlinetime) ? (uBuildTime-offlinetime) : 0);
    //跨天检查并清理
    int32_t iOfflineDay = time::diffTimeDay(offlinetime, uBuildTime);
    if (offlinetime==0 && iOfflineDay<=0)//新上线绝对是第一天
    {
        DailyCleanup(2);
        iOfflineDay = 0;
    }
    //跨周检查并清理
    int32_t iOfflineWeek = time::diffTimeWeek(offlinetime, uBuildTime);
    if (offlinetime==0 && iOfflineWeek<=0) {
        WeeklyCleanup();
        iOfflineWeek = 0;
    }
    //跨月检查并清理
    int32_t iOfflineMonth = time::diffTimeMonth(offlinetime, uBuildTime);
    if (offlinetime==0 && iOfflineMonth<=0) {
        MonthlyCleanup();
        iOfflineMonth = 0;
    }
    if (offlinetime!=0 && CCommonLogic::IsNeedReset(offlinetime, uBuildTime)) {
        DailyCleanup(iOfflineDay);
    }
    if (iOfflineWeek>0) {
        WeeklyCleanup();
    }
    if (iOfflineMonth>0) {
        MonthlyCleanup();
    }
    SetOfflineTime(uBuildTime);
    if (m_baseInfo.clogin<1)
        m_baseInfo.clogin = 1;

}

// 是否在大厅中
bool CPlayer::IsInLobby()
{
    if (m_curSvrID != 0)
    {
        return false;
    }
    return true;
}

bool CPlayer::SendMsgToGameSvr(const google::protobuf::Message* msg, uint16_t msg_type)
{
    if (m_curSvrID == 0)
        return false;
    CGameServerMgr::Instance().SendMsg2Server(m_curSvrID, msg, msg_type, GetUID());
    return true;
}

bool CPlayer::SendMsgToGameSvr(const void* msg, uint16_t msg_len, uint16_t msg_type)
{
    if (m_curSvrID == 0)
        return false;
    CGameServerMgr::Instance().SendMsg2Server(m_curSvrID, (uint8_t*) msg, msg_len, msg_type, GetUID());
    return true;
}
// 通知网络状态
void CPlayer::NotifyNetState2GameSvr(uint8_t state)
{
    net::svr::msg_notify_net_state msg;
    msg.set_uid(GetUID());
    msg.set_state(state);
    msg.set_newip(m_baseInfo.login_ip);
    msg.set_no_player(0);
    SendMsgToGameSvr(&msg, net::svr::L2GS_MSG_NOTIFY_NET_STATE);
}
// 请求返回大厅
void CPlayer::ActionReqBackLobby(uint8_t action)
{
    net::cli::msg_back_lobby_req msg;
    msg.set_uid(GetUID());
    msg.set_is_action(action);
    SendMsgToGameSvr(&msg, net::C2S_MSG_BACK_LOBBY);
}
// 进入游戏服务器
uint16_t CPlayer::EnterGameSvr(uint16_t svrID)
{
    auto pServer = CGameServerMgr::Instance().GetServerBySvrID(svrID);
    if (pServer == nullptr || (GetGameSvrID() != svrID))
    {
        BackLobby();
        LOG_DEBUG("服务器不存在:{}", svrID);
        return RESULT_CODE_SVR_REPAIR;
    }
    if (GetGameSvrID() != 0 && GetGameSvrID() != svrID)
    {
        LOG_DEBUG("进入服务器失败:uid {}--cur {}-->{}", GetUID(), GetGameSvrID(), svrID);
        ActionReqBackLobby(1);
        return RESULT_CODE_NEED_INLOBBY;
    }

    SetGameSvrID(svrID);

    // 发送游戏数据到游戏服
    net::svr::msg_enter_into_game_svr msg;
    msg.set_player_type(GetPlayerType());
    msg.set_play_type(0);
    GetPlayerGameData(&msg);
    SendMsgToGameSvr(&msg, net::svr::L2GS_MSG_ENTER_INTO_SVR);
    LOG_DEBUG("进入游戏服务器:{}-->{}", GetUID(), svrID);

    return RESULT_CODE_SUCCESS;
}


// 获得relogin时间
uint32_t CPlayer::GetReloginTime()
{
    return m_reloginTime;
}

// 网络延迟
uint32_t CPlayer::GetNetDelay()
{
    return m_netDelay;
}

bool CPlayer::SetNetDelay(uint32_t netDelay)
{
    m_netDelay = (m_netDelay+netDelay)/2;
    if ((time::getSysTime()-m_limitTime[emLIMIT_TIME_NETDELAY])>MINUTE) {
        m_limitTime[emLIMIT_TIME_NETDELAY] = time::getSysTime();
        return true;
    }
    return false;
}

// 保存数据
void CPlayer::SavePlayerBaseInfo()
{
    m_baseInfo.weeklogin = 1;
    m_baseInfo.name = "test";
    m_baseInfo.all_login_days = 10;
    m_baseInfo.sex = 1;
    m_baseInfo.vip = 11;

    net::base_info baseInfo;
    GetPlayerBaseData(&baseInfo);

    DUMP_PROTO_MSG_INFO(baseInfo);

    string baseData;
    baseInfo.SerializeToString(&baseData);

    LOG_DEBUG("save player data uid:{},datalen:{},{}", GetUID(), baseData.length(), m_baseInfo.offline_time);

    CDBAgentClientMgr::Instance().SavePlayerData(GetUID(), emACCDATA_TYPE_BASE, baseData);
}








