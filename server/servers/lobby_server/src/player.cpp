
#include "player.h"
#include "helper/bufferStream.h"
#include <time.h>
#include "player_mgr.h"
#include "common_logic.h"
#include "dbagent_client.h"

using namespace svrlib;
using namespace std;
using namespace Network;

namespace
{
static const uint32_t s_OfflineTime = MINUTE*10; // 离线10分钟下线
};

CPlayer::CPlayer(uint8_t type)
		:CPlayerBase(type)
{
	m_disconnectTime = 0;
	m_reloginTime    = 0;
	m_netDelay   = 0;
	m_limitTime.fill(0);

}

CPlayer::~CPlayer()
{

}

void CPlayer::OnLoginOut()
{
	LOG_DEBUG("玩家下线:{}",GetUID());
	if (IsPlaying())
	{
		// 保存数据
		SavePlayerBaseInfo();
	}
	SetPlayerState(PLAYER_STATE_LOGINOUT);
	if (CApplication::Instance().GetStatus() == emSERVER_STATE_REPAIR)
	{
		NotifyLoginOut(0);
	}

	SOL_CALL_LUA(CApplication::Instance().GetSolLuaState()["login_out"](this));
}

void CPlayer::OnLogin()
{
	LOG_DEBUG("OnLogin:{}", GetUID());
	net::cli::msg_login_rep repmsg;
	repmsg.set_result(net::RESULT_CODE_SUCCESS);
	repmsg.set_server_time(getSysTime());
	SendMsgToClient(&repmsg, net::S2C_MSG_LOGIN_REP);
	uint32_t uid = GetUID();


	CDBAgentClientMgr::Instance().LoadPlayerData(uid,emACCDATA_TYPE_BASE);

	SetPlayerState(PLAYER_STATE_LOAD_DATA);

	m_loadTime    = getSysTime();
	m_reloginTime = getSysTime();
}

void CPlayer::OnGetAllData()
{
	LOG_DEBUG("all data loaded over :{}",GetUID());
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
	repmsg.set_server_time(getSysTime());
	SendMsgToClient(&repmsg, net::S2C_MSG_LOGIN_REP);

	SendAllPlayerData2Client();
	NotifyEnterGame();

	m_reloginTime = getSysTime();
}

void CPlayer::OnTimeTick(uint64_t uTime, bool bNewDay)
{
	if (!IsPlaying())
	{
		return;
	}
	if (CCommonLogic::IsNeedReset(m_offlinetime, uTime))
	{
		DailyCleanup(1);
	}
	// 更新离线时间
	m_offlinetime = uTime;
	// 新的一天
	if (bNewDay)
	{
		tm local_time;
		getLocalTime(&local_time, uTime);

		// 跨周0星期天，1星期1
		if (local_time.tm_wday == 0)
			WeeklyCleanup();
		// 跨月
		if (local_time.tm_mday == 1)
			MonthlyCleanup();

		SendAllPlayerData2Client();
	}
	if (m_pSession == NULL)
	{
		if (m_disconnectTime == 0)
		{
			m_disconnectTime = getSysTime();
		}
	}
	else
	{
		m_disconnectTime = 0;
	}
}

// 是否需要回收
bool CPlayer::NeedRecover()
{
	//服务器维护状态
	if (CApplication::Instance().GetStatus() == emSERVER_STATE_REPAIR)
	{
		if (1)
		{
			LOG_ERROR("server is repair status,not playing killed loginout");
			return true;
		}
	}
	if (m_needRecover)
	{
		return true;
	}
	if (GetPlayerState() == PLAYER_STATE_LOAD_DATA && (getSysTime() - m_loadTime) > MINUTE)
	{
		LOG_ERROR("load player data time out :{}", GetUID());
		return true;
	}
	if (GetPlayerState() == PLAYER_STATE_LOAD_DATA)
		return false;

	if (m_pSession == NULL)
	{
		if ((getSysTime() - m_disconnectTime) > s_OfflineTime)// 不在游戏中，或者超时下线
		{
			LOG_DEBUG("not playing,time out loginout {} time {}", GetUID(), s_OfflineTime);
			return true;
		}
	}

	return false;
}

