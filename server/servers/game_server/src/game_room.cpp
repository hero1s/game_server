
#include <data_cfg_mgr.h>
#include "game_room.h"
#include "stdafx.h"
#include "game_table.h"
#include "game_server_config.h"
#include "game_table_private.h"
#include "game_table_coin.h"
#include "game_room_mgr.h"

using namespace svrlib;
using namespace std;
using namespace net;

namespace
{
const static uint8_t s_MaxSendTable = 30;
}

CGameRoom::CGameRoom()
		:m_timer(this)
{
	m_playerNum  = 0;        // 玩家人数
	m_showonline = 0;
	m_roomIndex  = 0;        // 分配房间索引
	m_gameType   = 0;
}

CGameRoom::~CGameRoom()
{
}

void CGameRoom::OnTimer()
{
	OnTimeTick();
	CApplication::Instance().schedule(&m_timer, 1000);
}

bool CGameRoom::Init(uint16_t gameType)
{
	LOG_DEBUG("init game room：{}-{}-{}", m_roomCfg.roomID, m_roomCfg.consume, m_roomCfg.baseScore);
	m_gameType = gameType;
	if (GetRoomType() == emROOM_TYPE_PRIVATE)// 私人房加载私人房数据
	{
		CDBMysqlMgr::Instance().AsyncLoadPrivateTableByGameType(m_gameType, [](shared_ptr<CDBEventRep>& pRep)
		{
		  LOG_DEBUG("OnLoadPrivateTable Data {}", pRep->vecData.size());
		  CGameRoom* pPrivateRoom = CGameRoomMgr::Instance().GetPrivateRoom();
		  if (pPrivateRoom == NULL)return;

		  if (pRep->vecData.size() > 0)
		  {
			  for (uint32_t i = 0; i < pRep->vecData.size(); ++i)
			  {
				  stPrivateTable table;
				  auto& refRows = pRep->vecData[i];

				  table.tableID   = refRows["tableid"];
				  table.hostID    = refRows["hostid"];
				  table.passwd    = refRows["passwd"].as<string>();
				  table.dueround  = refRows["dueround"];
				  table.dueTime   = refRows["duetime"];
				  table.startTime = refRows["starttime"];
				  table.baseScore = refRows["basescore"];
				  table.seatNum   = refRows["seatnum"];
				  table.gameType  = refRows["gametype"];
				  table.playType  = refRows["playtype"];

				  table.createType = refRows["createtype"];
				  table.createTime = refRows["createtime"];
				  table.gameInfo   = refRows["gameinfo"].as<string>();
				  table.addParam   = refRows["addparam"].as<string>();
				  table.matchParam = refRows["matchparam"].as<string>();
				  table.consume    = refRows["consume"];
				  table.enterMin   = refRows["entermin"];
				  table.rentType   = refRows["rent"];
				  table.fee        = refRows["fee"];
				  table.clubID     = refRows["clubid"];
				  table.clubFloor  = refRows["clubfloor"];
				  table.clubPos    = refRows["clubpos"];
				  table.idxRound   = refRows["idxround"];

				  pPrivateRoom->LoadPrivateTable(table);
			  }
		  }
		});
	}
	if (GetRoomType() == emROOM_TYPE_COMMON)// 普通房初始化桌子
	{
		for (uint32_t i = 0; i < m_roomCfg.tableNum; ++i)
		{
			MallocTable();
		}
	}
	CApplication::Instance().schedule(&m_timer, 1000);

	return true;
}

void CGameRoom::ShutDown()
{
	for (auto& it : m_mpTables)
	{
		CGameTable* pTable = it.second;
		pTable->ShutDown();
	}

	m_timer.cancel();
}

void CGameRoom::OnTimeTick()
{
	if (m_coolMarry.isTimeOut())
	{
		MarryTable();
		m_coolMarry.beginCooling(1000);
	}
	if (m_coolRecover.isTimeOut())
	{
		CheckRecover();
		m_coolRecover.beginCooling(2000);
	}
	if (m_coolNewTable.isTimeOut())
	{
		CheckNewTable();
		m_coolNewTable.beginCooling(5000);
		CalcShowOnline();
	}
}

