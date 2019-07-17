
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
	//LOG_DEBUG("�յ�������������Ϣ:uin:{}--cmd:{}",head->uin,head->cmd);
	if (CProtobufHandleBase::OnRecvClientMsg() == 1)
	{
		return handle_msg_gameing_oper();
	}
	return 0;
}

// ��Ϸ����Ϣ
int CHandleLobbyMsg::handle_msg_gameing_oper()
{
	//LOG_DEBUG("��Ϸ�ڲ���Ϣ:uid:{}--msg:{}",head->uin,head->cmd);
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

// ֪ͨ����״̬
int CHandleLobbyMsg::handle_msg_notify_net_state()
{
	net::msg_notify_net_state msg;
	PARSE_MSG(msg);
	uint32_t uid      = msg.uid();
	uint8_t  state    = msg.state();
	uint32_t loginIp  = msg.newip();
	uint8_t  noPlayer = msg.no_player();
	LOG_DEBUG("֪ͨ����״̬:{}-->{}-->noplayer:{}", uid, state, noPlayer);
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

// ֪ͨ������ά��
int CHandleLobbyMsg::handle_msg_notify_stop_server()
{
	net::msg_notify_stop_service msg;
	PARSE_MSG(msg);
	LOG_DEBUG("��Ϸ������ά��");
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

// ������Ϸ������
int CHandleLobbyMsg::handle_msg_enter_svr()
{
	net::msg_enter_into_game_svr msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	//LOG_DEBUG("������Ϸ������:{}",uid);

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

// ���󷿼��б���Ϣ
int CHandleLobbyMsg::handle_msg_req_rooms_info()
{
	net::msg_rooms_info_req msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	LOG_DEBUG("���󷿼��б���Ϣ:{}", uid);
	CGameRoomMgr::Instance().SendRoomList2Client(uid);

	return 0;
}

// ���ش���
int CHandleLobbyMsg::handle_msg_back_lobby()
{
	net::msg_back_lobby_req msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	LOG_DEBUG("���󷵻ش���:{} -- {}", uid, msg.is_action());
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer != NULL)
	{
		if (!pGamePlayer->CanBackLobby())
		{
			net::msg_back_lobby_rep rep;
			rep.set_result(RESULT_CODE_FAIL);
			rep.set_reason(RESULT_CODE_GAMEING);
			pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_BACK_LOBBY_REP);
			LOG_DEBUG("��Ϸ״̬�в��ܷ��ش���");
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
			LOG_ERROR("�������������󷵻ش���:{}");
			net::msg_leave_svr msg;
			msg.set_uid(uid);
			ReplyMsg(&msg, S2L_MSG_LEAVE_SVR, uid);
		}
	}

	LOG_DEBUG("���ͷ��ش�����Ϣ:{}", uid);
	net::msg_back_lobby_rep rep;
	rep.set_result(RESULT_CODE_SUCCESS);
	rep.set_reason(RESULT_CODE_SUCCESS);
	ReplyMsg(&rep, net::S2C_MSG_BACK_LOBBY_REP, uid);
	return 0;
}

// ���뷿��
int CHandleLobbyMsg::handle_msg_enter_room()
{
	net::msg_enter_room_req msg;
	PARSE_MSG(msg);
	uint32_t uid    = _head->uin;
	uint16_t roomID = msg.room_id();
	LOG_DEBUG("������뷿��:{}-{}", uid, roomID);
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
	{
		LOG_DEBUG("������뷿�����Ҳ�����");
		return 0;
	}
	net::msg_enter_room_rep rep;
	CGameRoom* pOldRoom = pGamePlayer->GetRoom();
	if (pOldRoom != NULL && pOldRoom->GetRoomID() == roomID)
	{
		LOG_DEBUG("�Ѿ��ڷ�������:{}", roomID);
		pOldRoom->EnterRoom(pGamePlayer);
		return 0;
	}
	if (!pGamePlayer->CanBackLobby())
	{
		LOG_DEBUG("�Ѿ�����λ���˲��ܻ���:{}", uid);
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
		LOG_DEBUG("���䲻���ڻ��߲��ܽ���:uid:{}--room:{}", uid, roomID);
		rep.set_result(0);
		pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_ENTER_ROOM_REP);
		return 0;
	}
	pNewRoom->EnterRoom(pGamePlayer);

	return 0;
}

// ���������б�
int CHandleLobbyMsg::handle_msg_req_table_list()
{
	net::msg_table_list_req msg;
	PARSE_MSG(msg);
	uint32_t uid       = _head->uin;
/*	LOG_DEBUG("���������б�:{},gametype:{},seachtype:{},tbl:{},clubid:{},playType:{}", uid, msg.game_type(),
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
		LOG_ERROR("��Ϸ���ʹ���:{}", msg.game_type());
	}

	return 0;
}

// ��������
int CHandleLobbyMsg::handle_msg_req_create_table()
{
	net::msg_create_table_req msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	LOG_DEBUG("���󴴽�����:{}--club:{}", uid, msg.club_id());
	DUMP_PROTO_MSG_INFO(msg);

	uint8_t                     bRet = net::RESULT_CODE_SUCCESS;
	net::msg_create_table_rep rep;
	rep.set_result(bRet);
	rep.set_table_id(0);
	rep.set_game_type(msg.game_type());
	rep.set_play_type(msg.play_type());

	//ά��״̬���ܴ�������
	if (CApplication::Instance().GetStatus() == emSERVER_STATE_REPAIR)
	{
		LOG_DEBUG("ά��״̬���ܴ�������");
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
		LOG_ERROR("���û�н�����Ϸ��:{}", uid);
	}

	if (cur_clubID != msg.club_id())
	{
		LOG_DEBUG("��ǰ���ھ��ֲ������ܴ������ֲ�����:{}---{}", cur_clubID, msg.club_id());
		rep.set_result(net::RESULT_CODE_ERROR_STATE);
		ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
		return 0;
	}

	if (msg.club_id() == 0)
	{// ��ͨ����
		if (pGamePlayer == NULL)
		{
			LOG_ERROR("��ͨ������Ҳ�����");
			rep.set_result(net::RESULT_CODE_FAIL);
			ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
			return 0;
		}
		pRoom = pGamePlayer->GetRoom();
		if (pRoom == NULL || pRoom->GetRoomType() != emROOM_TYPE_PRIVATE)
		{
			LOG_DEBUG("���ڷ����л��߲��ܴ�������:{}-->{}", uid, pGamePlayer->GetRoomID());
			rep.set_result(net::RESULT_CODE_FAIL);
			ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
			return 0;
		}
		hostID = pGamePlayer->GetUID();
	}
	else
	{// ���ֲ�����
		hostID = CRedisMgr::Instance().GetClubFiled(msg.club_id(), "host");
		pRoom  = CGameRoomMgr::Instance().GetPrivateRoom();
		if (hostID == 0 || pRoom == NULL)
		{
			LOG_DEBUG("���ֲ�����IDδȡ��:{}", msg.club_id());
			rep.set_result(net::RESULT_CODE_ERROR_PARAM);
			ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
			return 0;
		}
		if (msg.game_type() != pRoom->GetGameType() || msg.club_pos() > 100)
		{
			LOG_DEBUG("���ֲ�����ѡ����Ϸ����gameType:{}--clubPos:{}", msg.game_type(), msg.club_pos());
			rep.set_result(net::RESULT_CODE_ERROR_PARAM);
			ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
			return 0;
		}
		//���ֲ����λ���Ѿ������������
		CGameTable* pTable = pRoom->GetClubTable(msg.club_id(), msg.club_floor(), msg.club_pos());
		if (pTable != NULL)
		{
			LOG_DEBUG("�����ѿ���������ʧ��:{}--{}--{}", msg.club_id(), msg.club_floor(), msg.club_pos());
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
	conf.dueTime  = getSysTime() + SECONDS_IN_30_MIN;//30'��Ч��

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
		LOG_ERROR("��Ϸ���Ͳ�ƥ��:{}--{}", msg.game_type(), pRoom->GetGameType());
		rep.set_result(net::RESULT_CODE_ERROR_PARAM);
		ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
		return 0;
	}
	if (conf.createType == emPRIVATE_CREATE_SCORE)
	{
		if (!CCommonLogic::IsOpenScoreMode(msg.game_type(), msg.play_type()) || !CDataCfgMgr::Instance().IsOpenContest(msg.game_type(), msg.play_type()))
		{
			LOG_ERROR("��������ģʽ��ƥ��:{}--{}", msg.game_type(), msg.play_type());
			rep.set_result(net::RESULT_CODE_ERROR_PARAM);
			ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
			return 0;
		}
	}
	int64_t card = CDataCfgMgr::Instance().CalcPrivateCardCfg(pRoom->GetGameType(), conf.playType, msg.round_select(),
	                                                        conf.rentType, conf.dueround);
	if (card < 0)
	{
		LOG_ERROR("û���ҵ���Ϸ��������������Ϣ");
		rep.set_result(net::RESULT_CODE_ERROR_PARAM);
		ReplyMsg(&rep, net::S2C_MSG_CREATE_TABLE_REP, uid);
		return 0;
	}

	if (!pRoom->CanCreatePlayerTable(conf))
	{
		bRet = net::RESULT_CODE_CION_ERROR;
	}
	else
	{// ��������Ƿ��������۷�
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
			LOG_ERROR("��ʱ��֧��AA֧��:{}", uid);
			return 0;
		}

		conf.rentFee = card;
	}

	if (bRet == net::RESULT_CODE_SUCCESS)
	{
		conf.passwd = CRedisMgr::Instance().GetTablePasswdFromCache();

		//ͬ������
		CGameTable* pTable = pRoom->CreatePlayerTable(conf);
		if (pTable == NULL)
		{// ʧ���˿�
			rep.set_result(net::RESULT_CODE_FAIL);
			LOG_DEBUG("��������ʧ��");
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

// �뿪����
int CHandleLobbyMsg::handle_msg_leave_table_req()
{
	net::msg_leave_table_req msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	LOG_DEBUG("�����뿪����:{}", uid);
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
		return 0;
	CGameTable* pTable = pGamePlayer->GetTable();
	if (pTable == NULL)
	{
		LOG_DEBUG("����������:{}", uid);
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
		LOG_DEBUG("�����뿪����:{}", uid);
		rep.set_result(0);
	}
	pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_LEAVE_TABLE_REP);

	return 0;
}

// ��������
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
	if ((msg.table_id() != 0 || msg.passwd().length() > 1) && bRet == RESULT_CODE_SUCCESS)// ���⴦��˽�˷�����ɹ�������
		return 0;

	net::msg_enter_table_rep rep;
	rep.set_result(bRet);
	rep.set_table_id(msg.table_id());
	pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_ENTER_TABLE);
	return 0;
}

// ����׼��
int CHandleLobbyMsg::handle_msg_table_ready()
{
	net::msg_table_ready_req msg;
	PARSE_MSG(msg);
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
	{
		LOG_DEBUG("��Ҷ���ΪNULL");
		return 0;
	}
	LOG_DEBUG("����׼��:{}", pGamePlayer->GetUID());
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

// ��������
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

// ���������й�
int CHandleLobbyMsg::handle_msg_table_set_auto()
{
	net::msg_table_set_auto_req msg;
	PARSE_MSG(msg);
	LOG_DEBUG("���������й�:{}", msg.auto_type());
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

// ���ٿ�ʼ
int CHandleLobbyMsg::handle_msg_fast_join_room()
{
	net::msg_fast_join_room_req msg;
	PARSE_MSG(msg);
	LOG_DEBUG("���ټ��뷿��");
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

// ���ٻ���
int CHandleLobbyMsg::handle_msg_fast_join_table()
{
	LOG_DEBUG("���ٻ���");
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

// �鿴������Ϣ
int CHandleLobbyMsg::handle_msg_query_table_list()
{
	net::msg_query_table_list_req msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	LOG_DEBUG("�鿴������Ϣ:{}", uid);
	CGamePlayer* pGamePlayer = GetGamePlayer();
	if (pGamePlayer == NULL)
		return 0;
	CGameRoom* pRoom = pGamePlayer->GetRoom();
	if (pRoom == NULL)
	{
		LOG_DEBUG("���û�н��뷿��:{}", uid);
		return 0;
	}
	uint32_t startID = msg.startid();
	uint32_t endID   = msg.endid();
	pRoom->QueryTableListToPlayer(pGamePlayer, startID, endID);

	return 0;
}

// վ������
int CHandleLobbyMsg::handle_msg_sitdown_standup()
{
	if (CApplication::Instance().GetStatus() == emSERVER_STATE_REPAIR)
	{
		LOG_ERROR("ά��״̬,ֱ�ӵ����з�");
		return 0;
	}
	net::msg_sitdown_standup_req msg;
	PARSE_MSG(msg);
	LOG_DEBUG("վ������:{}", msg.oper_id());
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

// ɾ������
int CHandleLobbyMsg::handle_msg_delete_table()
{
	net::msg_notify_close_table msg;
	PARSE_MSG(msg);
	LOG_DEBUG("ǿ�ƽ�������:{}", msg.table_id());
	CGameRoomMgr::Instance().ForceClosePlayerTable(msg.table_id());

	return 0;
}
// ɾ������
int CHandleLobbyMsg::handle_msg_close_club_table()
{
	net::msg_close_club_table_req msg;
	PARSE_MSG(msg);
	uint32_t uid = _head->uin;
	LOG_DEBUG("��ҽ�ɢ����:{}--{}", uid, msg.passwd());
	CGameRoom* pPrivateRoom = CGameRoomMgr::Instance().GetPrivateRoom();
	if (pPrivateRoom == NULL)return 0;
	CGameTable* pTable = pPrivateRoom->GetTableByPasswd(msg.passwd());

	msg_close_club_table_rep rep;
	rep.set_club_id(msg.club_id());
	rep.set_passwd(msg.passwd());
	rep.set_result(net::RESULT_CODE_FAIL);
	if (pTable == NULL)
	{
		LOG_DEBUG("���Ӳ�����");
		ReplyMsg(&rep, S2C_MSG_CLOSE_CLUB_TABLE_REP, uid);
		return 0;
	}
	if (pTable->GetHostID() != uid)
	{
		LOG_DEBUG("���Ƿ������ܽ�ɢ");
		ReplyMsg(&rep, S2C_MSG_CLOSE_CLUB_TABLE_REP, uid);
		return 0;
	}
	pTable->ForceGameOver(true);
	rep.set_result(net::RESULT_CODE_SUCCESS);
	ReplyMsg(&rep, S2C_MSG_CLOSE_CLUB_TABLE_REP, uid);
	return 0;
}
// ��������
int CHandleLobbyMsg::handle_msg_reload_cfg()
{
	net::msg_notify_reload_cfg msg;
	PARSE_MSG(msg);
	LOG_DEBUG("���������ļ�:{}", msg.reason());
	CApplication::Instance().ConfigurationChanged();
	return 0;
}

CGamePlayer* CHandleLobbyMsg::GetGamePlayer()
{
	CGamePlayer* pPlayer = (CGamePlayer*) CPlayerMgr::Instance().GetPlayer(_head->uin);
	if (pPlayer == NULL)
	{
		//LOG_DEBUG("��Ϸ��Ҳ�����:{}",head->uin);
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









