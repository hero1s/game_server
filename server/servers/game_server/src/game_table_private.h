//
// Created by toney on 2018/6/11.
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

// ����������
enum emTAX_TYPE
{
  emTAX_TYPE_TABLE = 0, // �ֳܾ�ˮ
  emTAX_TYPE_ROUND = 1, // С�ֳ�ˮ

};

struct stScoreModeSetting
{
  //���ֳ���Ϣ
  bool   canMinus;                        // �Ƿ����Ϊ��
  uint8_t  taxType;                       // ��ˮ��ʽ
  uint32_t tax;                           // ��ˮ����
  uint8_t  taxWinNum;                     // ��ˮ����
  uint32_t taxBaseWin;                    // ��ˮ����(���ڶ��ٷֲų�)
  int64_t  minBankerScore;                // ��С��ׯ����
  uint8_t  scoreExchange;                 // ���ֶһ�����

  stScoreModeSetting()
  {
	  Reset();
  }
  void Reset()
  {
	  memset(this, 0, sizeof(stScoreModeSetting));
  }
};

// ��Ϸ�������
class CGamePrivateTable : public CGameTable
{
public:
	CGamePrivateTable(CGameRoom* pRoom, int64_t tableID);
	virtual ~CGamePrivateTable();

public://���غ���
	virtual bool Init();
	virtual void ShutDown();
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
public:
	// ˽�˷�����
	virtual void LoadPrivateTable(stPrivateTable& data);
	// ����˽�˷�
	virtual bool CreatePrivateTable();
	// ������������ļ�
	virtual bool CheckPrivateTableConfRight();

public:
	// ͶƱ��ɢ
	virtual void VoteGameOver(uint32_t reason, uint32_t opid, uint32_t reqID, uint32_t agree);
	virtual bool AllAgreeVote();
	virtual void ResetVoteInfo();
	virtual bool IsVoteStatus();
	// ֪ͨ��Ϸ��ǰ����
	virtual void NotifyGameOver(uint32_t reason);
	// ֪ͨͶƱ��Ϣ
	virtual void NotifyVoteGameOver();
	// ����ͶƱ��Ϣ
	virtual void ResetVoteGameOverState();
	// ǿ�ƽ���
	virtual void ForceGameOver(bool bNormal);
	// �߼�����
	virtual void LogicGameOver();

	// ���������Ϣ
	virtual void AddPrivatePlayerInfo(uint32_t uid, bool bAddLog);
	// ɾ�������Ϣ
	virtual void DelPrivatePlayerInfo(uint32_t uid);
	// �Զ�ɾ��û����λ�����
	virtual void AutoDelNoSeatPrivatePlayerInfo();
	virtual bool IsExistPrivatePlayerInfo(uint32_t uid);



	// ˽�˽��㸽����Ϣ
	virtual void SetPrivatePlayerInfoParam(uint32_t uid, string key, int32_t value);
	virtual int32_t GetPrivatePlayerInfoParam(uint32_t uid, string key);
	virtual void ChangePrivatePlayerInfoParam(uint32_t uid, string key, int32_t value, bool bReplaceMax = false);
	virtual int64_t GetPlayerGameScore(uint32_t uid);
	virtual void SetPlayerBuyIn(uint32_t uid,int64_t score);

	// �Ƿ��Ϊ��
	virtual bool CanMinus();
	virtual int64_t ChangeScoreByUID(uint32_t uid, int64_t score);
	int64_t SetScoreByUID(uint32_t uid, int64_t score); //���÷���
	// ���������Ϣ
	virtual void CalcPlayerGameInfo(uint32_t uid, int64_t winScore);
	// ����ǰ����Ӯ
	virtual void ClearRoundWinScore();
	// ����Զ��뿪
	virtual void CheckAutoLeaveTable();
	// ������ģʽ�Ƿ���Ҫ��ǰ����
	virtual bool CheckScoreModeGameOver();
public:
	// һ�ֿ�ʼ
	virtual void OnGameRoundStart();
	// һ�ֽ���
	virtual void OnGameRoundOver();
	virtual void OnGameRoundFlush();
	// ��Ϸ����
	virtual void OnGameOver(uint8_t overType);
	// �Ƿ����
	virtual uint8_t NeedGameOver();
	// ���˽�˷��Ƿ����
	virtual void CheckPrivateGameOver();
	// �Ƿ�ʼ��Ϸ
	virtual bool IsStartGame();

	// ������Ϸ��Ϣ
	virtual void SavePrivateTableGameInfo();
	virtual void FlushPrivateTableInfoToCache();
	// ��Ϸ��Ϣ
	virtual void SendPrivateGameInfo();
	// ˢ��˽�˷���Ϣ��������ֲ�
	virtual void FlushClubPrivateListToClient(bool bDelete = false);
	// ��¼���ֲ������Ϣ
	virtual void LogClubGameInfo();
	//AA֧��
	virtual bool PayAARoomCard(CGamePlayer* pPlayer);
	//AA�˿�
	virtual void BackPayAARoomCard(CGamePlayer* pPlayer);
	//֧������
	void PayRoomCard();
	//�˻ط���
	void BackRoomCard();
public:
	//�Ƿ���ֳ�
	bool IsScoreMode();
	//��ʼ�����ֳ�����
	void ResetClubScore(CGamePlayer* pPlayer);
	//��ˮ
	void CalcRoundTaxScore();
	void CalcTableTaxScore();
	//ˢ�»�����Ϣ�����ֲ�
	void CalcClubScore();
	//�������Ƿ��ܼ���
	void CheckPlayerCanPlay();
	//�ܷ���ׯ
	bool CanBankerScore(uint32_t uid);
	//���û��ֳ�����
	bool SetMatchParam();

protected:
	//ͶƱ
	CCooling m_agreeCooling;     // ͶƱ���CD
	uint32_t   m_agreeID;          // ������
	uint8_t    m_voteReason;       // ����ԭ��
	bool     m_voteEnd;          // ����Ƿ����
	bool     m_voteDiss;         // ͶƱ��ɢ
	bool     m_forceDiss;        // ǿ�ƽ�ɢ
	bool     m_logicDiss;        // �߼�����

	//������Ϣ
	map<uint32_t, stCalcScoreInfo> m_mpPlayerScoreInfo;             // �Ʒ������Ϣ

	bool   m_bClosed;                       // �Ƿ�ر�
	uint32_t m_startTime;                     // ������Ϸ��ʼ��Ϸʱ��
	uint32_t m_endTime;                       // ����ʱ��
	uint32_t m_createTime;                    // ����ʱ��
	bool   m_canJoinPlaying;                // �Ƿ��ֹ��;����

	//���ֳ���Ϣ
	stScoreModeSetting m_scoreSetting;      // ��������

};





