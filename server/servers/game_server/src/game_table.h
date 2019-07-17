
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
  GER_NORMAL = 0,        //常规结束
  GER_NO_PLAYER,         //没有玩家
  GER_DISMISS,           //游戏解散
  GER_USER_LEAVE,        //用户强退
  GER_NETWORK_ERROR,     //网络中断

};

// 座位信息
struct stSeat
{
  CGamePlayer* pPlayer;
  uint8_t  readyStatus;      // 准备状态
  uint8_t  autoStatus;       // 托管状态
  uint32_t readyTime;        // 准备时间
  uint8_t  overTimes;        // 超时次数
  uint32_t uid;              // 德州梭哈用户游戏结束前退出用uid处理
  uint8_t  agreeStatus;      // 表决状态
  uint8_t  playStatus;       // 游戏状态
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
// 分账算法
struct stDevide
{
  uint16_t chairID;     // 座位ID
  bool   isBanker;    // 是否庄家
  int64_t  curScore;    // 当前分数
  int64_t  winScore;    // 输赢分数
  int64_t  realWin;     // 实际输赢

  stDevide()
  {
	  Reset();
  }
  void Reset()
  {
	  memset(this, 0, sizeof(stDevide));
  }
};

// 桌子类型
enum TABLE_TYPE
{
  emTABLE_TYPE_SYSTEM = 0, // 系统桌子
  emTABLE_TYPE_PLAYER = 1, // 玩家组局桌子
};
// 桌子解散原因
enum TABLE_OVER_TYPE
{
  emTABLE_OVER_ROUND   = 1, // 局数打完
  emTABLE_OVER_DUETIME = 2, // 时间过期
  emTABLE_OVER_VOTE    = 3, // 投票解散
  emTABLE_OVER_FROCE   = 4, // 强制解散
  emTABLE_OVER_LOGIC   = 5, // 逻辑结束

};
// 私人房创建方式
enum PRIVATE_CREATE_TYPE
{
  emPRIVATE_CREATE_NULL  = 0, // 普通模式
  emPRIVATE_CREATE_SCORE = 1, // 积分模式

};

// 桌子信息
struct stTableConf
{
  string passwd;      // 密码
  uint32_t hostID;      // 房主ID
  int64_t  baseScore;   // 底分
  uint8_t  consume;     // 消费类型
  int64_t  enterMin;    // 最低进入
  int64_t  enterMax;    // 最大带入
  uint8_t  feeType;     // 台费类型
  int64_t  feeValue;    // 台费值
  uint32_t dueTime;     // 到期时间
  uint16_t seatNum;     // 座位数
  uint16_t dueround;    // 结束局数
  uint8_t  playType;    // 玩法
  uint8_t  createType;  // 创建方式
  uint8_t  rentType;    // 房费支付方式
  uint32_t rentFee;     // 支付费用
  string addParam;    // 附加参数
  string matchParam;  // 比赛参数
  int64_t  clubID;      // 俱乐部ID
  int64_t  clubFloor;   // 俱乐部楼层
  uint16_t clubPos;     // 俱乐部桌子位置
  uint8_t  idxRound;    // 局数选择

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
	  seatNum    = 0;  // 座位数
	  dueround   = 0;
	  playType   = 0;
	  createType = 0;
	  addParam   = "";
	  matchParam = "";
	  rentType   = 0;
	  rentFee    = 0;
	  clubID     = 0;  // 俱乐部ID
	  clubFloor  = 0;  // 俱乐部楼层
	  clubPos    = 0;  // 俱乐部桌子位置
	  idxRound   = 0;
  }
};

// 游戏桌子
class CGameTable
{
public:
	CGameTable(CGameRoom* pRoom, int64_t tableID, uint8_t tableType);
	virtual ~CGameTable();
	virtual void OnProccess();
	int64_t GetTableID();
	uint8_t GetTableType();
	CGameRoom* GetHostRoom();