bool CPlayer::CanModifyData()
{
	if (m_bPlayerState >= PLAYER_STATE_PLAYING)
		return true;
	return false;
}

//--- 每日清理
void CPlayer::DailyCleanup(int32_t iOfflineDay)
{
	LOG_DEBUG("daily cleanup:{}",GetUID());
	if (iOfflineDay > 1)
	{
		m_baseInfo.set_clogin(1);
	}
	else
	{
		m_baseInfo.set_clogin(m_baseInfo.clogin()+1);
	}

	m_baseInfo.set_weeklogin(m_baseInfo.weeklogin()+1);

	SOL_CALL_LUA(CApplication::Instance().GetSolLuaState()["new_day"](this));
}

//--- 每周清理
void CPlayer::WeeklyCleanup()
{

	//UnsetRewardBitFlag(net::REWARD_WLOGIN3);
	//UnsetRewardBitFlag(net::REWARD_WLOGIN5);
	//UnsetRewardBitFlag(net::REWARD_WLOGIN6);
	m_baseInfo.set_weeklogin(1);

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
	LOG_DEBUG("send player account data:{}",GetUID());

	net::cli::msg_player_data_rep msg;
	GetPlayerBaseData(msg.mutable_base_data());
	SendMsgToClient(&msg, net::S2C_MSG_PLAYER_INFO);

	return true;
}

// 构建初始化
void CPlayer::BuildInit()
{
	LOG_DEBUG("player build init :{}",GetUID());

	// 检测日常
	uint32_t uBuildTime     = getSysTime();
	uint32_t uOfflineSecond = ((m_offlinetime && uBuildTime > m_offlinetime) ? (uBuildTime - m_offlinetime) : 0);
	//跨天检查并清理
	int32_t  iOfflineDay    = diffTimeDay(m_offlinetime, uBuildTime);
	if (m_offlinetime == 0 && iOfflineDay <= 0)//新上线绝对是第一天
	{
		DailyCleanup(2);
		iOfflineDay = 0;
	}
	//跨周检查并清理
	int32_t iOfflineWeek = diffTimeWeek(m_offlinetime, uBuildTime);
	if (m_offlinetime == 0 && iOfflineWeek <= 0)
	{
		WeeklyCleanup();
		iOfflineWeek = 0;
	}
	//跨月检查并清理
	int32_t iOfflineMonth = diffTimeMonth(m_offlinetime, uBuildTime);
	if (m_offlinetime == 0 && iOfflineMonth <= 0)
	{
		MonthlyCleanup();
		iOfflineMonth = 0;
	}
	if (m_offlinetime != 0 && CCommonLogic::IsNeedReset(m_offlinetime, uBuildTime))
	{
		DailyCleanup(iOfflineDay);
	}
	if (iOfflineWeek > 0)
	{
		WeeklyCleanup();
	}
	if (iOfflineMonth > 0)
	{
		MonthlyCleanup();
	}
	m_offlinetime      = uBuildTime;
	if (m_baseInfo.clogin() < 1)
		m_baseInfo.set_clogin(1);

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
	m_netDelay = (m_netDelay + netDelay)/2;
	if ((getSysTime() - m_limitTime[emLIMIT_TIME_NETDELAY]) > MINUTE)
	{
		m_limitTime[emLIMIT_TIME_NETDELAY] = getSysTime();
		return true;
	}
	return false;
}
// 保存数据
void CPlayer::SavePlayerBaseInfo()
{
    m_baseInfo.set_weeklogin(1);
    m_baseInfo.set_name("test");
    m_baseInfo.set_all_login_days(10);
    m_baseInfo.set_sex(1);
    m_baseInfo.set_uid(GetUID());
    m_baseInfo.set_vip(10);

    DUMP_PROTO_MSG_INFO(m_baseInfo);

	string baseData;
	m_baseInfo.SerializeToString(&baseData);

	LOG_DEBUG("save player data uid:{},datalen:{},{}",GetUID(),baseData.length(),m_offlinetime);

	CDBAgentClientMgr::Instance().SavePlayerData(GetUID(),emACCDATA_TYPE_BASE,baseData);
}








