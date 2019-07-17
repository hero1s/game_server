
#include "../game_room_mgr.h"
#include <data_cfg_mgr.h>
#include "packet/inner_protobuf_pkg.h"
#include "../stdafx.h"
#include "msg_lobby_handle.h"
#include "msg_define.pb.h"
#include "../game_table_private.h"
#include "game_define.h"

using namespace Network;
using namespace svrlib;
using namespace net;

CHandleLobbyMsg::CHandleLobbyMsg()
{
	bind_handler(this, net::L2S_MSG_REGISTER_REP, &CHandleLobbyMsg::handle_msg_register_rep);
	bind_handler(this, net::L2S_MSG_NOTIFY_NET_STATE, &CHandleLobbyMsg::handle_msg_notify_net_state);

	bind_handler(this, net::L2S_MSG_NOTIFY_STOP_SERVICE, &CHandleLobbyMsg::handle_msg_notify_stop_server);
	bind_handler(this, net::L2S_MSG_FLUSH_CHANGE_ACC_DATA, &CHandleLobbyMsg::handle_msg_flush_change_acc_data);
	bind_handler(this, net::L2S_MSG_NOTIFY_CLOSE_TABLE, &CHandleLobbyMsg::handle_msg_delete_table);
	bind_handler(this, net::C2S_MSG_CLOSE_CLUB_TABLE_REQ, &CHandleLobbyMsg::handle_msg_close_club_table);
	bind_handler(this, net::L2S_MSG_NOTIFY_RELOAD_CFG, &CHandleLobbyMsg::handle_msg_reload_cfg);

	bind_handler(this, net::L2S_MSG_ENTER_INTO_SVR, &CHandleLobbyMsg::handle_msg_enter_svr);

	bind_handler(this, net::C2S_MSG_REQ_ROOMS_INFO, &CHandleLobbyMsg::handle_msg_req_rooms_info);
	bind_handler(this, net::C2S_MSG_BACK_LOBBY, &CHandleLobbyMsg::handle_msg_back_lobby);
	bind_handler(this, net::C2S_MSG_ENTER_ROOM, &CHandleLobbyMsg::handle_msg_enter_room);
	bind_handler(this, net::C2S_MSG_REQ_TABLE_LIST, &CHandleLobbyMsg::handle_msg_req_table_list);
	bind_handler(this, net::C2S_MSG_REQ_CREATE_TABLE, &CHandleLobbyMsg::handle_msg_req_create_table);
	bind_handler(this, net::C2S_MSG_LEAVE_TABLE_REQ, &CHandleLobbyMsg::handle_msg_leave_table_req);
	bind_handler(this, net::C2S_MSG_ENTER_TABLE_REQ, &CHandleLobbyMsg::handle_msg_enter_table);
	bind_handler(this, net::C2S_MSG_TABLE_READY, &CHandleLobbyMsg::handle_msg_table_ready);
	bind_handler(this, net::C2S_MSG_TABLE_CHAT, &CHandleLobbyMsg::handle_msg_table_chat);
	bind_handler(this, net::C2S_MSG_TABLE_SET_AUTO, &CHandleLobbyMsg::handle_msg_table_set_auto);
	bind_handler(this, net::C2S_MSG_FAST_JOIN_ROOM, &CHandleLobbyMsg::handle_msg_fast_join_room);
	bind_handler(this, net::C2S_MSG_FAST_JOIN_TABLE, &CHandleLobbyMsg::handle_msg_fast_join_table);
	bind_handler(this, net::C2S_MSG_QUERY_TABLE_LIST_REQ, &CHandleLobbyMsg::handle_msg_query_table_list);
	bind_handler(this, net::C2S_MSG_SITDOWN_STANDUP, &CHandleLobbyMsg::handle_msg_sitdown_standup);

}
CHandleLobbyMsg::~CHandleLobbyMsg()
{

}

int CHandleLobbyMsg::OnRecvClientMsg()
{
	//LOG_DEBUG("收到大厅服务器消息:uin:{}--cmd:{}",head->uin,head->cmd);
	if (CProtobufHandleBase::OnRecvClientMsg() == 1)
	{
		return handle_msg_gameing_oper();
	}
	return 0;
}

