//
// Created by toney on 16/4/5.
//
#include "game_room_mgr.h"
#include "stdafx.h"
#include "data_cfg_mgr.h"

using namespace std;
using namespace svrlib;

CGameRoomMgr::CGameRoomMgr()
{

}

CGameRoomMgr::~CGameRoomMgr()
{

}

bool CGameRoomMgr::Init()
{
	// 初始化房间信息
	uint16_t gameType    = CDataCfgMgr::Instance().GetCurSvrCfg().gameType;
	uint8_t  gameSubType = CDataCfgMgr::Instance().GetCurSvrCfg().gameSubType;

	if (gameSubType != GAME_SUB_PRIVATE)
	{// 普通场配置房间
		vector<stRoomCfg> vecRooms;

		bool bRet = CDBMysqlMgr::Instance().GetSyncDBOper(DB_INDEX_TYPE_CFG).LoadRoomCfg(gameType, gameSubType, vecRooms);
		if (!bRet)return false;
		for (auto& cfg : vecRooms)
		{
			CGameRoom* pRoom = new CGameRoom();
			pRoom->SetRoomCfg(cfg);
			if (pRoom->Init(gameType) == false)
			{
				LOG_ERROR("room config error :{}", cfg.roomID);
				return false;
			}
			m_mpRooms.insert(make_pair(pRoom->GetRoomID(), pRoom));
		}
	}
	else
	{// 私人房直接开启一个房间
		stRoomCfg roomCfg;
		roomCfg.roomID    = 1;
		roomCfg.consume   = ROOM_CONSUME_TYPE_SCORE;
		roomCfg.enter_min = 0;
		roomCfg.enter_max = 1000000000;
		roomCfg.baseScore = 1;
		roomCfg.roomType  = emROOM_TYPE_PRIVATE;

		CGameRoom* pRoom = new CGameRoom();
		pRoom->SetRoomCfg(roomCfg);
		if (pRoom->Init(gameType) == false)
		{
			LOG_ERROR("room config error :{}", roomCfg.roomID);
			return false;
		}
		m_mpRooms.insert(make_pair(pRoom->GetRoomID(), pRoom));
	}

	CDBMysqlMgr::Instance().ClearPlayerOnlineInfo(CApplication::Instance().GetServerID());
	return true;
}

void CGameRoomMgr::ShutDown()
{
	for (auto& it : m_mpRooms)
	{
		CGameRoom* pRoom = it.second;
		pRoom->ShutDown();
		SAFE_DELETE(pRoom);
	}
	m_mpRooms.clear();
}

CGameRoom* CGameRoomMgr::GetRoom(uint32_t roomID)
{
	CGameRoom* pRoom = NULL;
	auto it = m_mpRooms.find(roomID);
	if (it != m_mpRooms.end())
	{
		pRoom = it->second;
	}
	return pRoom;
}

CGameRoom* CGameRoomMgr::GetPrivateRoom()
{
	CGameRoom* pRoom = NULL;
	for (auto& it : m_mpRooms)
	{
		CGameRoom* pGameRoom = it.second;
		if (pGameRoom->GetRoomType() == emROOM_TYPE_PRIVATE)
		{
			pRoom = pGameRoom;
			break;
		}
	}
	return pRoom;
}

void CGameRoomMgr::ForceClosePlayerTable(int64_t tableID)
{
	CGameRoom* pGameRoom = GetPrivateRoom();
	if (pGameRoom == NULL)return;
	CGameTable* pTable = pGameRoom->GetTable(tableID);
	if (pTable != NULL)
	{
		pTable->ForceGameOver(false);
	}
}

void CGameRoomMgr::SendClubPrivateList(uint32_t uid, int64_t clubID, uint16_t svrid)
{
	net::msg_table_list_rep repmsg;
	repmsg.set_game_type(CDataCfgMgr::Instance().GetCurSvrCfg().gameType);
	repmsg.set_is_alter(0);
	repmsg.set_club_id(clubID);

	CGameRoom* pGameRoom = GetPrivateRoom();
	if (pGameRoom != NULL)
	{
		vector<CGameTable*> vecTables;
		pGameRoom->GetClubTables(clubID, vecTables);
		for (auto pTable : vecTables)
		{
			if (pTable != NULL /*&& !pTable->WantNeedRecover()*/)
			{
				net::table_face_info* pInfo = repmsg.add_tables();
				pTable->GetTableFaceInfo(pInfo);
			}
		}
	}
	//LOG_DEBUG("发送俱乐部桌子列表到俱乐部:{}", repmsg.tables_size());
	CLobbyMgr::Instance().SendMsg2Client(&repmsg, net::S2C_MSG_TABLE_LIST, uid, svrid);
}

