
#pragma once

// 玩家基础数据
struct stPlayerBaseInfo
{
  string name;         // 姓名
  uint8_t  sex;          // 性别
  uint32_t offlinetime;  // 最后登录时间
  string safepwd;      // 保险箱密码
  uint8_t  safeboxState; // 保险箱状态
  uint32_t clogin;       // 连续登陆天数
  uint8_t  weekLogin;    // 本周累计登陆
  uint8_t  bankrupt;     // 破产次数
  uint32_t loginIP;      // 登陆IP
  uint32_t allLoginDays; // 累计登陆天数
  int64_t  roomCard;     // 房卡
  int64_t  coin;         // 金币
  int32_t  vip;          // vip值
  int64_t  safecoin;     // 保险箱资产
  int64_t  clubID;       // 所在俱乐部ID
  double lon;           // 经纬度
  double lat;           // 经纬度

  stPlayerBaseInfo()
  {
	  clear();
  }

  void clear()
  {
	  name         = "";
	  sex          = 0;
	  offlinetime  = 0;
	  safepwd      = "";
	  safeboxState = 0;
	  clogin       = 0;
	  weekLogin    = 0;
	  bankrupt     = 0;
	  loginIP      = 0;
	  allLoginDays = 0;
	  roomCard     = 0;
	  coin         = 0;
	  vip          = 0;
	  safecoin     = 0;
	  clubID       = 0;
	  lon          = 0;
	  lat          = 0;

  }
};

enum emACC_VALUE_TYPE
{
  emACC_VALUE_ROOMCARD = 1,        // 房卡
  emACC_VALUE_COIN     = 2,        // 金币
  emACC_VALUE_SAFECOIN = 3,        // 保险箱值

  emACC_VALUE_MAX,                 // max
};

// 房间配置信息
struct stRoomCfg
{
  uint16_t roomID;       // 房间ID
  uint8_t  consume;      // 消费类型
  int64_t  enter_min;    // 进入门槛
  int64_t  enter_max;    // 进入限制
  int64_t  baseScore;    // 底分
  uint8_t  roomType;     // 房间类型(0普通房,1私人房)
  uint16_t tableNum;     // 创建桌子数
  uint8_t  marry;        // 匹配方式
  uint8_t  limitEnter;   // 进入限制(通过最小携带)
  uint32_t showonline;   // 显示在线
  int64_t  sitdown;      // 坐下条件
  uint8_t  feeType;      // 台费类型
  int32_t  feeValue;     // 台费值
  uint16_t seatNum;      // 座位数
  uint16_t showType;     // 显示类型
  uint16_t showPic;      // 显示图片

  stRoomCfg()
  {
	  memset(this, 0, sizeof(stRoomCfg));
  }
};

//任务奖励配置
struct stMissionPrizeCfg
{
  uint32_t poid;   //道具id
  uint32_t qty;    //数量
  stMissionPrizeCfg()
  {
	  memset(this, 0, sizeof(stMissionPrizeCfg));
  }
};

//任务配置信息
struct stMissionCfg
{
  uint32_t                    msid;        //任务ID
  uint16_t                    type;        //动作类型
  uint8_t                     autoprize;   //自动领奖
  uint32_t                    cate1;       //分类
  uint32_t                    cate2;       //分类
  uint32_t                    cate3;       //分类
  uint32_t                    cate4;       //分类
  uint32_t                    mtimes;      //达到次数
  uint8_t                     straight;     //是否连续
  uint8_t                     cycle;        //周期
  uint32_t                    cycletimes;  //可完成次数
  uint8_t                     status;       //任务状态
  vector<stMissionPrizeCfg> missionprize; //任务奖励
};

//玩家任务信息
struct stUserMission
{
  uint32_t msid;    // ID
  uint32_t rtimes;  // 任务进度
  uint32_t ctimes;  // 任务完成次数
  uint32_t ptime;   // 操作时间
  uint8_t  update;  // 更新数据库类型
  uint32_t cptime;  // 完成时间
  stUserMission()
  {
	  memset(this, 0, sizeof(stUserMission));
  }
};

// 数据类型
enum emACCDATA_TYPE
{
  emACCDATA_TYPE_BASE = 0,    // 基本信息
  emACCDATA_TYPE_MISS,        // 任务数据

  emACCDATA_TYPE_MAX,
};

enum emDBACTION
{
  emDB_ACTION_NONE = 0,           //不操作
  emDB_ACTION_UPDATE,             //修改
  emDB_ACTION_INSERT,             //插入
  emDB_ACTION_DELETE,             //删除
};

// 私人房信息
struct stPrivateTable
{
  int64_t  tableID;     // 桌子ID
  string passwd;      // 密码
  uint32_t hostID;      // 房主ID
  uint16_t dueround;    // 结束局数
  uint32_t dueTime;     // 到期时间
  uint32_t startTime;   // 开始时间
  int64_t  baseScore;   // 底分
  uint16_t seatNum;     // 座位数
  uint16_t gameType;    // 游戏类型
  uint8_t  playType;    // 玩法
  string addParam;    // 附加字符串参数
  string matchParam;  // 比赛参数
  int32_t  enterMin;    // 最小进入
  uint8_t  consume;     // 消费类型
  uint8_t  rentType;    // 房费支付方式
  uint32_t fee;         // 房费
  int64_t  clubID;      // 俱乐部ID
  int64_t  clubFloor;   // 俱乐部楼层
  uint16_t clubPos;     // 俱乐部桌子位置
  uint8_t  idxRound;    // 局数选择

  uint32_t createTime;  // 创建时间
  uint8_t  createType;  // 创建方式
  string gameInfo;    // 游戏信息

  stPrivateTable()
  {
	  tableID    = 0;
	  hostID     = 0;
	  passwd     = "";
	  hostID     = 0;
	  baseScore  = 0;
	  seatNum    = 0;
	  dueTime    = 0;
	  startTime  = 0;
	  gameType   = 0;
	  playType   = 0;
	  addParam   = ""; // 附加参数
	  matchParam = "";// 比赛参数
	  enterMin   = 0;
	  consume    = 0;
	  rentType   = 0;
	  fee        = 0;
	  clubID     = 0;
	  clubFloor  = 0;
	  clubPos    = 0;
	  idxRound   = 0;

	  createType = 0;
	  createTime = 0;
	  gameInfo   = "";
  }
};

// 私人房卡消耗
struct stRoomCardCfg
{
  uint8_t  idx;         // 序号
  uint16_t gameType;    // 游戏类型
  uint8_t  playType;    // 玩法
  uint8_t  round;       // 局数
  uint8_t  card;        // 房卡
  uint8_t  cardAA;      // 支付方式
  stRoomCardCfg()
  {
	  memset(this, 0, sizeof(stRoomCardCfg));
  }
};
// 游戏信息
struct stGameListCfg
{
  uint32_t gameID;    // 游戏ID
  uint16_t gameType;  // 游戏类型
  uint16_t playType;  // 玩法
  uint8_t  contest;   // 是否开放比赛
  stGameListCfg()
  {
	  memset(this, 0, sizeof(stGameListCfg));
  }
};

// 计分信息
struct stCalcScoreInfo
{
  uint32_t uid;
  int64_t  score;
  int64_t  fee;
  int64_t  buyIn;
  string param;
  int64_t  roundWin;//当前小局输赢

  stCalcScoreInfo()
  {
	  Reset();
  }
  void Reset()
  {
	  uid      = 0;
	  score    = 0;
	  fee      = 0;
	  buyIn    = 0;
	  param    = "";
	  roundWin = 0;
  }
  int64_t WinScore()
  {
	  return score - buyIn;
  }

};








