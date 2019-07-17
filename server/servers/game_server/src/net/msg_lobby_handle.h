
#pragma once

#include "packet/inner_protobuf_pkg.h"

using namespace Network;

class CGamePlayer;

class CHandleLobbyMsg : public CInnerMsgHanlde, public AutoDeleteSingleton<CHandleLobbyMsg>
{
public:
	CHandleLobbyMsg();
	virtual ~CHandleLobbyMsg();

public:
	virtual int OnRecvClientMsg();

	// 游戏内消息
	virtual int handle_msg_gameing_oper();

protected:
	// 注册服务器返回
	int handle_msg_register_rep();

	// 通知网络状态
	int handle_msg_notify_net_state();

	// 通知服务器维护
	int handle_msg_notify_stop_server();

	int handle_msg_flush_change_acc_data();

	// 进入游戏服务器
	int handle_msg_enter_svr();

	// 请求房间列表信息
	int handle_msg_req_rooms_info();

	// 返回大厅
	int handle_msg_back_lobby();

	// 进入房间
	int handle_msg_enter_room();

	// 请求桌子列表
	int handle_msg_req_table_list();

	// 创建桌子
	int handle_msg_req_create_table();

	// 离开桌子
	int handle_msg_leave_table_req();

	// 进入桌子
	int handle_msg_enter_table();

	// 桌子准备
	int handle_msg_table_ready();

	// 桌子聊天
	int handle_msg_table_chat();

	// 桌子设置托管
	int handle_msg_table_set_auto();

	// 快速开始
	int handle_msg_fast_join_room();

	// 快速换桌
	int handle_msg_fast_join_table();

	// 查看桌子信息
	int handle_msg_query_table_list();

	// 站立做起
	int handle_msg_sitdown_standup();

	// 删除房间
	int handle_msg_delete_table();
	// 删除房间
	int handle_msg_close_club_table();

	// 重载配置
	int handle_msg_reload_cfg();

	CGamePlayer* GetGamePlayer();

	void ReplyMsg(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin);

private:



};



