bool CGameRoom::EnterRoom(CGamePlayer* pGamePlayer)
{
	if (pGamePlayer->GetRoom() != this && !CanEnterRoom(pGamePlayer))
	{
		LOG_DEBUG("不能加入房间");
		return false;
	}
	if (pGamePlayer->GetRoom() != this)
	{
		pGamePlayer->SetRoom(this);
		pGamePlayer->SetAutoReady(false);
		m_playerNum++;
	}
	auto pTable = pGamePlayer->GetTable();

	net::msg_enter_room_rep rep;
	rep.set_result(1);
	rep.set_cur_table(0);
	rep.set_passwd("");
	if (pTable != NULL)
	{// 是否还需要进入
		if (pTable->WantNeedRecover())
		{
			pTable = pGamePlayer->TryLeaveCurTable();
		}
	}
	if (pTable != NULL)
	{
		rep.set_cur_table(pTable->GetTableID());
		rep.set_passwd(pTable->GetPasswd());
	}

	GetRoomInfo(rep.mutable_room());
	pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_ENTER_ROOM_REP);

	CDBMysqlMgr::Instance().UpdatePlayerOnlineInfo(pGamePlayer->GetUID(), CApplication::Instance().GetServerID(),
	                                               GetRoomID(), pGamePlayer->GetPlayerType());
	CalcShowOnline();
	LOG_DEBUG("EnterRoom uid:{},roomid:{},curtable:{}-{}", pGamePlayer->GetUID(), GetRoomID(), rep.cur_table(), rep.passwd());

	pGamePlayer->NotifyLobbyChangePlayStatus();
	return true;
}

bool CGameRoom::LeaveRoom(CGamePlayer* pGamePlayer)
{
	if (pGamePlayer->GetRoom() != this)
	{
		LOG_ERROR("not the same room:{}", pGamePlayer->GetUID());
		return false;
	}

	CGameTable* pTable = pGamePlayer->GetTable();
	if (pTable != NULL)
	{
		if (!pTable->CanLeaveTable(pGamePlayer))
			return false;
		pTable->LeaveTable(pGamePlayer);
	}
	LeaveMarry(pGamePlayer);
	pGamePlayer->SetRoom(NULL);
	pGamePlayer->SetAutoReady(false);
	m_playerNum--;

	CDBMysqlMgr::Instance().UpdatePlayerOnlineInfo(pGamePlayer->GetUID(), CApplication::Instance().GetServerID(), 0,
	                                               pGamePlayer->GetPlayerType());
	CalcShowOnline();
	LOG_DEBUG("LeaveRoom uid:{},roomid:{}", pGamePlayer->GetUID(), GetRoomID());

	pGamePlayer->NotifyLobbyChangePlayStatus();
	return true;
}