// 游戏内消息
int CHandleLobbyMsg::handle_msg_gameing_oper()
{
	//LOG_DEBUG("游戏内部消息:uid:{}--msg:{}",head->uin,head->cmd);
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
	{
		return 0;
	}
	CGameTable* pGameTable = pGamePlayer->GetTable();
	if (pGameTable != NULL)
	{
		pGameTable->OnMessage(pGamePlayer, _head->cmd, _pkt_buf, _buf_len);
	}
	return 0;
}

int CHandleLobbyMsg::handle_msg_register_rep()
{
	net::msg_register_svr_rep msg;
	PARSE_MSG(msg);

	LOG_DEBUG("Register Server Rep :{}", msg.result());
	if (msg.result() == 1)
	{
		CLobbyMgr::Instance().RegisterRep(_pNetObj->GetUID(), true);
	}
	else
	{
		CLobbyMgr::Instance().RegisterRep(_pNetObj->GetUID(), false);
		LOG_ERROR("register server fail {} -->:{}", _pNetObj->GetUID(), CApplication::Instance().GetServerID());
	}
	return 0;
}

// 通知网络状态
int CHandleLobbyMsg::handle_msg_notify_net_state()
{
	net::msg_notify_net_state msg;
	PARSE_MSG(msg);
	uint32_t uid      = msg.uid();
	uint8_t  state    = msg.state();
	uint32_t loginIp  = msg.newip();
	uint8_t  noPlayer = msg.no_player();
	LOG_DEBUG("通知网络状态:{}-->{}-->noplayer:{}", uid, state, noPlayer);
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer != NULL)
	{
		if (noPlayer == 1)
		{
			pGamePlayer->SetNetStateNoPlayer();
		}
		else
		{
			pGamePlayer->SetIP(loginIp);
			pGamePlayer->SetNetState(state);
		}
	}
	return 0;
}

// 通知服务器维护
int CHandleLobbyMsg::handle_msg_notify_stop_server()
{
	net::msg_notify_stop_service msg;
	PARSE_MSG(msg);
	LOG_DEBUG("游戏服务器维护");
	CRedisMgr::Instance().CheckCurGroup();

	return 0;
}

int CHandleLobbyMsg::handle_msg_flush_change_acc_data()
{
	net::msg_flush_change_account_data msg;
	PARSE_MSG(msg);
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
		return 0;

	pGamePlayer->ChangeAccountValue(emACC_VALUE_COIN, msg.coin());
	pGamePlayer->ChangeAccountValue(emACC_VALUE_SAFECOIN, msg.safe_coin());
	pGamePlayer->SetCurClubID(msg.club_id(), false);
	pGamePlayer->SetLat(msg.lat());
	pGamePlayer->SetLon(msg.lon());

	return 0;
}

// 进入游戏服务器
int CHandleLobbyMsg::handle_msg_enter_svr()
{
	net::msg_enter_into_game_svr msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	//LOG_DEBUG("进入游戏服务器:{}",uid);

	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer != NULL)
	{
		pGamePlayer->ReLogin();

	}
	else
	{
		if (msg.player_type() == PLAYER_TYPE_ONLINE)
		{
			pGamePlayer = new CGamePlayer(PLAYER_TYPE_ONLINE);
			pGamePlayer->SetPlayerGameData(msg);
			pGamePlayer->OnLogin();
			CPlayerMgr::Instance().AddPlayer(pGamePlayer);

		}
		else
		{
			LOG_ERROR("error player type :{}", msg.player_type());
			return 0;
		}
	}
	pGamePlayer->SetLoginLobbySvrID(_pNetObj->GetUID(), false);

	net::msg_enter_gamesvr_rep msgrep;
	msgrep.set_result(RESULT_CODE_SUCCESS);
	msgrep.set_svrid(CDataCfgMgr::Instance().GetCurSvrCfg().svrid);
	msgrep.set_play_type(msg.play_type());

	pGamePlayer->SendMsgToClient(&msgrep, net::S2C_MSG_ENTER_SVR_REP);

	CDBMysqlMgr::Instance().UpdatePlayerOnlineInfo(pGamePlayer->GetUID(), CApplication::Instance().GetServerID(), 0,
	                                               pGamePlayer->GetPlayerType());

	return 0;
}

// 请求房间列表信息
int CHandleLobbyMsg::handle_msg_req_rooms_info()
{
	net::msg_rooms_info_req msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	LOG_DEBUG("请求房间列表信息:{}", uid);
	CGameRoomMgr::Instance().SendRoomList2Client(uid);

	return 0;
}

