//
// Created by toney on 2018/6/13.
//

#pragma once

#include "game_table.h"
#include "game_player.h"
#include "svrlib.h"
#include "msg_define.pb.h"
#include "modern/nlohmann/json_wrap.h"

using namespace svrlib;
using namespace std;

class CGamePlayer;
class CGameRoom;

// ��Ϸ��ҳ�����
class CGameCoinTable : public CGameTable
{
public:
	CGameCoinTable(CGameRoom* pRoom, int64_t tableID);
	virtual ~CGameCoinTable();

public://���غ���
	virtual void OnProccess();
	//��Ϸ��Ϣ
	virtual int OnMessage(CGamePlayer* pPlayer, uint16_t cmdID, const uint8_t* pkt_buf, uint16_t buf_len);

	virtual bool EnterTable(CGamePlayer* pPlayer);
	virtual bool LeaveTable(CGamePlayer* pPlayer, bool bNotify = false, uint8_t leaveType = 0);
	// �ܷ����
	virtual int32_t CanEnterTable(CGamePlayer* pPlayer);
	// �ܷ��뿪
	virtual bool CanLeaveTable(CGamePlayer* pPlayer);
	// �Ƿ������Ϸ
	virtual bool IsJoinPlay(uint16_t chairID);
	// �ܷ�����
	virtual bool CanSitDown(CGamePlayer* pPlayer, uint16_t chairID);
	// �ܷ�վ��
	virtual bool CanStandUp(CGamePlayer* pPlayer);
	// ��Ҫ����
	virtual bool NeedSitDown();
	// ���׼��
	virtual bool PlayerReady(CGamePlayer* pPlayer, bool bReady);

	//�û����߻�����
	virtual bool OnActionUserNetState(CGamePlayer* pPlayer, bool bConnected, bool isJoin = true);
	//�û�����
	virtual bool OnActionUserSitDown(uint16_t wChairID, CGamePlayer* pPlayer);
	//�û�����
	virtual bool OnActionUserStandUp(uint16_t wChairID, CGamePlayer* pPlayer);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(uint16_t wChairID, CGamePlayer* pPlayer);
	//��ҽ�����뿪
	virtual void OnPlayerJoin(bool isJoin, uint16_t chairID, CGamePlayer* pPlayer);

	// �Ƿ���Ҫ����
	virtual bool NeedRecover();
	// ��������
	virtual bool WantNeedRecover();
protected:
	// û���ֵ�����Զ�վ��
	virtual void StandUpNotScore();
	// �۳���ʼ̨��
	virtual void DeductStartFee(bool bNeedReady);
	// �۳�����̨��
	virtual void DeducEndFee(uint32_t uid, int64_t& winScore);
	// �ϱ���Ϸս��
	virtual void ReportGameResult(uint32_t uid, int64_t winScore);
	// ���������Ϣ
	virtual void CalcPlayerGameInfo(uint32_t uid, int64_t winScore);
	virtual int64_t ChangeScoreValueByUID(uint32_t uid, int64_t& score, uint16_t operType, uint16_t subType);

};