uint8_t CGameRoom::EnterTable(CGamePlayer* pGamePlayer, int64_t tableID, string passwd)
{
	if (pGamePlayer->GetRoom() != this)
	{
		LOG_ERROR("not the same room:{}", pGamePlayer->GetUID());
		return net::RESULT_CODE_FAIL;
	}
	//获取新旧桌子
	CGameTable* pOldTable = pGamePlayer->GetTable();
	CGameTable* pNewTable = GetTable(tableID);
	if (pNewTable == NULL)
	{
		pNewTable = GetTableByPasswd(passwd);
	}
	//新桌子不存在直接返回
	if (pNewTable == NULL)
	{
		LOG_DEBUG("the table is not have:{}--{}", tableID, passwd);
		return net::RESULT_CODE_NOT_TABLE;
	}
	if (pNewTable->GetClubID() != pGamePlayer->GetCurClubID())
	{
		LOG_DEBUG("桌子俱乐部ID不匹配{}：tbl:{}--self:{},passwd:{}", pGamePlayer->GetUID(), pNewTable->GetClubID(), pGamePlayer->GetCurClubID(), passwd);
		return net::RESULT_CODE_ERROR_STATE;
	}

	//私人房
	if (GetRoomType() == emROOM_TYPE_PRIVATE)
	{
		// 特殊处理
		net::msg_enter_table_rep rep;
		rep.set_result(RESULT_CODE_SUCCESS);
		rep.set_table_id(tableID);
		if (pOldTable != NULL)
		{
			if (pOldTable == pNewTable)
			{
				LOG_DEBUG("backe table");
				pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_ENTER_TABLE);
				pOldTable->OnActionUserNetState(pGamePlayer, true);
				return net::RESULT_CODE_SUCCESS;
			}
			else
			{
				pOldTable = pGamePlayer->TryLeaveCurTable();
				if (pOldTable != NULL)
				{
					return net::RESULT_CODE_FAIL;
				}
			}
			ASSERT_LOG(pOldTable == NULL, "旧桌子不为NULL");
		}
		if (((CGamePrivateTable*) pNewTable)->IsScoreMode() && pGamePlayer->GetClubScore() <= 0)
		{//积分场加载积分
			int64_t clubScore = CDBMysqlMgr::Instance().GetSyncDBOper(DB_INDEX_TYPE_ACC).LoadClubScore(
					pGamePlayer->GetUID(), pGamePlayer->GetCurClubID());
			pGamePlayer->SetClubScore(clubScore);
		}

		if (!((CGamePrivateTable*) pNewTable)->IsExistPrivatePlayerInfo(pGamePlayer->GetUID())
				&& pNewTable->IsRightPasswd(passwd) == false)
		{
			LOG_DEBUG("passwd is error:{}", passwd);
			pGamePlayer->ClearClubScore();
			return net::RESULT_CODE_PASSWD_ERROR;
		}
		int32_t ret = pNewTable->CanEnterTable(pGamePlayer);
		if (ret != net::RESULT_CODE_SUCCESS)
		{
			LOG_DEBUG("cant join table {}", ret);
			pGamePlayer->ClearClubScore();
			return ret;
		}
		pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_ENTER_TABLE);
		if (pNewTable->EnterTable(pGamePlayer))
		{
			return net::RESULT_CODE_SUCCESS;
		}
		pGamePlayer->ClearClubScore();
		return net::RESULT_CODE_FAIL;
	}
	else
	{// 普通房
		if (pOldTable != NULL)
		{
			if (pOldTable->GetTableID() == tableID)
			{
				LOG_DEBUG("back table");
				pOldTable->OnActionUserNetState(pGamePlayer, true);
				return net::RESULT_CODE_SUCCESS;
			}
			if (!pOldTable->CanLeaveTable(pGamePlayer))
			{
				LOG_DEBUG("can't leave table:{}", pGamePlayer->GetUID());
				return net::RESULT_CODE_FAIL;
			}
			if (!pOldTable->LeaveTable(pGamePlayer))
			{
				LOG_DEBUG("leave table faild:{}", pGamePlayer->GetUID());
				return net::RESULT_CODE_FAIL;
			}
		}
		if (IsNeedMarry())
		{
			JoinMarry(pGamePlayer, 0);
		}
		else
		{
			int32_t ret = pNewTable->CanEnterTable(pGamePlayer);
			if (ret != net::RESULT_CODE_SUCCESS)
			{
				if (FastJoinTable(pGamePlayer))
				{
					return net::RESULT_CODE_SUCCESS;
				}
				else
				{
					LOG_DEBUG("can't join the table");
					return ret;
				}
			}
			if (pNewTable->EnterTable(pGamePlayer))
			{
				return net::RESULT_CODE_SUCCESS;
			}
			return net::RESULT_CODE_FAIL;
		}
		return net::RESULT_CODE_SUCCESS;
	}
	LOG_DEBUG("join table faild:{}-->{}", pGamePlayer->GetUID(), tableID);
	return net::RESULT_CODE_FAIL;
}

bool CGameRoom::FastJoinTable(CGamePlayer* pGamePlayer)
{
	if (pGamePlayer->GetRoom() != this)
	{
		LOG_ERROR("not the same room:{}", pGamePlayer->GetUID());
		return false;
	}
	if (GetRoomType() != emROOM_TYPE_COMMON)
		return false;
	CGameTable* pOldTable = pGamePlayer->GetTable();
	if (pOldTable == NULL)
	{
		if (IsNeedMarry())
		{
			JoinMarry(pGamePlayer, 0);
			return true;
		}
		else
		{
			return JoinNoFullTable(pGamePlayer, 0);
		}
	}
	else
	{
		if (pOldTable->CanLeaveTable(pGamePlayer) && pOldTable->LeaveTable(pGamePlayer))
		{
			if (IsNeedMarry())
			{
				JoinMarry(pGamePlayer, pOldTable->GetTableID());
				return true;
			}
			else
			{
				return JoinNoFullTable(pGamePlayer, pOldTable->GetTableID());
			}
		}
	}
	return false;
}