// 返回大厅
int CHandleLobbyMsg::handle_msg_back_lobby()
{
	net::msg_back_lobby_req msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	LOG_DEBUG("请求返回大厅:{} -- {}", uid, msg.is_action());
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer != NULL)
	{
		if (!pGamePlayer->CanBackLobby())
		{
			net::msg_back_lobby_rep rep;
			rep.set_result(RESULT_CODE_FAIL);
			rep.set_reason(RESULT_CODE_GAMEING);
			pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_BACK_LOBBY_REP);
			LOG_DEBUG("游戏状态中不能返回大厅");
			return 0;
		}
		else
		{
			CPlayerMgr::Instance().RecoverPlayer(pGamePlayer);
		}
		return 0;
	}
	else
	{
		if (msg.is_action() > 0)
		{
			LOG_ERROR("服务器主动请求返回大厅:{}");
			net::msg_leave_svr msg;
			msg.set_uid(uid);
			ReplyMsg(&msg, S2L_MSG_LEAVE_SVR, uid);
		}
	}

	LOG_DEBUG("发送返回大厅消息:{}", uid);
	net::msg_back_lobby_rep rep;
	rep.set_result(RESULT_CODE_SUCCESS);
	rep.set_reason(RESULT_CODE_SUCCESS);
	ReplyMsg(&rep, net::S2C_MSG_BACK_LOBBY_REP, uid);
	return 0;
}

// 进入房间
int CHandleLobbyMsg::handle_msg_enter_room()
{
	net::msg_enter_room_req msg;
	PARSE_MSG(msg);
	uint32_t uid    = _head->uin;
	uint16_t roomID = msg.room_id();
	LOG_DEBUG("请求进入房间:{}-{}", uid, roomID);
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
	{
		LOG_DEBUG("请求进入房间的玩家不存在");
		return 0;
	}
	net::msg_enter_room_rep rep;
	CGameRoom* pOldRoom = pGamePlayer->GetRoom();
	if (pOldRoom != NULL && pOldRoom->GetRoomID() == roomID)
	{
		LOG_DEBUG("已经在房间中了:{}", roomID);
		pOldRoom->EnterRoom(pGamePlayer);
		return 0;
	}
	if (!pGamePlayer->CanBackLobby())
	{
		LOG_DEBUG("已经在座位上了不能换房:{}", uid);
		rep.set_result(0);
		pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_ENTER_ROOM_REP);
		return 0;
	}

	if (pOldRoom != NULL)
	{
		pOldRoom->LeaveRoom(pGamePlayer);
	}
	CGameRoom* pNewRoom = CGameRoomMgr::Instance().GetRoom(roomID);
	if (pNewRoom == NULL || !pNewRoom->CanEnterRoom(pGamePlayer))
	{
		LOG_DEBUG("房间不存在或者不能进入:uid:{}--room:{}", uid, roomID);
		rep.set_result(0);
		pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_ENTER_ROOM_REP);
		return 0;
	}
	pNewRoom->EnterRoom(pGamePlayer);

	return 0;
}

// 请求桌子列表
int CHandleLobbyMsg::handle_msg_req_table_list()
{
	net::msg_table_list_req msg;
	PARSE_MSG(msg);
	uint32_t uid       = _head->uin;
/*	LOG_DEBUG("请求桌子列表:{},gametype:{},seachtype:{},tbl:{},clubid:{},playType:{}", uid, msg.game_type(),
	           msg.seach_type(), msg.table_id(), msg.club_id(), msg.play_type());*/
	uint8_t  seachType = msg.seach_type();
	int64_t  tableID   = msg.table_id();
	int64_t  clubID    = msg.club_id();
	uint16_t playType  = msg.play_type();
	if (msg.game_type() == CDataCfgMgr::Instance().GetCurSvrCfg().gameType)
	{
		if (clubID != 0)
		{
			CGameRoomMgr::Instance().SendClubPrivateList(uid, clubID, _pNetObj->GetUID());
		}
		else
		{
			CGameRoomMgr::Instance().SendAllPrivateTableList2Client(uid, _pNetObj->GetUID());
		}
		return 0;
	}
	else
	{
		LOG_ERROR("游戏类型错误:{}", msg.game_type());
	}

	return 0;
}

