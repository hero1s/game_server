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

	// ǿ�ƽ�ɢ����
	void ForceClosePlayerTable(int64_t tableID);

	// ���;��ֲ�˽�˷��б�
	void SendClubPrivateList(uint32_t uid, int64_t clubID, uint16_t svrid);

	// ����˽�˷������б�
	void SendAllPrivateTableList2Client(uint32_t uid, uint16_t svrid);

	// ���ͷ����б�
	void SendRoomList2Client(uint32_t uid);

	// ���ټ��뷿��
	bool FastJoinRoom(CGamePlayer* pPlayer, uint8_t consume);

	// ���ָ���������ͷ���
	void GetRoomList(uint8_t consume, vector<CGameRoom*>& rooms);

private:
	using MAP_ROOMS = unordered_map<uint32_t, CGameRoom*>;
	MAP_ROOMS m_mpRooms;

};


