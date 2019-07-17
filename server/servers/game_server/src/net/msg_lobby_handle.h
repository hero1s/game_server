
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

	// ��Ϸ����Ϣ
	virtual int handle_msg_gameing_oper();

protected:
	// ע�����������
	int handle_msg_register_rep();

	// ֪ͨ����״̬
	int handle_msg_notify_net_state();

	// ֪ͨ������ά��
	int handle_msg_notify_stop_server();

	int handle_msg_flush_change_acc_data();

	// ������Ϸ������
	int handle_msg_enter_svr();

	// ���󷿼��б���Ϣ
	int handle_msg_req_rooms_info();

	// ���ش���
	int handle_msg_back_lobby();

	// ���뷿��
	int handle_msg_enter_room();

	// ���������б�
	int handle_msg_req_table_list();

	// ��������
	int handle_msg_req_create_table();

	// �뿪����
	int handle_msg_leave_table_req();

	// ��������
	int handle_msg_enter_table();

	// ����׼��
	int handle_msg_table_ready();

	// ��������
	int handle_msg_table_chat();

	// ���������й�
	int handle_msg_table_set_auto();

	// ���ٿ�ʼ
	int handle_msg_fast_join_room();

	// ���ٻ���
	int handle_msg_fast_join_table();

	// �鿴������Ϣ
	int handle_msg_query_table_list();

	// վ������
	int handle_msg_sitdown_standup();

	// ɾ������
	int handle_msg_delete_table();
	// ɾ������
	int handle_msg_close_club_table();

	// ��������
	int handle_msg_reload_cfg();

	CGamePlayer* GetGamePlayer();

	void ReplyMsg(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin);

private:



};



