// 创建桌子
int CHandleLobbyMsg::handle_msg_req_create_table()
{
	net::msg_create_table_req msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	LOG_DEBUG("请求创建桌子:{}--club:{}", uid, msg.club_id());
	DUMP_PROTO_MSG_INFO(msg);

	uint8_t                     bRet = net::RESULT_CODE_SUCCESS;
	net::msg_create_table_rep rep;
	rep.set_result(bRet);
	rep.set_table_id(0);
	rep.set_game_type(msg.game_type());
	rep.set_play_type(msg.play_type());

	//维护状态不能创建桌子
	if (CApplication::Instance().GetStatus() == emSERVER_STATE_REPAIR)
	{
		LOG_DEBUG("维护状态不能创建桌子");
		rep.set_result(net::RESULT_CODE_SVR_REPAIR);
		ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
		return 0;
	}

	CGamePlayer* pGamePlayer = GetGamePlayer();
	CGameRoom  * pRoom       = NULL;
	uint32_t hostID     = 0;
	int64_t  cur_clubID = 0;
	if (pGamePlayer != NULL)
	{
		cur_clubID = pGamePlayer->GetCurClubID();
	}
	else
	{
		cur_clubID = CRedisMgr::Instance().GetPlayerOnlineInfo(_head->uin, "cid");
		LOG_ERROR("玩家没有进入游戏服:{}", uid);
	}

	if (cur_clubID != msg.club_id())
	{
		LOG_DEBUG("当前不在俱乐部，不能创建俱乐部桌子:{}---{}", cur_clubID, msg.club_id());
		rep.set_result(net::RESULT_CODE_ERROR_STATE);
		ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
		return 0;
	}

	if (msg.club_id() == 0)
	{// 普通开房
		if (pGamePlayer == NULL)
		{
			LOG_ERROR("普通开房玩家不存在");
			rep.set_result(net::RESULT_CODE_FAIL);
			ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
			return 0;
		}
		pRoom = pGamePlayer->GetRoom();
		if (pRoom == NULL || pRoom->GetRoomType() != emROOM_TYPE_PRIVATE)
		{
			LOG_DEBUG("不在房间中或者不能创建房间:{}-->{}", uid, pGamePlayer->GetRoomID());
			rep.set_result(net::RESULT_CODE_FAIL);
			ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
			return 0;
		}
		hostID = pGamePlayer->GetUID();
	}
	else
	{// 俱乐部开房
		hostID = CRedisMgr::Instance().GetClubFiled(msg.club_id(), "host");
		pRoom  = CGameRoomMgr::Instance().GetPrivateRoom();
		if (hostID == 0 || pRoom == NULL)
		{
			LOG_DEBUG("俱乐部房主ID未取到:{}", msg.club_id());
			rep.set_result(net::RESULT_CODE_ERROR_PARAM);
			ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
			return 0;
		}
		if (msg.game_type() != pRoom->GetGameType() || msg.club_pos() > 100)
		{
			LOG_DEBUG("俱乐部开房选择游戏错误：gameType:{}--clubPos:{}", msg.game_type(), msg.club_pos());
			rep.set_result(net::RESULT_CODE_ERROR_PARAM);
			ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
			return 0;
		}
		//俱乐部如果位置已经开房，则进入
		CGameTable* pTable = pRoom->GetClubTable(msg.club_id(), msg.club_floor(), msg.club_pos());
		if (pTable != NULL)
		{
			LOG_DEBUG("房间已开启，创建失败:{}--{}--{}", msg.club_id(), msg.club_floor(), msg.club_pos());
			rep.set_result(net::RESULT_CODE_EXIST_OBJ);
			ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
			return 0;
		}
	}

	stTableConf conf;
	conf.baseScore = msg.base_score();
	RANGE(conf.baseScore, 1, 10);
	conf.consume  = (msg.consume() != net::ROOM_CONSUME_TYPE_COIN) ? net::ROOM_CONSUME_TYPE_SCORE
	                                                               : net::ROOM_CONSUME_TYPE_COIN;
	conf.enterMin = msg.enter_min();
	conf.playType = msg.play_type();
	conf.hostID   = hostID;
	conf.dueTime  = getSysTime() + SECONDS_IN_30_MIN;//30'有效期

	conf.seatNum    = msg.seat_num();
	conf.createType = msg.create_type();
	conf.addParam   = msg.add_param();
	conf.matchParam = msg.match_param();
	conf.rentType   = msg.rent_type();
	conf.clubID     = msg.club_id();
	conf.clubFloor  = msg.club_floor();
	conf.clubPos    = msg.club_pos();
	conf.idxRound   = msg.round_select();

	if (msg.game_type() != pRoom->GetGameType())
	{
		LOG_ERROR("游戏类型不匹配:{}--{}", msg.game_type(), pRoom->GetGameType());
		rep.set_result(net::RESULT_CODE_ERROR_PARAM);
		ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
		return 0;
	}
	if (conf.createType == emPRIVATE_CREATE_SCORE)
	{
		if (!CCommonLogic::IsOpenScoreMode(msg.game_type(), msg.play_type()) || !CDataCfgMgr::Instance().IsOpenContest(msg.game_type(), msg.play_type()))
		{
			LOG_ERROR("创建房间模式不匹配:{}--{}", msg.game_type(), msg.play_type());
			rep.set_result(net::RESULT_CODE_ERROR_PARAM);
			ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
			return 0;
		}
	}
	int64_t card = CDataCfgMgr::Instance().CalcPrivateCardCfg(pRoom->GetGameType(), conf.playType, msg.round_select(),
	                                                        conf.rentType, conf.dueround);
	if (card < 0)
	{
		LOG_ERROR("没有找到游戏房卡消费配置信息");
		rep.set_result(net::RESULT_CODE_ERROR_PARAM);
		ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
		return 0;
	}

	if (!pRoom->CanCreatePlayerTable(conf))
	{
		bRet = net::RESULT_CODE_CION_ERROR;
	}
	else
	{// 检测配置是否正常，扣费
		if (conf.rentType == 0)
		{
			if (card > 0)
			{
				if (CLobbyMgr::Instance().FrozenPlayerRoomCard(conf.hostID, conf.clubID, card) == false)
				{
					LOG_ERROR("create table fee not enough :{}--{}", card, conf.dueround);
					bRet = net::RESULT_CODE_NOT_DIAMOND;
				}
			}
		}
		else
		{
			rep.set_result(net::RESULT_CODE_ERROR_PARAM);
			ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
			LOG_ERROR("暂时不支持AA支付:{}", uid);
			return 0;
		}

		conf.rentFee = card;
	}

	if (bRet == net::RESULT_CODE_SUCCESS)
	{
		conf.passwd = CRedisMgr::Instance().GetTablePasswdFromCache();

		//同步创建
		CGameTable* pTable = pRoom->CreatePlayerTable(conf);
		if (pTable == NULL)
		{// 失败退款
			rep.set_result(net::RESULT_CODE_FAIL);
			LOG_DEBUG("创建桌子失败");
			if (conf.rentType == 0)
			{
				CLobbyMgr::Instance().UnfrozenPlayerRoomCard(conf.hostID, conf.clubID, card);
			}
			CRedisMgr::Instance().DelTablePasswdFromCache(conf.passwd);
		}
		else
		{
			rep.set_table_id(pTable->GetTableID());
			rep.set_passwd(pTable->GetPasswd());

			net::table_svrinfo info;
			info.set_game_type(pTable->GetHostRoom()->GetGameType());
			info.set_play_type(pTable->GetPlayType());
			info.set_club_id(pTable->GetClubID());
			info.set_club_floor(pTable->GetClubFloor());

			CRedisMgr::Instance().SetTableOnlineSvrInfo(pTable->GetPasswd(), info);
		}

	}
	else
	{
		rep.set_result(bRet);
		rep.set_table_id(0);
		rep.set_passwd("");
	}

	ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);

	return 0;
}

