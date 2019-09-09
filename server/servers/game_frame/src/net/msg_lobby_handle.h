
#pragma once

#include "packet/inner_protobuf_pkg.h"

using namespace Network;

class CGamePlayer;

class CHandleLobbyMsg : public CInnerMsgHanlde
{
public:
	CHandleLobbyMsg();
	virtual ~CHandleLobbyMsg();

public:
	virtual int OnRecvClientMsg();

	// 游戏内消息
	virtual int handle_msg_gameing_oper();

protected:
	// 通知网络状态
	int handle_msg_notify_net_state();

	// 进入游戏服务器
	int handle_msg_enter_svr();

	// 返回大厅
	int handle_msg_back_lobby();

	std::shared_ptr<CGamePlayer> GetGamePlayer();

	void ReplyMsg(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin);

private:



};



















