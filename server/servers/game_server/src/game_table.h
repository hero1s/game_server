
#pragma once

#include "game_player.h"
#include "svrlib.h"
#include "msg_define.pb.h"
#include "modern/nlohmann/json_wrap.h"
#include "utility/cooling.h"

using namespace svrlib;
using namespace std;

class CGamePlayer;
class CGameRoom;

//table log
#ifndef LOG_TAB
#define LOG_TAB_DEBUG(x, ...) spdlog::get("log")->debug("[{}:{}][{}][{}] " x,__FILENAME__,__LINE__,__FUNCTION__,GetPasswd(), ##__VA_ARGS__)
#define LOG_TAB_ERROR(x, ...) spdlog::get("log")->error("[{}:{}][{}][{}] " x,__FILENAME__,__LINE__,__FUNCTION__,GetPasswd(), ##__VA_ARGS__)
#endif

enum GAME_END_TYPE
{
  GER_NORMAL = 0,        //�������
  GER_NO_PLAYER,         //û�����
  GER_DISMISS,           //��Ϸ��ɢ
  GER_USER_LEAVE,        //�û�ǿ��
  GER_NETWORK_ERROR,     //�����ж�

};

// ��λ��Ϣ
struct stSeat
{
  CGamePlayer* pPlayer;
  uint8_t  readyStatus;      // ׼��״̬
  uint8_t  autoStatus;       // �й�״̬
  uint32_t readyTime;        // ׼��ʱ��
  uint8_t  overTimes;        // ��ʱ����
  uint32_t uid;              // ��������û���Ϸ����ǰ�˳���uid����
  uint8_t  agreeStatus;      // ���״̬
  uint8_t  playStatus;       // ��Ϸ״̬
  stSeat()
  {
	  Reset();
  }
  bool IsReady()
  {
	  return pPlayer != NULL && readyStatus == 1;
  }
  void Reset()
  {
	  memset(this, 0, sizeof(stSeat));
  }
};
// �����㷨
struct stDevide
{
  uint16_t chairID;     // ��λID
  bool   isBanker;    // �Ƿ�ׯ��
  int64_t  curScore;    // ��ǰ����
  int64_t  winScore;    // ��Ӯ����
  int64_t  realWin;     // ʵ����Ӯ

  stDevide()
  {
	  Reset();
  }
  void Reset()
  {
	  memset(this, 0, sizeof(stDevide));
  }
};

// ��������
enum TABLE_TYPE
{
  emTABLE_TYPE_SYSTEM = 0, // ϵͳ����
  emTABLE_TYPE_PLAYER = 1, // ����������
};
// ���ӽ�ɢԭ��
enum TABLE_OVER_TYPE
{
  emTABLE_OVER_ROUND   = 1, // ��������
  emTABLE_OVER_DUETIME = 2, // ʱ�����
  emTABLE_OVER_VOTE    = 3, // ͶƱ��ɢ
  emTABLE_OVER_FROCE   = 4, // ǿ�ƽ�ɢ
  emTABLE_OVER_LOGIC   = 5, // �߼�����

};
// ˽�˷�������ʽ
enum PRIVATE_CREATE_TYPE
{
  emPRIVATE_CREATE_NULL  = 0, // ��ͨģʽ
  emPRIVATE_CREATE_SCORE = 1, // ����ģʽ

};

// ������Ϣ
struct stTableConf
{
  string passwd;      // ����
  uint32_t hostID;      // ����ID
  int64_t  baseScore;   // �׷�
  uint8_t  consume;     // ��������
  int64_t  enterMin;    // ��ͽ���
  int64_t  enterMax;    // ������
  uint8_t  feeType;     // ̨������
  int64_t  feeValue;    // ̨��ֵ
  uint32_t dueTime;     // ����ʱ��
  uint16_t seatNum;     // ��λ��
  uint16_t dueround;    // ��������
  uint8_t  playType;    // �淨
  uint8_t  createType;  // ������ʽ
  uint8_t  rentType;    // ����֧����ʽ
  uint32_t rentFee;     // ֧������
  string addParam;    // ���Ӳ���
  string matchParam;  // ��������
  int64_t  clubID;      // ���ֲ�ID
  int64_t  clubFloor;   // ���ֲ�¥��
  uint16_t clubPos;     // ���ֲ�����λ��
  uint8_t  idxRound;    // ����ѡ��

  stTableConf()
  {
	  hostID     = 0;
	  passwd     = "";
	  hostID     = 0;
	  baseScore  = 0;
	  consume    = 0;
	  enterMin   = 0;
	  enterMax   = 0;
	  feeType    = 0;
	  feeValue   = 0;
	  dueTime    = 0;
	  seatNum    = 0;  // ��λ��
	  dueround   = 0;
	  playType   = 0;
	  createType = 0;
	  addParam   = "";
	  matchParam = "";
	  rentType   = 0;
	  rentFee    = 0;
	  clubID     = 0;  // ���ֲ�ID
	  clubFloor  = 0;  // ���ֲ�¥��
	  clubPos    = 0;  // ���ֲ�����λ��
	  idxRound   = 0;
  }
};