// 离开桌子
int CHandleLobbyMsg::handle_msg_leave_table_req()
{
	net::msg_leave_table_req msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	LOG_DEBUG("请求离开桌子:{}", uid);
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
		return 0;
	CGameTable* pTable = pGamePlayer->GetTable();
	if (pTable == NULL)
	{
		LOG_DEBUG("不在桌子上:{}", uid);
		return 0;
	}
	CGameRoom* pRoom = pGamePlayer->GetRoom();
	net::msg_leave_table_rep rep;
	if (pTable->CanLeaveTable(pGamePlayer))
	{
		pTable->LeaveTable(pGamePlayer);
		rep.set_result(1);
		rep.set_is_host(pGamePlayer->GetUID() == pTable->GetHostID() ? 1 : 0);
		if (pRoom != NULL && pRoom->GetRoomType() == emROOM_TYPE_COMMON)
		{
			pRoom->LeaveRoom(pGamePlayer);
		}
	}
	else
	{
		LOG_DEBUG("不能离开桌子:{}", uid);
		rep.set_result(0);
	}
	pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_LEAVE_TABLE_REP);

	return 0;
}

// 进入桌子
int CHandleLobbyMsg::handle_msg_enter_table()
{
	net::msg_enter_table_req msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	LOG_DEBUG("req enter table:uid:{}-->tblid:{}-->passwd:{}", uid, msg.table_id(), msg.passwd());
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
		return 0;
	CGameRoom* pRoom = pGamePlayer->GetRoom();
	if (pRoom == NULL)
	{
		LOG_DEBUG("change table not in room:{}", uid);
		return 0;
	}
	uint8_t bRet = pRoom->EnterTable(pGamePlayer, msg.table_id(), msg.passwd());
	if ((msg.table_id() != 0 || msg.passwd().length() > 1) && bRet == RESULT_CODE_SUCCESS)// 特殊处理，私人房进入成功不返还
		return 0;

	net::msg_enter_table_rep rep;
	rep.set_result(bRet);
	rep.set_table_id(msg.table_id());
	pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_ENTER_TABLE);
	return 0;
}