void CGameRoom::SetRoomCfg(stRoomCfg& cfg)
{
	m_roomCfg = cfg;
}

uint16_t CGameRoom::GetRoomID()
{
	return m_roomCfg.roomID;
}

uint8_t CGameRoom::GetConsume()
{
	return m_roomCfg.consume;
}

int64_t CGameRoom::GetEnterMin()
{
	return m_roomCfg.enter_min;
}

int64_t CGameRoom::GetEnterMax()
{
	return m_roomCfg.enter_max;
}

int32_t CGameRoom::GetBaseScore()
{
	return m_roomCfg.baseScore;
}

uint8_t CGameRoom::GetRoomType()
{
	return m_roomCfg.roomType;
}

int64_t CGameRoom::GetSitDown()
{
	return m_roomCfg.sitdown;
}

bool CGameRoom::IsNeedMarry()
{
	return m_roomCfg.marry == 1;
}

int32_t CGameRoom::GetPlayerNum()
{
	return m_playerNum;
}

uint16_t CGameRoom::GetGameType()
{
	return m_gameType;
}

bool CGameRoom::CanEnterRoom(CGamePlayer* pGamePlayer)
{
	if (pGamePlayer->GetRoom() != NULL && pGamePlayer->GetRoom() != this)
	{
		LOG_DEBUG("haved join the other room");
		return false;
	}
	if (GetRoomType() == emROOM_TYPE_COMMON)
	{
		if (m_roomCfg.limitEnter != 0)
		{
			if (pGamePlayer->GetAccountValue(emACC_VALUE_COIN) < GetEnterMin() ||
					pGamePlayer->GetAccountValue(emACC_VALUE_COIN) > GetEnterMax())
			{
				LOG_DEBUG("less score--room:{}--min:{}--max:{}", GetRoomID(), GetEnterMin(), GetEnterMax());
				return false;
			}
		}
	}
	return true;
}

bool CGameRoom::CanLeaveRoom(CGamePlayer* pGamePlayer)
{
	CGameTable* pTable = pGamePlayer->GetTable();
	if (pTable != NULL && !pTable->CanLeaveTable(pGamePlayer))
		return false;

	return true;
}

void CGameRoom::GetRoomInfo(net::room_info* pRoom)
{
	pRoom->set_id(GetRoomID());
	pRoom->set_consume(m_roomCfg.consume);
	pRoom->set_enter_min(m_roomCfg.enter_min);
	pRoom->set_enter_max(m_roomCfg.enter_max);
	pRoom->set_player_num(m_showonline);
	pRoom->set_basescore(m_roomCfg.baseScore);
	pRoom->set_show_type(m_roomCfg.showType);
	pRoom->set_show_pic(m_roomCfg.showPic);

}

//发送金币场桌子信息给玩家
void CGameRoom::SendTableListToPlayer(CGamePlayer* pGamePlayer, int64_t tableID)
{
	LOG_DEBUG("SendTableList to player:{}", tableID);
	if (GetRoomType() != emROOM_TYPE_PRIVATE)
	{
		LOG_DEBUG("not private don't send tablelist");
		return;
	}
	uint8_t                   tableCount = 0;
	net::msg_table_list_rep msg;
	msg.set_game_type(GetGameType());
	msg.set_club_id(0);
	msg.set_is_alter(0);

	if (tableID != 0)
	{
		CGameTable* pTable = GetTable(tableID);
		if (pTable != NULL)
		{
			net::table_face_info* pInfo = msg.add_tables();
			pTable->GetTableFaceInfo(pInfo);
		}
		pGamePlayer->SendMsgToClient(&msg, net::S2C_MSG_TABLE_LIST);
		return;
	}
	vector<CGameTable*> tables;
	for (auto& it : m_mpTables)
	{
		CGameTable* pTable = it.second;
		// 过滤俱乐部房间，
		if (pTable->GetClubID() != 0 || pTable->WantNeedRecover())
			continue;

		tables.push_back(pTable);
	}
	// 排序
	std::sort(tables.begin(), tables.end(), [](auto pTable1, auto pTable2)
	{
	  if (pTable1->GetOnlinePlayerNum() == pTable2->GetOnlinePlayerNum())
	  {
		  if (pTable1->GetPlayerNum() == pTable2->GetPlayerNum())
		  {
			  return pTable1->GetTableID() < pTable2->GetTableID();
		  }
		  return pTable1->GetPlayerNum() > pTable2->GetPlayerNum();
	  }
	  return pTable1->GetOnlinePlayerNum() > pTable2->GetOnlinePlayerNum();
	});

	for (uint32_t i = 0; i < tables.size() && i < s_MaxSendTable; ++i)
	{
		CGameTable* pTable = tables[i];
		if (pTable != NULL && !pTable->WantNeedRecover())
		{
			net::table_face_info* pInfo = msg.add_tables();
			pTable->GetTableFaceInfo(pInfo);
		}
	}
	LOG_DEBUG("send table list:{}", msg.tables_size());
	pGamePlayer->SendMsgToClient(&msg, net::S2C_MSG_TABLE_LIST);
}

