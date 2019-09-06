
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

	// 通知返回大厅(退出游戏)
	void NotifyLeaveGameSvr();

	// 能否退出
	bool CanBackLobby();
	// 重置心跳时间
	void ResetHeart();
	// 断线时间
	uint32_t GetDisconnectTime();

	virtual void OnTimeTick(uint64_t uTime, bool bNewDay);

	// 是否需要回收
	virtual bool NeedRecover();

	// 是否正在游戏中
	bool IsInGamePlaying();
	// 消息处理
	int OnMessage(uint16_t cmdID, const uint8_t* pkt_buf, uint16_t buf_len);
public:
	uint8_t GetNetState();
	void SetNetState(uint8_t state);

	void SetPlayDisconnect(bool bFlag);
	bool IsPlayDisconnect();

protected:
	uint8_t m_netState;                   	// 网络状态
	uint32_t m_msgHeartTime;              	// 消息心跳时间
	bool   m_playDisconnect;            	// 游戏是否断线中

};





