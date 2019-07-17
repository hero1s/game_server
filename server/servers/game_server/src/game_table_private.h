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

// 比赛场设置
enum emTAX_TYPE
{
  emTAX_TYPE_TABLE = 0, // 总局抽水
  emTAX_TYPE_ROUND = 1, // 小局抽水

};

struct stScoreModeSetting
{
  //积分场信息
  bool   canMinus;                        // 是否可以为负
  uint8_t  taxType;                       // 抽水方式
  uint32_t tax;                           // 抽水比例
  uint8_t  taxWinNum;                     // 抽水人数
  uint32_t taxBaseWin;                    // 抽水基数(大于多少分才抽)
  int64_t  minBankerScore;                // 最小上庄积分
  uint8_t  scoreExchange;                 // 积分兑换比例

  stScoreModeSetting()
  {
	  Reset();
  }
  void Reset()
  {
	  memset(this, 0, sizeof(stScoreModeSetting));
  }
};

// 游戏组局桌子
class CGamePrivateTable : public CGameTable
{
public:
	CGamePrivateTable(CGameRoom* pRoom, int64_t tableID);
	virtual ~CGamePrivateTable();

public://重载函数
	virtual bool Init();
	virtual void ShutDown();
	virtual void OnProccess();
	//游戏消息
	virtual int OnMessage(CGamePlayer* pPlayer, uint16_t cmdID, const uint8_t* pkt_buf, uint16_t buf_len);

	virtual bool EnterTable(CGamePlayer* pPlayer);
	virtual bool LeaveTable(CGamePlayer* pPlayer, bool bNotify = false, uint8_t leaveType = 0);
	// 能否进入
	virtual int32_t CanEnterTable(CGamePlayer* pPlayer);
	// 能否离开
	virtual bool CanLeaveTable(CGamePlayer* pPlayer);
	// 是否参与游戏
	virtual bool IsJoinPlay(uint16_t chairID);
	// 能否坐下
	virtual bool CanSitDown(CGamePlayer* pPlayer, uint16_t chairID);
	// 能否站起
	virtual bool CanStandUp(CGamePlayer* pPlayer);
	// 需要坐下
	virtual bool NeedSitDown();
	// 玩家准备
	virtual bool PlayerReady(CGamePlayer* pPlayer, bool bReady);
	//用户断线或重连
	virtual bool OnActionUserNetState(CGamePlayer* pPlayer, bool bConnected, bool isJoin = true);
	//用户坐下
	virtual bool OnActionUserSitDown(uint16_t wChairID, CGamePlayer* pPlayer);
	//用户起立
	virtual bool OnActionUserStandUp(uint16_t wChairID, CGamePlayer* pPlayer);
	//用户同意
	virtual bool OnActionUserOnReady(uint16_t wChairID, CGamePlayer* pPlayer);
	//玩家进入或离开
	virtual void OnPlayerJoin(bool isJoin, uint16_t chairID, CGamePlayer* pPlayer);

	// 是否需要回收
	virtual bool NeedRecover();
	// 即将回收
	virtual bool WantNeedRecover();
public:
	// 私人房操作
	virtual void LoadPrivateTable(stPrivateTable& data);
	// 创建私人房
	virtual bool CreatePrivateTable();
	// 检测桌子配置文件
	virtual bool CheckPrivateTableConfRight();

public:
	// 投票解散
	virtual void VoteGameOver(uint32_t reason, uint32_t opid, uint32_t reqID, uint32_t agree);
	virtual bool AllAgreeVote();
	virtual void ResetVoteInfo();
	virtual bool IsVoteStatus();
	// 通知游戏提前结束
	virtual void NotifyGameOver(uint32_t reason);
	// 通知投票信息
	virtual void NotifyVoteGameOver();
	// 重置投票信息
	virtual void ResetVoteGameOverState();
	// 强制结束
	virtual void ForceGameOver(bool bNormal);
	// 逻辑结束
	virtual void LogicGameOver();