	// 玩家坐下或者站起
	virtual bool PlayerSitDownStandUp(CGamePlayer* pPlayer, bool sitDown, uint16_t chairID);

	bool AddLooker(CGamePlayer* pPlayer);
	bool RemoveLooker(CGamePlayer* pPlayer);
	// 是否旁观
	bool IsExistLooker(uint32_t uid);
	// 重置准备
	virtual bool ResetPlayerReady();
	// 自动准备
	void AutoReadyAll();
	bool IsAllReady();
	// 游戏状态
	void SetPlayStatus(uint16_t chairID, uint8_t status);
	uint8_t GetPlayStatus(uint16_t chairID);
	void ResetPlayStatus(uint8_t status = 0);

	// 设置玩家自动准备
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
	// 是否开启配牌
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
	// 所在俱乐部
	int64_t GetClubID();
	int64_t GetClubFloor();
	// 所在俱乐部位置
	uint32_t GetClubPos();

	void SetTableConf(stTableConf& conf);
	stTableConf& GetTableConf();

	// 玩家当前分数
	virtual int64_t GetPlayerCurScore(CGamePlayer* pPlayer);
	virtual int64_t GetPlayerGameScore(uint32_t uid);
	// 是否可为负
	virtual bool CanMinus();

	// 分赃算法
	virtual stDevide GetDevide(uint16_t chairID, int64_t winScore, bool isBanker);
	// 按权重分配
	virtual void DevideByWeight(vector<stDevide>& players, bool isHaveBanker);
	// 按顺序分配
	virtual void DevideByOrder(vector<stDevide>& players, bool isHaveBanker);

public:
	virtual bool EnterTable(CGamePlayer* pPlayer) = 0;
	virtual bool LeaveTable(CGamePlayer* pPlayer, bool bNotify = false, uint8_t leaveType = 0) = 0;
	// 能否进入
	virtual int32_t CanEnterTable(CGamePlayer* pPlayer) = 0;
	// 能否离开
	virtual bool CanLeaveTable(CGamePlayer* pPlayer) = 0;
	// 是否参与游戏
	virtual bool IsJoinPlay(uint16_t chairID) = 0;
	// 能否坐下
	virtual bool CanSitDown(CGamePlayer* pPlayer, uint16_t chairID) = 0;
	// 能否站起
	virtual bool CanStandUp(CGamePlayer* pPlayer) = 0;
	// 需要坐下
	virtual bool NeedSitDown() = 0;
	// 玩家准备
	virtual bool PlayerReady(CGamePlayer* pPlayer, bool bReady) = 0;
	// 是否需要回收
	virtual bool NeedRecover()=0;
	// 即将回收
	virtual bool WantNeedRecover()=0;
	// 获得桌子信息
	virtual void GetTableFaceInfo(net::table_face_info* pInfo)=0;
public:
	//配置桌子
	virtual bool Init()         = 0;
	virtual void ShutDown()     = 0;
	// 强制结束
	virtual void ForceGameOver(bool bClient) = 0;
	//复位桌子
	virtual void ResetTable()   = 0;
	virtual void OnTimeTick()   = 0;
	//游戏消息
	virtual int OnMessage(CGamePlayer* pPlayer, uint16_t cmdID, const uint8_t* pkt_buf, uint16_t buf_len) = 0;
	virtual int OnGameMessage(CGamePlayer* pPlayer, uint16_t cmdID, const uint8_t* pkt_buf, uint16_t buf_len) = 0;
	//用户断线或重连
	virtual bool OnActionUserNetState(CGamePlayer* pPlayer, bool bConnected, bool isJoin = true) = 0;
	//用户坐下
	virtual bool OnActionUserSitDown(uint16_t wChairID, CGamePlayer* pPlayer) = 0;
	//用户起立
	virtual bool OnActionUserStandUp(uint16_t wChairID, CGamePlayer* pPlayer) = 0;
	//用户同意
	virtual bool OnActionUserOnReady(uint16_t wChairID, CGamePlayer* pPlayer) = 0;
	//玩家进入或离开
	virtual void OnPlayerJoin(bool isJoin, uint16_t chairID, CGamePlayer* pPlayer)=0;
	// 发送场景信息(断线重连)
	virtual void SendGameScene(CGamePlayer* pPlayer) = 0;

// 消息发送
public:
	virtual void TableMsgToLooker(const google::protobuf::Message* msg, uint16_t msg_type);
	virtual void TableMsgToPlayer(const google::protobuf::Message* msg, uint16_t msg_type, bool bRecord = true);
	virtual void TableMsgToAll(const google::protobuf::Message* msg, uint16_t msg_type, bool bRecord = true);
	virtual void TableMsgToClient(uint16_t chairID, const google::protobuf::Message* msg, uint16_t msg_type, bool bRecord = true);
	// 发送桌子信息
	void SendTableInfoToClient(CGamePlayer* pPlayer);
	// 发送准备状态
	void SendReadyStateToClient();
	// 发送座位信息
	void SendSeatInfoToClient(CGamePlayer* pGamePlayer = NULL);
	// 刷新座位数值飘分
	void FlushSeatValueInfoToClient(bool bShowChange = false);
	// 发送旁观列表
	void SendLookerListToClient(CGamePlayer* pGamePlayer = NULL);
	// 通知玩家加入
	void NotifyPlayerJoin(CGamePlayer* pPlayer, bool isJoin);
	// 通知离开
	void NotifyPlayerLeave(CGamePlayer* pPlayer, uint8_t leaveType = 0);

protected:
	void ResetInitSeat(uint8_t seatNum);
	void SitDown(stSeat& seat, CGamePlayer* pPlayer);

protected:
	void GetSeatInfo(net::msg_seat_info_rep& msg);
	void GetReadyInfo(net::msg_table_ready_rep& msg);
	// 获得桌子基本信息
	void GetTableFaceBaseInfo(net::table_info* pBaseInfo);

protected:
	// 牌局日志
	void InitBlingLog(bool bNeedReady = false);
	void ChangeUserBlingLog(CGamePlayer* pPlayer, int64_t winScore);
	void ChangeUserBlingLogFee(uint32_t uid, int64_t fee);
	void SaveBlingLog();
	// 操作日志
	void WriteBankerLog(uint16_t chairID);
	// 手牌
	void WriteHandleCardLog(uint16_t chairID, uint8_t cardData[], uint8_t cardCount, uint8_t cardType, int64_t score);
	// 出牌
	void WriteOutCardLog(uint16_t chairID, uint8_t cardData[], uint8_t cardCount, uint8_t eFlag);
	// 牌局录像
	void InitRecordGameMsg();
	void PushRecordGameMsg(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uid = 0);
	void SaveRecordGameMsg();

protected:
	CGameRoom* m_pHostRoom;                                                   // 所属房间
	MemberTimerEvent<CGameTable, &CGameTable::OnProccess> m_timer;            // 定时器
	stTableConf                                           m_conf;             // 桌子配置
	vector<stSeat>                                        m_vecPlayers;       // 玩家
	map<uint32_t, CGamePlayer*>                             m_mpLookers;        // 围观者
	uint8_t                                                 m_gameState;        // 游戏状态
	int64_t                                                 m_tableID;          // 桌子ID
	CCooling                                                m_coolLogic;        // 逻辑CD
	uint8_t                                                 m_tableType;        // 桌子类型(动态桌子，私人桌子);
	//日志
	stGameBlingLog                                        m_blingLog;         // 牌局日志
	net::game_record                                      m_gameRecord;       // 游戏录像
	json                                                  m_operLog;          // 操作日志
	bool                                                  m_openRecord;       // 开启录像
	bool                                                  m_needOpenRecord;   // 需要开启录像
	string                                                m_chessid;          // 牌局ID
	uint32_t                                              m_round;            // 当前轮数


};