// ��Ϸ����
class CGameTable
{
public:
	CGameTable(CGameRoom* pRoom, int64_t tableID, uint8_t tableType);
	virtual ~CGameTable();
	virtual void OnProccess();
	int64_t GetTableID();
	uint8_t GetTableType();
	CGameRoom* GetHostRoom();

	// ������»���վ��
	virtual bool PlayerSitDownStandUp(CGamePlayer* pPlayer, bool sitDown, uint16_t chairID);

	bool AddLooker(CGamePlayer* pPlayer);
	bool RemoveLooker(CGamePlayer* pPlayer);
	// �Ƿ��Թ�
	bool IsExistLooker(uint32_t uid);
	// ����׼��
	virtual bool ResetPlayerReady();
	// �Զ�׼��
	void AutoReadyAll();
	bool IsAllReady();
	// ��Ϸ״̬
	void SetPlayStatus(uint16_t chairID, uint8_t status);
	uint8_t GetPlayStatus(uint16_t chairID);
	void ResetPlayStatus(uint8_t status = 0);

	// ��������Զ�׼��
	bool PlayerSetAuto(CGamePlayer* pPlayer, uint8_t autoType);
	bool IsReady(CGamePlayer* pPlayer);
	bool IsExistPlayer(uint32_t uid);
	uint32_t GetPlayerNum();
	uint32_t GetOnlinePlayerNum();
	uint32_t GetReadyNum();
	uint32_t GetNoReadyNum();
	virtual bool IsFullTable();
	virtual bool IsEmptyTable();

	virtual bool IsAllDisconnect(uint32_t disconnectTime);
	virtual bool IsDisconnect(uint16_t chairID);
	// �Ƿ�������
	virtual bool IsOpenCfgCard();
	void SetGameState(uint8_t state);
	uint8_t GetGameState();

	CGamePlayer* GetPlayer(uint16_t chairID);
	uint32_t GetPlayerID(uint16_t chairID);
	uint16_t GetChairID(CGamePlayer* pPlayer);
	uint8_t GetConsumeType();
	int64_t GetBaseScore();
	int64_t GetEnterMin();
	int64_t GetEnterMax();
	uint8_t GetPlayType();
	uint32_t GetHostID();
	bool IsRightPasswd(string passwd);
	string GetPasswd();
	// ���ھ��ֲ�
	int64_t GetClubID();
	int64_t GetClubFloor();
	// ���ھ��ֲ�λ��
	uint32_t GetClubPos();

	void SetTableConf(stTableConf& conf);
	stTableConf& GetTableConf();

	// ��ҵ�ǰ����
	virtual int64_t GetPlayerCurScore(CGamePlayer* pPlayer);
	virtual int64_t GetPlayerGameScore(uint32_t uid);
	// �Ƿ��Ϊ��
	virtual bool CanMinus();

