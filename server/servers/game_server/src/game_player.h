

#pragma once

#include "svrlib.h"
#include "player_base.h"
#include "game_define.h"
#include "msg_define.pb.h"
#include "game_table.h"

using namespace svrlib;
using namespace std;
using namespace net;
using namespace Network;

class CGameRoom;

class CGameTable;

class CGamePlayer : public CPlayerBase
{
public:
	CGamePlayer(uint8_t type);

	virtual ~CGamePlayer();

	virtual bool SendMsgToClient(const google::protobuf::Message* msg, uint16_t msg_type);

	virtual void OnLoginOut();

	virtual void OnLogin();

	virtual void ReLogin();

	// 通知返回大厅(退出游戏)
	void NotifyLeaveGameSvr();

	// 请求大厅修改数值
	void
	NotifyLobbyChangeAccValue(int32_t operType, int32_t subType, int64_t coin, int64_t safeCoin, const string& chessid = "");
	// 刷新玩家游戏状态到大厅
	void NotifyLobbyChangePlayStatus();

	// 修改玩家账号数值（增量修改）
	virtual void
	SyncChangeAccountValue(uint16_t operType, uint16_t subType, int64_t coin, int64_t safecoin, const string& chessid = "");
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

	// 更新数值到桌子
	void FlushAccValue2Table();

	// 离开当前桌子
	CGameTable* TryLeaveCurTable();
	// 清除俱乐部比赛积分
	void ClearClubScore();

public:
	uint8_t GetNetState();
	void SetNetState(uint8_t state);
	void SetNetStateNoPlayer();
	CGameRoom* GetRoom();
	void SetRoom(CGameRoom* pRoom);
	uint16_t GetRoomID();
	CGameTable* GetTable();
	void SetTable(CGameTable* pTable);
	int64_t GetTableID();
	void SetAutoReady(bool bAuto);
	bool IsAutoReady();
	void SetPlayDisconnect(bool bFlag);
	bool IsPlayDisconnect();
	// 俱乐部积分
	void SetClubScore(int64_t score);
	int64_t GetClubScore();

protected:
	uint8_t m_netState;                   // 网络状态
	CGameRoom * m_pGameRoom;            // 所在房间
	CGameTable* m_pGameTable;           // 所在桌子
	uint32_t m_msgHeartTime;              // 消息心跳时间
	bool   m_autoReady;                 // 自动准备
	bool   m_playDisconnect;            // 游戏是否断线中
	int64_t  m_clubScore;                 // 俱乐部积分

};





