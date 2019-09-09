
#pragma once

#include "svrlib.h"
#include "player_base.h"
#include "game_define.h"
#include "msg_define.pb.h"

using namespace svrlib;
using namespace std;
using namespace net;
using namespace Network;

class CGamePlayer : public CPlayerBase
{
public:
	CGamePlayer(PLAYER_TYPE type);

	virtual ~CGamePlayer();

	virtual bool SendMsgToClient(const google::protobuf::Message* msg, uint16_t msg_type);

	virtual void OnLoginOut();

	virtual void OnLogin();

	virtual void ReLogin();

	// ֪ͨ���ش���(�˳���Ϸ)
	void NotifyLeaveGameSvr();

	// �ܷ��˳�
	bool CanBackLobby();
	// ��������ʱ��
	void ResetHeart();
	// ����ʱ��
	uint32_t GetDisconnectTime();

	virtual void OnTimeTick(uint64_t uTime, bool bNewDay);

	// �Ƿ���Ҫ����
	virtual bool NeedRecover();

	// �Ƿ�������Ϸ��
	bool IsInGamePlaying();
	// ��Ϣ����
	int OnMessage(uint16_t cmdID, const uint8_t* pkt_buf, uint16_t buf_len);
public:
	uint8_t GetNetState();
	void SetNetState(uint8_t state);

	void SetPlayDisconnect(bool bFlag);
	bool IsPlayDisconnect();

protected:
	uint8_t m_netState;                   	// ����״̬
	uint32_t m_msgHeartTime;              	// ��Ϣ����ʱ��
	bool   m_playDisconnect;            	// ��Ϸ�Ƿ������

};