// 桌子准备
int CHandleLobbyMsg::handle_msg_table_ready()
{
	net::msg_table_ready_req msg;
	PARSE_MSG(msg);
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
	{
		LOG_DEBUG("玩家对象为NULL");
		return 0;
	}
	LOG_DEBUG("桌子准备:{}", pGamePlayer->GetUID());
	CGameTable* pTable = pGamePlayer->GetTable();
	if (pTable != NULL)
	{
		pTable->PlayerReady(pGamePlayer, msg.ready() > 0 ? true : false);
		pGamePlayer->SetAutoReady(false);
	}
	else
	{
		LOG_ERROR("table is not exist ,player ready table is null");
	}
	return 0;
}

// 桌子聊天
int CHandleLobbyMsg::handle_msg_table_chat()
{
	net::msg_table_chat_req msg;
	PARSE_MSG(msg);

	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
		return 0;

	CGameTable* pTable = pGamePlayer->GetTable();
	if (pTable != NULL)
	{
		net::msg_table_chat_rep rep;
		rep.set_uid(pGamePlayer->GetUID());
		rep.set_chat_msg(msg.chat_msg());
		pTable->TableMsgToAll(&rep, net::S2C_MSG_TABLE_CHAT, false);
	}
	return 0;
}

// 桌子设置托管
int CHandleLobbyMsg::handle_msg_table_set_auto()
{
	net::msg_table_set_auto_req msg;
	PARSE_MSG(msg);
	LOG_DEBUG("桌子设置托管:{}", msg.auto_type());
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
		return 0;

	CGameTable* pTable = pGamePlayer->GetTable();
	if (pTable != NULL)
	{
		pTable->PlayerSetAuto(pGamePlayer, msg.auto_type());
	}
	return 0;
}

// 快速开始
int CHandleLobbyMsg::handle_msg_fast_join_room()
{
	net::msg_fast_join_room_req msg;
	PARSE_MSG(msg);
	LOG_DEBUG("快速加入房间");
	CGamePlayer* pGamePlayer         = GetGamePlayer();
	if (pGamePlayer == NULL)
		return 0;
	net::msg_fast_join_room_rep rep;
	uint8_t                       bRet = CGameRoomMgr::Instance().FastJoinRoom(pGamePlayer, msg.consume())
	                                   ? RESULT_CODE_SUCCESS
	                                   : RESULT_CODE_FAIL;
	rep.set_result(bRet);
	pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_FAST_JOIN_ROOM_REP);
	return 0;
}

// 快速换桌
int CHandleLobbyMsg::handle_msg_fast_join_table()
{
	LOG_DEBUG("快速换桌");
	net::msg_fast_join_table_req msg;
	PARSE_MSG(msg);
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
		return 0;
	net::msg_fast_join_table_rep rep;
	rep.set_result(0);
	CGameRoom* pRoom = pGamePlayer->GetRoom();
	if (pRoom != NULL)
	{
		if (pRoom->FastJoinTable(pGamePlayer))
		{
			rep.set_result(1);
		}
	}
	pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_FAST_JOIN_TABLE_REP);
	return 0;
}