	// 增加玩家信息
	virtual void AddPrivatePlayerInfo(uint32_t uid, bool bAddLog);
	// 删除玩家信息
	virtual void DelPrivatePlayerInfo(uint32_t uid);
	// 自动删除没有座位的玩家
	virtual void AutoDelNoSeatPrivatePlayerInfo();
	virtual bool IsExistPrivatePlayerInfo(uint32_t uid);



	// 私人结算附加信息
	virtual void SetPrivatePlayerInfoParam(uint32_t uid, string key, int32_t value);
	virtual int32_t GetPrivatePlayerInfoParam(uint32_t uid, string key);
	virtual void ChangePrivatePlayerInfoParam(uint32_t uid, string key, int32_t value, bool bReplaceMax = false);
	virtual int64_t GetPlayerGameScore(uint32_t uid);
	virtual void SetPlayerBuyIn(uint32_t uid,int64_t score);

	// 是否可为负
	virtual bool CanMinus();
	virtual int64_t ChangeScoreByUID(uint32_t uid, int64_t score);
	int64_t SetScoreByUID(uint32_t uid, int64_t score); //设置分数
	// 结算玩家信息
	virtual void CalcPlayerGameInfo(uint32_t uid, int64_t winScore);
	// 清理当前局输赢
	virtual void ClearRoundWinScore();
	// 检测自动离开
	virtual void CheckAutoLeaveTable();
	// 检测积分模式是否需要提前结束
	virtual bool CheckScoreModeGameOver();
public:
	// 一局开始
	virtual void OnGameRoundStart();
	// 一局结束
	virtual void OnGameRoundOver();
	virtual void OnGameRoundFlush();
	// 游戏结束
	virtual void OnGameOver(uint8_t overType);
	// 是否结束
	virtual uint8_t NeedGameOver();
	// 检测私人房是否结束
	virtual void CheckPrivateGameOver();
	// 是否开始游戏
	virtual bool IsStartGame();

	// 保存游戏信息
	virtual void SavePrivateTableGameInfo();
	virtual void FlushPrivateTableInfoToCache();
	// 游戏信息
	virtual void SendPrivateGameInfo();
	// 刷新私人房信息变更到俱乐部
	virtual void FlushClubPrivateListToClient(bool bDelete = false);
	// 记录俱乐部相关信息
	virtual void LogClubGameInfo();
	//AA支付
	virtual bool PayAARoomCard(CGamePlayer* pPlayer);
	//AA退款
	virtual void BackPayAARoomCard(CGamePlayer* pPlayer);
	//支付房卡
	void PayRoomCard();
	//退回房卡
	void BackRoomCard();
public:
	//是否积分场
	bool IsScoreMode();
	//初始化积分场积分
	void ResetClubScore(CGamePlayer* pPlayer);
	//抽水
	void CalcRoundTaxScore();
	void CalcTableTaxScore();
	//刷新积分信息到俱乐部
	void CalcClubScore();
	//检测玩家是否还能继续
	void CheckPlayerCanPlay();
	//能否坐庄
	bool CanBankerScore(uint32_t uid);
	//设置积分场参数
	bool SetMatchParam();

protected:
	//投票
	CCooling m_agreeCooling;     // 投票表决CD
	uint32_t   m_agreeID;          // 发起人
	uint8_t    m_voteReason;       // 发起原因
	bool     m_voteEnd;          // 表决是否结束
	bool     m_voteDiss;         // 投票解散
	bool     m_forceDiss;        // 强制解散
	bool     m_logicDiss;        // 逻辑结束

	//桌子信息
	map<uint32_t, stCalcScoreInfo> m_mpPlayerScoreInfo;             // 计分玩家信息

	bool   m_bClosed;                       // 是否关闭
	uint32_t m_startTime;                     // 房卡游戏开始游戏时间
	uint32_t m_endTime;                       // 结束时间
	uint32_t m_createTime;                    // 创建时间
	bool   m_canJoinPlaying;                // 是否禁止中途加入

	//积分场信息
	stScoreModeSetting m_scoreSetting;      // 积分设置

};





