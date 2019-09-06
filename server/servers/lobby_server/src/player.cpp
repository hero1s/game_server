
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
    static const uint32_t s_OfflineTime = MINUTE*10; // ����10��������
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
        // ��������
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

    // �������ݵ��ͻ���
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
    // ��������ʱ��
    m_baseInfo.offline_time = uTime;
    // �µ�һ��
    if (bNewDay) {
        tm local_time;
        time::localtime(uTime, &local_time);

        // ����0�����죬1����1
        if (local_time.tm_wday==0)
            WeeklyCleanup();
        // ����
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

// �Ƿ���Ҫ����
bool CPlayer::NeedRecover()
{
    //������ά��״̬
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
        if ((time::getSysTime()-m_disconnectTime)>s_OfflineTime)// ������Ϸ�У����߳�ʱ����
        {
            LOG_DEBUG("not playing,time out loginout {} time {}", GetUID(), s_OfflineTime);
            return true;
        }
    }

    return false;
}

// ���ش����ص�
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

//--- ÿ������
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

//--- ÿ������
void CPlayer::WeeklyCleanup()
{

    //UnsetRewardBitFlag(net::REWARD_WLOGIN3);
    //UnsetRewardBitFlag(net::REWARD_WLOGIN5);
    //UnsetRewardBitFlag(net::REWARD_WLOGIN6);
    m_baseInfo.weeklogin = 1;

    SOL_CALL_LUA(CApplication::Instance().GetSolLuaState()["new_week"](this));
}

//--- ÿ������
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

// ֪ͨ���ش���
void CPlayer::NotifyClientBackLobby(uint8_t result, uint8_t reason)
{
    net::cli::msg_back_lobby_rep rep;
    rep.set_result(result);
    rep.set_reason(reason);
    SendMsgToClient(&rep, net::S2C_MSG_BACK_LOBBY_REP);
}

// ������ʼ��
void CPlayer::BuildInit()
{
    LOG_DEBUG("player build init :{}", GetUID());

    // ����ճ�
    uint32_t uBuildTime = time::getSysTime();
    uint32_t offlinetime = m_baseInfo.offline_time;
    uint32_t uOfflineSecond = ((offlinetime && uBuildTime>offlinetime) ? (uBuildTime-offlinetime) : 0);
    //�����鲢����
    int32_t iOfflineDay = time::diffTimeDay(offlinetime, uBuildTime);
    if (offlinetime==0 && iOfflineDay<=0)//�����߾����ǵ�һ��
    {
        DailyCleanup(2);
        iOfflineDay = 0;
    }
    //���ܼ�鲢����
    int32_t iOfflineWeek = time::diffTimeWeek(offlinetime, uBuildTime);
    if (offlinetime==0 && iOfflineWeek<=0) {
        WeeklyCleanup();
        iOfflineWeek = 0;
    }
    //���¼�鲢����
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

// �Ƿ��ڴ�����
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
// ֪ͨ����״̬
void CPlayer::NotifyNetState2GameSvr(uint8_t state)
{
    net::svr::msg_notify_net_state msg;
    msg.set_uid(GetUID());
    msg.set_state(state);
    msg.set_newip(m_baseInfo.login_ip);
    msg.set_no_player(0);
    SendMsgToGameSvr(&msg, net::svr::L2GS_MSG_NOTIFY_NET_STATE);
}
// ���󷵻ش���
void CPlayer::ActionReqBackLobby(uint8_t action)
{
    net::cli::msg_back_lobby_req msg;
    msg.set_uid(GetUID());
    msg.set_is_action(action);
    SendMsgToGameSvr(&msg, net::C2S_MSG_BACK_LOBBY);
}
// ������Ϸ������
uint16_t CPlayer::EnterGameSvr(uint16_t svrID)
{
    auto pServer = CGameServerMgr::Instance().GetServerBySvrID(svrID);
    if (pServer == nullptr || (GetGameSvrID() != svrID))
    {
        BackLobby();
        LOG_DEBUG("������������:{}", svrID);
        return RESULT_CODE_SVR_REPAIR;
    }
    if (GetGameSvrID() != 0 && GetGameSvrID() != svrID)
    {
        LOG_DEBUG("���������ʧ��:uid {}--cur {}-->{}", GetUID(), GetGameSvrID(), svrID);
        ActionReqBackLobby(1);
        return RESULT_CODE_NEED_INLOBBY;
    }

    SetGameSvrID(svrID);

    // ������Ϸ���ݵ���Ϸ��
    net::svr::msg_enter_into_game_svr msg;
    msg.set_player_type(GetPlayerType());
    msg.set_play_type(0);
    GetPlayerGameData(&msg);
    SendMsgToGameSvr(&msg, net::svr::L2GS_MSG_ENTER_INTO_SVR);
    LOG_DEBUG("������Ϸ������:{}-->{}", GetUID(), svrID);

    return RESULT_CODE_SUCCESS;
}


// ���reloginʱ��
uint32_t CPlayer::GetReloginTime()
{
    return m_reloginTime;
}

// �����ӳ�
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

// ��������
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