	// �����㷨
	virtual stDevide GetDevide(uint16_t chairID, int64_t winScore, bool isBanker);
	// ��Ȩ�ط���
	virtual void DevideByWeight(vector<stDevide>& players, bool isHaveBanker);
	// ��˳�����
	virtual void DevideByOrder(vector<stDevide>& players, bool isHaveBanker);

public:
	virtual bool EnterTable(CGamePlayer* pPlayer) = 0;
	virtual bool LeaveTable(CGamePlayer* pPlayer, bool bNotify = false, uint8_t leaveType = 0) = 0;
	// �ܷ����
	virtual int32_t CanEnterTable(CGamePlayer* pPlayer) = 0;
	// �ܷ��뿪
	virtual bool CanLeaveTable(CGamePlayer* pPlayer) = 0;
	// �Ƿ������Ϸ
	virtual bool IsJoinPlay(uint16_t chairID) = 0;
	// �ܷ�����
	virtual bool CanSitDown(CGamePlayer* pPlayer, uint16_t chairID) = 0;
	// �ܷ�վ��
	virtual bool CanStandUp(CGamePlayer* pPlayer) = 0;
	// ��Ҫ����
	virtual bool NeedSitDown() = 0;
	// ���׼��
	virtual bool PlayerReady(CGamePlayer* pPlayer, bool bReady) = 0;
	// �Ƿ���Ҫ����
	virtual bool NeedRecover()=0;
	// ��������
	virtual bool WantNeedRecover()=0;
	// ���������Ϣ
	virtual void GetTableFaceInfo(net::table_face_info* pInfo)=0;
public:
	//��������
	virtual bool Init()         = 0;
	virtual void ShutDown()     = 0;
	// ǿ�ƽ���
	virtual void ForceGameOver(bool bClient) = 0;
	//��λ����
	virtual void ResetTable()   = 0;
	virtual void OnTimeTick()   = 0;
	//��Ϸ��Ϣ
	virtual int OnMessage(CGamePlayer* pPlayer, uint16_t cmdID, const uint8_t* pkt_buf, uint16_t buf_len) = 0;
	virtual int OnGameMessage(CGamePlayer* pPlayer, uint16_t cmdID, const uint8_t* pkt_buf, uint16_t buf_len) = 0;
	//�û����߻�����
	virtual bool OnActionUserNetState(CGamePlayer* pPlayer, bool bConnected, bool isJoin = true) = 0;
	//�û�����
	virtual bool OnActionUserSitDown(uint16_t wChairID, CGamePlayer* pPlayer) = 0;
	//�û�����
	virtual bool OnActionUserStandUp(uint16_t wChairID, CGamePlayer* pPlayer) = 0;
	//�û�ͬ��
	virtual bool OnActionUserOnReady(uint16_t wChairID, CGamePlayer* pPlayer) = 0;
	//��ҽ�����뿪
	virtual void OnPlayerJoin(bool isJoin, uint16_t chairID, CGamePlayer* pPlayer)=0;
	// ���ͳ�����Ϣ(��������)
	virtual void SendGameScene(CGamePlayer* pPlayer) = 0;

// ��Ϣ����
public:
	virtual void TableMsgToLooker(const google::protobuf::Message* msg, uint16_t msg_type);
	virtual void TableMsgToPlayer(const google::protobuf::Message* msg, uint16_t msg_type, bool bRecord = true);
	virtual void TableMsgToAll(const google::protobuf::Message* msg, uint16_t msg_type, bool bRecord = true);
	virtual void TableMsgToClient(uint16_t chairID, const google::protobuf::Message* msg, uint16_t msg_type, bool bRecord = true);
	// ����������Ϣ
	void SendTableInfoToClient(CGamePlayer* pPlayer);
	// ����׼��״̬
	void SendReadyStateToClient();
	// ������λ��Ϣ
	void SendSeatInfoToClient(CGamePlayer* pGamePlayer = NULL);
	// ˢ����λ��ֵƮ��
	void FlushSeatValueInfoToClient(bool bShowChange = false);
	// �����Թ��б�
	void SendLookerListToClient(CGamePlayer* pGamePlayer = NULL);
	// ֪ͨ��Ҽ���
	void NotifyPlayerJoin(CGamePlayer* pPlayer, bool isJoin);
	// ֪ͨ�뿪
	void NotifyPlayerLeave(CGamePlayer* pPlayer, uint8_t leaveType = 0);

protected:
	void ResetInitSeat(uint8_t seatNum);
	void SitDown(stSeat& seat, CGamePlayer* pPlayer);

protected:
	void GetSeatInfo(net::msg_seat_info_rep& msg);
	void GetReadyInfo(net::msg_table_ready_rep& msg);
	// ������ӻ�����Ϣ
	void GetTableFaceBaseInfo(net::table_info* pBaseInfo);

protected:
	// �ƾ���־
	void InitBlingLog(bool bNeedReady = false);
	void ChangeUserBlingLog(CGamePlayer* pPlayer, int64_t winScore);
	void ChangeUserBlingLogFee(uint32_t uid, int64_t fee);
	void SaveBlingLog();
	// ������־
	void WriteBankerLog(uint16_t chairID);
	// ����
	void WriteHandleCardLog(uint16_t chairID, uint8_t cardData[], uint8_t cardCount, uint8_t cardType, int64_t score);
	// ����
	void WriteOutCardLog(uint16_t chairID, uint8_t cardData[], uint8_t cardCount, uint8_t eFlag);
	// �ƾ�¼��
	void InitRecordGameMsg();
	void PushRecordGameMsg(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uid = 0);
	void SaveRecordGameMsg();

protected:
	CGameRoom* m_pHostRoom;                                                   // ��������
	MemberTimerEvent<CGameTable, &CGameTable::OnProccess> m_timer;            // ��ʱ��
	stTableConf                                           m_conf;             // ��������
	vector<stSeat>                                        m_vecPlayers;       // ���
	map<uint32_t, CGamePlayer*>                             m_mpLookers;        // Χ����
	uint8_t                                                 m_gameState;        // ��Ϸ״̬
	int64_t                                                 m_tableID;          // ����ID
	CCooling                                                m_coolLogic;        // �߼�CD
	uint8_t                                                 m_tableType;        // ��������(��̬���ӣ�˽������);
	//��־
	stGameBlingLog                                        m_blingLog;         // �ƾ���־
	net::game_record                                      m_gameRecord;       // ��Ϸ¼��
	json                                                  m_operLog;          // ������־
	bool                                                  m_openRecord;       // ����¼��
	bool                                                  m_needOpenRecord;   // ��Ҫ����¼��
	string                                                m_chessid;          // �ƾ�ID
	uint32_t                                              m_round;            // ��ǰ����


};




