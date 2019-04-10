
#include "player.h"
#include "helper/bufferStream.h"
#include "stdafx.h"
#include <time.h>
#include "player_mgr.h"

using namespace svrlib;
using namespace std;
using namespace Network;

namespace
{
static const uint32_t s_OfflineTime = SECONDS_IN_MIN*10; // ����10��������
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
bool CPlayer::SendMsgToClient(const google::protobuf::Message* msg, uint16_t msg_type, uint8_t compress, uint8_t encrypt)
{
	if (msg->ByteSize() > 500)
	{
		return CPlayerBase::SendMsgToClient(msg, msg_type, 1, encrypt);
	}
	return CPlayerBase::SendMsgToClient(msg, msg_type, compress, encrypt);
}

bool CPlayer::SendMsgToClient(const void* msg, uint16_t msg_len, uint16_t msg_type, uint8_t compress, uint8_t encrypt)
{
	if (msg_len > 500)
	{
		return CPlayerBase::SendMsgToClient(msg, msg_len, msg_type, 1, encrypt);
	}
	return CPlayerBase::SendMsgToClient(msg, msg_len, msg_type, compress, encrypt);
}

void CPlayer::OnLoginOut()
{
	LOG_DEBUG("�������:{}",GetUID());
	if (IsPlaying())
	{
		// ��������
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
	net::msg_login_rep repmsg;
	repmsg.set_result(net::RESULT_CODE_SUCCESS);
	repmsg.set_server_time(getSysTime());
	SendMsgToClient(&repmsg, net::S2C_MSG_LOGIN_REP);
	uint32_t uid = GetUID();

	// test
	string testData;
	if(CPlayerCacheMgr::Instance().GetPlayerCacheData(uid,emACCDATA_TYPE_BASE,testData)){
		LOG_DEBUG("cache get player data success :{}",uid);
		net::base_info baseInfo;//��������
		if(baseInfo.ParseFromString(testData)){
			LOG_DEBUG("cache load base info parase successs :{},datalen:{}",uid,testData.length());
			DUMP_PROTO_MSG_INFO(baseInfo);
		}else{
			LOG_ERROR("cache load base info parase fail:{} {}",uid,testData.length());
		}
	}

	// ��ȡ����
	CDBMysqlMgr::Instance().AsyncLoadPlayerData(GetUID(), [uid](shared_ptr<CDBEventRep>& pRep)
	{
	  LOG_DEBUG("OnLoadPlayerData:{}",uid);
	  if (pRep->vecData.size() > 0)
	  {
		  auto& refRows = pRep->vecData[0];
		  net::base_info baseInfo;//��������
		  string baseData = refRows["Base"].as<string>();
		  if(baseInfo.ParseFromString(baseData)){
			  LOG_DEBUG("load base info success :{},datalen:{}",uid,baseData.length());
              DUMP_PROTO_MSG_INFO(baseInfo);
		  }else{
			  LOG_ERROR("base info parase error :{} {}",uid,baseData.length());
		  }
		  uint32_t offlinetime  = refRows["OfflineTime"];

		  CPlayer* pPlayer = dynamic_cast<CPlayer*>(CPlayerMgr::Instance().GetPlayer(uid));
		  if (pPlayer != NULL && pPlayer->GetPlayerState() == PLAYER_STATE_LOAD_DATA)
		  {
			  pPlayer->SetPlayerBaseData(baseInfo);
			  pPlayer->SetOfflineTime(offlinetime);
			  pPlayer->SetLoadState(emACCDATA_TYPE_BASE);
			  pPlayer->SetLoadState(emACCDATA_TYPE_MISS);//test
			  if (pPlayer->IsLoadOver())
			  {
				  pPlayer->OnGetAllData();
			  }
		  }
		  else
		  {
			  LOG_DEBUG("the player is not find:{}--{}", uid, baseInfo.name());
		  }
	  }
	  else
	  {
		  LOG_ERROR("the base data is can't load:{}", uid);
	  }
	});

	SetPlayerState(PLAYER_STATE_LOAD_DATA);

	m_loadTime    = getSysTime();
	m_reloginTime = getSysTime();
}

void CPlayer::OnGetAllData()
{
	LOG_DEBUG("all data loaded over :{}",GetUID());
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
	net::msg_login_rep repmsg;
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
	// ��������ʱ��
	m_offlinetime = uTime;
	// �µ�һ��
	if (bNewDay)
	{
		tm local_time;
		getLocalTime(&local_time, uTime);

		// ����0�����죬1����1
		if (local_time.tm_wday == 0)
			WeeklyCleanup();
		// ����
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

// �Ƿ���Ҫ����
bool CPlayer::NeedRecover()
{
	//������ά��״̬
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
	if (GetPlayerState() == PLAYER_STATE_LOAD_DATA && (getSysTime() - m_loadTime) > SECONDS_IN_MIN)
	{
		LOG_ERROR("load player data time out :{}", GetUID());
		return true;
	}
	if (GetPlayerState() == PLAYER_STATE_LOAD_DATA)
		return false;

	if (m_pSession == NULL)
	{
		if ((getSysTime() - m_disconnectTime) > s_OfflineTime)// ������Ϸ�У����߳�ʱ����
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

//--- ÿ������
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

//--- ÿ������
void CPlayer::WeeklyCleanup()
{

	//UnsetRewardBitFlag(net::REWARD_WLOGIN3);
	//UnsetRewardBitFlag(net::REWARD_WLOGIN5);
	//UnsetRewardBitFlag(net::REWARD_WLOGIN6);
	m_baseInfo.set_weeklogin(1);

	SOL_CALL_LUA(CApplication::Instance().GetSolLuaState()["new_week"](this));
}

//--- ÿ������
void CPlayer::MonthlyCleanup()
{


	SOL_CALL_LUA(CApplication::Instance().GetSolLuaState()["new_month"](this));
}

void CPlayer::NotifyEnterGame()
{
	net::msg_enter_game_rep msg;
	msg.set_result(0);

	SendMsgToClient(&msg, net::S2C_MSG_ENTER_GAME);
	LOG_DEBUG("notify enter game:{}", GetUID());
}

void CPlayer::NotifyLoginOut(uint32_t code, string deviceid)
{
	LOG_DEBUG("notify leave game :{}--{}", GetUID(), code);
	net::msg_loginout_rep loginoutmsg;
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

	net::msg_player_data_rep msg;
	GetPlayerBaseData(msg.mutable_base_data());
	SendMsgToClient(&msg, net::S2C_MSG_PLAYER_INFO);

	return true;
}

// ������ʼ��
void CPlayer::BuildInit()
{
	LOG_DEBUG("player build init :{}",GetUID());

	// ����ճ�
	uint32_t uBuildTime     = getSysTime();
	uint32_t uOfflineSecond = ((m_offlinetime && uBuildTime > m_offlinetime) ? (uBuildTime - m_offlinetime) : 0);
	//�����鲢����
	int32_t  iOfflineDay    = diffTimeDay(m_offlinetime, uBuildTime);
	if (m_offlinetime == 0 && iOfflineDay <= 0)//�����߾����ǵ�һ��
	{
		DailyCleanup(2);
		iOfflineDay = 0;
	}
	//���ܼ�鲢����
	int32_t iOfflineWeek = diffTimeWeek(m_offlinetime, uBuildTime);
	if (m_offlinetime == 0 && iOfflineWeek <= 0)
	{
		WeeklyCleanup();
		iOfflineWeek = 0;
	}
	//���¼�鲢����
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
	m_netDelay = (m_netDelay + netDelay)/2;
	if ((getSysTime() - m_limitTime[emLIMIT_TIME_NETDELAY]) > SECONDS_IN_MIN)
	{
		m_limitTime[emLIMIT_TIME_NETDELAY] = getSysTime();
		return true;
	}
	return false;
}
// ��������
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
	CDBMysqlMgr::Instance().SavePlayerBaseInfo(GetUID(),baseData,m_offlinetime);
	LOG_DEBUG("save player data uid:{},datalen:{},{}",GetUID(),baseData.length(),m_offlinetime);

	//test
	CPlayerCacheMgr::Instance().SetPlayerCacheData(GetUID(),emACCDATA_TYPE_BASE,baseData);

}