//查询桌子列表信息
void CGameRoom::QueryTableListToPlayer(CGamePlayer* pGamePlayer, uint32_t start, uint32_t end)
{
	uint8_t                         tableCount = 0;
	net::msg_query_table_list_rep msg;
	vector<CGameTable*>           tables;
	for (auto& it : m_mpTables)
	{
		CGameTable* pTable = it.second;
		tables.push_back(pTable);
	}
	// 排序
	msg.set_table_num(tables.size());
	std::sort(tables.begin(), tables.end(), [](auto pTable1, auto pTable2)
	{
	  return pTable1->GetTableID() < pTable2->GetTableID();
	});

	for (uint32_t i = start; i < tables.size() && i <= end && tableCount < s_MaxSendTable; ++i)
	{
		CGameTable          * pTable = tables[i];
		net::table_face_info* pInfo  = msg.add_tables();
		pTable->GetTableFaceInfo(pInfo);
		tableCount++;
	}
	pGamePlayer->SendMsgToClient(&msg, net::S2C_MSG_QUERY_TABLE_LIST_REP);
}

// 拷贝房间配置信息
void CGameRoom::CopyRoomCfgToTableCfg(stTableConf& conf)
{
	conf.baseScore = GetBaseScore();
	conf.consume   = GetConsume();
	conf.enterMin  = GetEnterMin();
	conf.enterMax  = GetEnterMax();
	conf.feeType   = m_roomCfg.feeType;
	conf.feeValue  = m_roomCfg.feeValue;
	conf.seatNum   = m_roomCfg.seatNum;
}

CGameTable* CGameRoom::MallocTable()
{
	CGameTable* pTable = NULL;
	if (!m_freeTable.empty())
	{
		pTable = m_freeTable.front();
		m_freeTable.pop();
	}
	else
	{
		int64_t tableID = ++m_roomIndex;
		pTable = CreateCoinTable(tableID);
		stTableConf conf;
		CopyRoomCfgToTableCfg(conf);
		pTable->SetTableConf(conf);
		pTable->Init();
	}
	pTable->ResetTable();
	m_mpTables.insert(make_pair(pTable->GetTableID(), pTable));
	LOG_DEBUG("room:{} malloctable:{}", GetRoomID(), pTable->GetTableID());
	return pTable;
}

void CGameRoom::FreeTable(CGameTable* pTable)
{
	LOG_DEBUG("room:{} freetable:{}", GetRoomID(), pTable->GetTableID());
	m_mpTables.erase(pTable->GetTableID());
	if (GetRoomType() != emROOM_TYPE_PRIVATE)
	{
		m_freeTable.push(pTable);
	}
	else
	{
		LOG_DEBUG("释放桌子:{}", pTable->GetTableID());
		pTable->ShutDown();
		SAFE_DELETE(pTable);
	}
}

CGameTable* CGameRoom::GetTable(int64_t tableID)
{
	auto it = m_mpTables.find(tableID);
	if (it != m_mpTables.end())
	{
		return it->second;
	}
	return NULL;
}

