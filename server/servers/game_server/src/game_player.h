

#pragma once

#include "svrlib.h"
#include "player_base.h"
#include "game_define.h"
#include "msg_define.pb.h"
#include "game_table.h"

using namespace svrlib;
using namespace std;
using namespace net;
using namespace Network;

class CGameRoom;

class CGameTable;

class CGamePlayer : public CPlayerBase
{
public:
	CGamePlayer(uint8_t type);

	virtual ~CGamePlayer();

	virtual bool SendMsgToClient(const google::protobuf::Message* msg, uint16_t msg_type);

	virtual void OnLoginOut();

	virtual void OnLogin();

	virtual void ReLogin();

	// ֪ͨ���ش���(�˳���Ϸ)
	void NotifyLeaveGameSvr();

	// ��������޸���ֵ
	void
	NotifyLobbyChangeAccValue(int32_t operType, int32_t subType, int64_t coin, int64_t safeCoin, const string& chessid = "");
	// ˢ�������Ϸ״̬������
	void NotifyLobbyChangePlayStatus();

	// �޸�����˺���ֵ�������޸ģ�
	virtual void
	SyncChangeAccountValue(uint16_t operType, uint16_t subType, int64_t coin, int64_t safecoin, const string& chessid = "");
	// �ܷ��˳�
	bool CanBackLobby();
	// ��������ʱ��
	void ResetHeart();
	// ����ʱ��
	uint32_t GetDisconnectTime();

	virtual void OnTimeTick(uint64_t uTime, bool bNewDay);

	// �Ƿ���Ҫ����
	virtual bool NeedRecover();

	// �Ƿ�������Ϸ��
	bool IsInGamePlaying();

	// ������ֵ������
	void FlushAccValue2Table();

	// �뿪��ǰ����
	CGameTable* TryLeaveCurTable();
	// ������ֲ���������
	void ClearClubScore();

public:
	uint8_t GetNetState();
	void SetNetState(uint8_t state);
	void SetNetStateNoPlayer();
	CGameRoom* GetRoom();
	void SetRoom(CGameRoom* pRoom);
	uint16_t GetRoomID();
	CGameTable* GetTable();
	void SetTable(CGameTable* pTable);
	int64_t GetTableID();
	void SetAutoReady(bool bAuto);
	bool IsAutoReady();
	void SetPlayDisconnect(bool bFlag);
	bool IsPlayDisconnect();
	// ���ֲ�����
	void SetClubScore(int64_t score);
	int64_t GetClubScore();

protected:
	uint8_t m_netState;                   // ����״̬
	CGameRoom * m_pGameRoom;            // ���ڷ���
	CGameTable* m_pGameTable;           // ��������
	uint32_t m_msgHeartTime;              // ��Ϣ����ʱ��
	bool   m_autoReady;                 // �Զ�׼��
	bool   m_playDisconnect;            // ��Ϸ�Ƿ������
	int64_t  m_clubScore;                 // ���ֲ�����

};





