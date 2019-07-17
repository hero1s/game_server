//
// Created by toney on 16/4/5.
//

#pragma once

#include "svrlib.h"
#include "game_room.h"
#include "msg_define.pb.h"
#include <unordered_map>

class CGamePlayer;

class CGameRoomMgr : public AutoDeleteSingleton<CGameRoomMgr>
{
public:
	CGameRoomMgr();

	~CGameRoomMgr();

	bool Init();

	void ShutDown();

	CGameRoom* GetRoom(uint32_t roomID);

	CGameRoom* GetPrivateRoom();

	// 强制解散房间
	void ForceClosePlayerTable(int64_t tableID);

	// 发送俱乐部私人房列表
	void SendClubPrivateList(uint32_t uid, int64_t clubID, uint16_t svrid);

	// 发送私人房桌子列表
	void SendAllPrivateTableList2Client(uint32_t uid, uint16_t svrid);

	// 发送房间列表
	void SendRoomList2Client(uint32_t uid);

	// 快速加入房间
	bool FastJoinRoom(CGamePlayer* pPlayer, uint8_t consume);

	// 获得指定消费类型房间
	void GetRoomList(uint8_t consume, vector<CGameRoom*>& rooms);

private:
	using MAP_ROOMS = unordered_map<uint32_t, CGameRoom*>;
	MAP_ROOMS m_mpRooms;

};