CGameTable* CGameRoom::GetTableByPasswd(string passwd)
{
	for (auto& it : m_mpTables)
	{
		CGameTable* pTable = it.second;
		if (pTable->IsRightPasswd(passwd))
			return pTable;
	}
	return NULL;
}

void CGameRoom::GetTableHostID(uint32_t hostID, vector<CGameTable*>& vecTables)
{
	vecTables.clear();
	for (auto& it : m_mpTables)
	{
		CGameTable* pTable = it.second;
		if (!pTable->WantNeedRecover() && pTable->GetHostID() == hostID && pTable->GetClubID() == 0)
		{
			vecTables.push_back(pTable);
		}
	}
}

//获得自己参与的桌子
void CGameRoom::GetSelfJoinTables(uint32_t playerID, vector<CGameTable*>& vecTables)
{
	vecTables.clear();
	for (auto& it : m_mpTables)
	{
		CGameTable* pTable = it.second;
		if (!pTable->WantNeedRecover() && pTable->IsExistPlayer(playerID))
		{
			vecTables.push_back(pTable);
		}
	}
}

//获得俱乐部桌子
void CGameRoom::GetClubTables(int64_t clubID, vector<CGameTable*>& vecTables)
{
	vecTables.clear();
	for (auto& it : m_mpTables)
	{
		CGameTable* pTable = it.second;
		if (pTable->GetClubID() == clubID)
		{
			vecTables.push_back(pTable);
		}
	}
}

CGameTable* CGameRoom::GetClubTable(int64_t clubID, int64_t floor, uint16_t pos)
{
	for (auto& it : m_mpTables)
	{
		CGameTable* pTable = it.second;
		if (pTable->GetClubID() == clubID
				&& pTable->GetClubFloor() == floor
				&& pTable->GetClubPos() == pos)
		{
			return pTable;
		}
	}
	return NULL;
}

bool CGameRoom::CanCreatePlayerTable(stTableConf& cfg)
{
	if (GetRoomType() != emROOM_TYPE_PRIVATE)
	{
		return false;
	}
	if (cfg.dueround < 1 || cfg.seatNum < 2)
	{
		LOG_ERROR("游戏局数错误:{}--{}", cfg.dueround, cfg.seatNum);
		return false;
	}
	// 控制创建房间总数
	if (cfg.clubID == 0)
	{
		vector<CGameTable*> vecTables;
		GetTableHostID(cfg.hostID, vecTables);
		if (vecTables.size() > 50)
		{
			LOG_ERROR("最多同时开启房间数:{}", vecTables.size());
			return false;
		}
	}

	return true;
}

CGameTable* CGameRoom::CreatePlayerTable(stTableConf& cfg)
{
	if (!CanCreatePlayerTable(cfg))
		return NULL;

	LOG_DEBUG("创建私人房:createType:{}--round:{}--score:{}", cfg.createType, cfg.dueround, cfg.baseScore);
	CGamePrivateTable* pTable = CreatePrivateTable(0, cfg.playType);
	pTable->SetTableConf(cfg);
	pTable->Init();
	if (!pTable->CreatePrivateTable())
	{
		LOG_ERROR("create private table oper faild");
		SAFE_DELETE(pTable);
		return NULL;
	}
	pTable->ResetTable();
	m_mpTables.insert(make_pair(pTable->GetTableID(), pTable));
	pTable->FlushClubPrivateListToClient();
	return pTable;
}

bool CGameRoom::LoadPrivateTable(stPrivateTable& table)
{
	if (GetRoomType() != emROOM_TYPE_PRIVATE)
	{
		LOG_DEBUG("不是私人房:{}--{}", GetRoomID(), GetRoomType());
		return false;
	}
	if (GetTable(table.tableID) != NULL)
	{
		LOG_DEBUG("私人房桌子已经存在:{}", table.tableID);
		return false;
	}
	CGamePrivateTable* pTable = CreatePrivateTable(table.tableID, table.playType);
	pTable->LoadPrivateTable(table);
	pTable->Init();
	pTable->CheckPrivateTableConfRight();
	pTable->ResetTable();

	m_mpTables.insert(make_pair(pTable->GetTableID(), pTable));
	LOG_DEBUG("load private room:{}--{}", pTable->GetTableID(), pTable->GetPasswd());
	m_roomIndex = table.tableID++;

	pTable->FlushClubPrivateListToClient();
	//重新加载的桌子强制解散
	pTable->ForceGameOver(false);

	return true;
}