void CGameRoomMgr::SendAllPrivateTableList2Client(uint32_t uid, uint16_t svrid)
{
	uint8_t                   tableCount = 0;
	net::msg_table_list_rep msg;
	msg.set_game_type(CDataCfgMgr::Instance().GetCurSvrCfg().gameType);
	msg.set_is_alter(0);
	msg.set_club_id(0);

	CGameRoom* pGameRoom = GetPrivateRoom();
	if (pGameRoom != NULL)
	{
		vector<CGameTable*> vecTbls;
		vector<CGameTable*> vecTbls1;
		pGameRoom->GetSelfJoinTables(uid, vecTbls);
		pGameRoom->GetTableHostID(uid, vecTbls1);

		std::set<CGameTable*> setTbls;

		for (auto it:vecTbls)
		{
			setTbls.insert(it);
		}
		for (auto it:vecTbls1)
		{
			setTbls.insert(it);
		}
		for (auto pTable : setTbls)
		{
			if (pTable != NULL && !pTable->WantNeedRecover())
			{
				net::table_face_info* pInfo = msg.add_tables();
				pTable->GetTableFaceInfo(pInfo);
			}
		}
	}

	LOG_DEBUG("发送桌子列表到大厅:{}--size:{}", uid, msg.tables_size());
	CLobbyMgr::Instance().SendMsg2Client(&msg, net::S2C_MSG_TABLE_LIST, uid, svrid);
}

void CGameRoomMgr::SendRoomList2Client(uint32_t uid)
{
	CGamePlayer* pGamePlayer = (CGamePlayer*) CPlayerMgr::Instance().GetPlayer(uid);
	if (pGamePlayer == NULL)
		return;
	net::msg_rooms_info_rep roominfo;
	roominfo.set_cur_roomid(pGamePlayer->GetRoomID());
	for (auto& it : m_mpRooms)
	{
		net::room_info* pRoom     = roominfo.add_rooms();
		CGameRoom     * pGameRoom = it.second;
		pGameRoom->GetRoomInfo(pRoom);
	}
	pGamePlayer->SendMsgToClient(&roominfo, net::S2C_MSG_ROOMS_INFO);
	LOG_DEBUG("Send Room List:{}", roominfo.rooms_size());

}

bool CGameRoomMgr::FastJoinRoom(CGamePlayer* pPlayer, uint8_t consume)
{
	CGameRoom* pOldRoom = pPlayer->GetRoom();
	if (pOldRoom != NULL)
	{
		if (!pOldRoom->CanLeaveRoom(pPlayer))
		{
			return false;
		}
		if (!pOldRoom->LeaveRoom(pPlayer))
		{
			return false;
		}
	}
	vector<CGameRoom*> rooms;
	GetRoomList(consume, rooms);
	for (auto pRoom : rooms)
	{
		if (pRoom->GetPlayerNum() > 0 && pRoom->CanEnterRoom(pPlayer))
		{
			pRoom->EnterRoom(pPlayer);
			pRoom->FastJoinTable(pPlayer);
			return true;
		}
	}
	for (auto pRoom : rooms)
	{
		if (pRoom->CanEnterRoom(pPlayer))
		{
			pRoom->EnterRoom(pPlayer);
			pRoom->FastJoinTable(pPlayer);
			return true;
		}
	}

	return false;
}

void CGameRoomMgr::GetRoomList(uint8_t consume, vector<CGameRoom*>& rooms)
{
	for (auto& it : m_mpRooms)
	{
		CGameRoom* pGameRoom = it.second;
		//if(pGameRoom->GetDeal() == deal && pGameRoom->GetConsume() == consume){
		rooms.push_back(pGameRoom);
		//}
	}
	std::sort(rooms.begin(), rooms.end(), [](auto pRoom1, auto pRoom2)
	{
	  return pRoom1->GetEnterMin() > pRoom2->GetEnterMin();
	});
}













