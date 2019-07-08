
#pragma once

#include "svrlib.h"
#include "player_base.h"
#include "game_define.h"
#include "msg_define.pb.h"
#include "ebus/frame_event.hpp"
#include "ebus/event_bus.hpp"
#include "network_asio/tcp_client.h"
#include "network_asio/tcp_conn.h"
#include "network_asio/byte_buffer.h"

using namespace svrlib;
using namespace std;
using namespace net;
using namespace Network;

class CPlayer : public CPlayerBase, public ebus::EventHandler<ebus::NewDayEvent>
{
	enum LIMIT_TIME
	{
	  emLIMIT_TIME_NETDELAY,
	  emLIMIT_TIME_MAX,
	};
public:
	CPlayer(uint8_t type);

	virtual ~CPlayer();

	virtual void OnLoginOut();

	virtual void OnLogin();

	virtual void OnGetAllData();

	virtual void ReLogin();

	virtual void OnTimeTick(uint64_t uTime, bool bNewDay);

	virtual void onEvent(ebus::NewDayEvent & e) override;

	// 是否需要回收
	virtual bool NeedRecover();
	bool CanModifyData();

	//--- 每日清理
	void DailyCleanup(int32_t iOfflineDay);

	//--- 每周清理
	void WeeklyCleanup();

	//--- 每月清理
	void MonthlyCleanup();

	// 信息同步
	void NotifyEnterGame();

	void NotifyLoginOut(uint32_t code, string deviceid = "");

	bool SendAllPlayerData2Client();

	bool SendAccData2Client();

	bool UpdateAccValue2Client();
	// 构建初始化
	void BuildInit();
public:
	// 获得relogin时间
	uint32_t GetReloginTime();
	// 网络延迟
	uint32_t GetNetDelay();
	bool SetNetDelay(uint32_t netDelay);
protected:
	// 保存数据
	void SavePlayerBaseInfo();

protected:
	uint32_t                               m_disconnectTime;                // 断线时间
	uint32_t                               m_reloginTime;                   // 上次重连时间(限制可能的bug导致频繁重连)
	uint32_t                               m_loadTime;                      // 加载数据时间
	uint32_t                               m_netDelay;                      // 网络延迟
	std::array<uint32_t, emLIMIT_TIME_MAX> m_limitTime;  					// 限制时间
	ebus::HandlerRegistration* 			   m_dayEventReg;
	std::shared_ptr<TCPClient>			   m_tcpClient = nullptr;
};