// 检测回收空桌子
void CGameRoom::CheckRecover()
{
	vector<CGameTable*> vecRecovers;
	for (auto& it : m_mpTables)
	{
		CGameTable* pTable = it.second;
		if (pTable->NeedRecover())
		{
			vecRecovers.push_back(pTable);
		}
	}
	for_each(vecRecovers.begin(), vecRecovers.end(), [&](CGameTable* pTable)
	{
	  FreeTable(pTable);
	});
}

// 检测是否需要生成新桌子
void CGameRoom::CheckNewTable()
{
	if (GetRoomType() != emROOM_TYPE_COMMON || IsNeedMarry())
		return;

	if (GetFreeTableNum() < 1)
	{
		LOG_DEBUG("not free table and create table");
		MallocTable();
	}
}

// 匹配桌子
void CGameRoom::MarryTable()
{
	if (GetRoomType() != emROOM_TYPE_PRIVATE && IsNeedMarry())// 不是私人房
	{
		CGameTable* pTable = NULL;
		while (!m_marryPlayers.empty())
		{
			map<CGamePlayer*, uint32_t>::iterator it = m_marryPlayers.begin();
			CGamePlayer* pPlayer = it->first;
			if (JoinNoFullTable(pPlayer, it->second))
			{
				LeaveMarry(pPlayer);
				continue;
			}
			if (pTable == NULL || pTable->IsFullTable())
			{
				pTable = MallocTable();
			}
			if (pTable->EnterTable(pPlayer))
			{
				LeaveMarry(pPlayer);
			}
			else
			{
				LeaveRoom(pPlayer);
			}
		}
	}
}

// 进入空闲桌子
bool CGameRoom::JoinNoFullTable(CGamePlayer* pPlayer, uint32_t excludeID)
{
	vector<CGameTable*> readyTables;
	for (auto & it : m_mpTables)
	{
		CGameTable* pTable = it.second;
		if (pTable->GetTableID() != excludeID && pTable->CanEnterTable(pPlayer) == net::RESULT_CODE_SUCCESS)
		{
			readyTables.push_back(pTable);
		}
	}
	std::sort(readyTables.begin(), readyTables.end(), [](auto pTable1, auto pTable2)
	{
	  if (pTable1->GetReadyNum() == pTable2->GetReadyNum())
	  {
		  if (pTable1->GetPlayerNum() == pTable2->GetPlayerNum())
		  {
			  return pTable1->GetTableID() < pTable2->GetTableID();
		  }
		  return pTable1->GetPlayerNum() > pTable2->GetPlayerNum();
	  }
	  return pTable1->GetReadyNum() > pTable2->GetReadyNum();
	});
	for (auto pTable : readyTables)
	{
		if (pTable->EnterTable(pPlayer))
		{
			return true;
		}
	}
	return false;
}

// 加入匹配
void CGameRoom::JoinMarry(CGamePlayer* pPlayer, uint32_t excludeID)
{
	if (GetRoomType() == emROOM_TYPE_PRIVATE)
		return;
	if (IsJoinMarry(pPlayer))
		return;
	m_marryPlayers.insert(make_pair(pPlayer, excludeID));
}

void CGameRoom::LeaveMarry(CGamePlayer* pPlayer)
{
	if (GetRoomType() == emROOM_TYPE_PRIVATE)
		return;

	m_marryPlayers.erase(pPlayer);
}

bool CGameRoom::IsJoinMarry(CGamePlayer* pPlayer)
{
	return (m_marryPlayers.find(pPlayer) != m_marryPlayers.end());
}

uint32_t CGameRoom::GetFreeTableNum()
{
	uint32_t num = m_freeTable.size();
	for (auto& it : m_mpTables)
	{
		CGameTable* pTable = it.second;
		if (!pTable->IsFullTable())
			num++;
	}
	return num;
}

void CGameRoom::CalcShowOnline()
{
	if (m_roomCfg.showonline == 0)
	{
		m_showonline = m_playerNum;
	}
	else
	{
		m_showonline = m_playerNum*m_roomCfg.showonline + g_RandGen.RandUInt()%100;
	}
}