// 查看桌子信息
int CHandleLobbyMsg::handle_msg_query_table_list()
{
	net::msg_query_table_list_req msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	LOG_DEBUG("查看桌子信息:{}", uid);
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
		return 0;
	CGameRoom* pRoom = pGamePlayer->GetRoom();
	if (pRoom == NULL)
	{
		LOG_DEBUG("玩家没有进入房间:{}", uid);
		return 0;
	}
	uint32_t startID = msg.startid();
	uint32_t endID   = msg.endid();
	pRoom->QueryTableListToPlayer(pGamePlayer, startID, endID);

	return 0;
}

// 站立做起
int CHandleLobbyMsg::handle_msg_sitdown_standup()
{
	if (CApplication::Instance().GetStatus() == emSERVER_STATE_REPAIR)
	{
		LOG_ERROR("维护状态,直接掉线切服");
		return 0;
	}
	net::msg_sitdown_standup_req msg;
	PARSE_MSG(msg);
	LOG_DEBUG("站立坐下:{}", msg.oper_id());
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
		return 0;

	CGameTable* pTable = pGamePlayer->GetTable();
	if (pTable != NULL)
	{
		net::msg_sitdown_standup_rep rep;

		if (pTable->PlayerSitDownStandUp(pGamePlayer, msg.oper_id(), msg.chair_id()))
		{
			rep.set_result(net::RESULT_CODE_SUCCESS);
		}
		else
		{
			rep.set_result(net::RESULT_CODE_FAIL);
		}

		rep.set_oper_id(msg.oper_id());
		rep.set_chair_id(msg.chair_id());
		pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_SITDOWN_STANDUP);
	}
	return 0;
}

// 删除房间
int CHandleLobbyMsg::handle_msg_delete_table()
{
	net::msg_notify_close_table msg;
	PARSE_MSG(msg);
	LOG_DEBUG("强制结束房间:{}", msg.table_id());
	CGameRoomMgr::Instance().ForceClosePlayerTable(msg.table_id());

	return 0;
}
// 删除房间
int CHandleLobbyMsg::handle_msg_close_club_table()
{
	net::msg_close_club_table_req msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	LOG_DEBUG("玩家解散桌子:{}--{}", uid, msg.passwd());
	CGameRoom* pPrivateRoom = CGameRoomMgr::Instance().GetPrivateRoom();
	if (pPrivateRoom == NULL)return 0;
	CGameTable* pTable = pPrivateRoom->GetTableByPasswd(msg.passwd());

	msg_close_club_table_rep rep;
	rep.set_club_id(msg.club_id());
	rep.set_passwd(msg.passwd());
	rep.set_result(net::RESULT_CODE_FAIL);
	if (pTable == NULL)
	{
		LOG_DEBUG("桌子不存在");
		ReplyMsg(&rep, S2C_MSG_CLOSE_CLUB_TABLE_REP, uid);
		return 0;
	}
	if (pTable->GetHostID() != uid)
	{
		LOG_DEBUG("不是房主不能解散");
		ReplyMsg(&rep, S2C_MSG_CLOSE_CLUB_TABLE_REP, uid);
		return 0;
	}
	pTable->ForceGameOver(true);
	rep.set_result(net::RESULT_CODE_SUCCESS);
	ReplyMsg(&rep, S2C_MSG_CLOSE_CLUB_TABLE_REP, uid);
	return 0;
}
// 重载配置
int CHandleLobbyMsg::handle_msg_reload_cfg()
{
	net::msg_notify_reload_cfg msg;
	PARSE_MSG(msg);
	LOG_DEBUG("重载配置文件:{}", msg.reason());
	CApplication::Instance().ConfigurationChanged();
	return 0;
}

CGamePlayer* CHandleLobbyMsg::GetGamePlayer()
{
	CGamePlayer* pPlayer = (CGamePlayer*) CPlayerMgr::Instance().GetPlayer(_head->uin);
	if (pPlayer == NULL)
	{
		//LOG_DEBUG("游戏玩家不存在:{}",head->uin);
	}
	else
	{
		pPlayer->ResetHeart();
	}
	return pPlayer;
}

void CHandleLobbyMsg::ReplyMsg(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin)
{
	SendInnerMsg(_pNetObj, msg, msg_type, uin);
}









