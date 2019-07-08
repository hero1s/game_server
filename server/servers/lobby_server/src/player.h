
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

	// �Ƿ���Ҫ����
	virtual bool NeedRecover();
	bool CanModifyData();

	//--- ÿ������
	void DailyCleanup(int32_t iOfflineDay);

	//--- ÿ������
	void WeeklyCleanup();

	//--- ÿ������
	void MonthlyCleanup();

	// ��Ϣͬ��
	void NotifyEnterGame();

	void NotifyLoginOut(uint32_t code, string deviceid = "");

	bool SendAllPlayerData2Client();

	bool SendAccData2Client();

	bool UpdateAccValue2Client();
	// ������ʼ��
	void BuildInit();
public:
	// ���reloginʱ��
	uint32_t GetReloginTime();
	// �����ӳ�
	uint32_t GetNetDelay();
	bool SetNetDelay(uint32_t netDelay);
protected:
	// ��������
	void SavePlayerBaseInfo();

protected:
	uint32_t                               m_disconnectTime;                // ����ʱ��
	uint32_t                               m_reloginTime;                   // �ϴ�����ʱ��(���ƿ��ܵ�bug����Ƶ������)
	uint32_t                               m_loadTime;                      // ��������ʱ��
	uint32_t                               m_netDelay;                      // �����ӳ�
	std::array<uint32_t, emLIMIT_TIME_MAX> m_limitTime;  					// ����ʱ��
	ebus::HandlerRegistration* 			   m_dayEventReg;
	std::shared_ptr<TCPClient>			   m_tcpClient = nullptr;
};




