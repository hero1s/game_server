
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

	// ��Ϸ����Ϣ
	virtual int handle_msg_gameing_oper();

protected:
	// ֪ͨ����״̬
	int handle_msg_notify_net_state();

	// ������Ϸ������
	int handle_msg_enter_svr();

	// ���ش���
	int handle_msg_back_lobby();

	std::shared_ptr<CGamePlayer> GetGamePlayer();

	void ReplyMsg(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin);

private:



};



















