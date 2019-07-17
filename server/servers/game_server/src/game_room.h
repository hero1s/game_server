
#pragma once

#include <game_player.h>
#include <queue>
#include "svrlib.h"
#include "db_struct_define.h"
#include "msg_define.pb.h"
#include "game_table.h"
#include <unordered_map>

using namespace svrlib;
using namespace std;

class CGamePlayer;

class CGameTable;

class CGamePrivateTable;

class CGameCoinTable;

// 房间类型
enum emROOM_TYPE {
    emROOM_TYPE_COMMON = 0, // 普通房
    emROOM_TYPE_PRIVATE,    // 私人房
    emROOM_TYPE_MATCH,      // 比赛房
};

class CGameRoom {
public:
    CGameRoom();

    virtual ~CGameRoom();

    // 工厂函数
    CGamePrivateTable *CreatePrivateTable(int64_t tableID, uint16_t playType);

    CGameCoinTable *CreateCoinTable(int64_t tableID);

    void OnTimer();

    bool Init(uint16_t gameType);

    void ShutDown();

    void OnTimeTick();

    bool EnterRoom(CGamePlayer *pGamePlayer);

    bool LeaveRoom(CGamePlayer *pGamePlayer);

    uint8_t EnterTable(CGamePlayer *pGamePlayer, int64_t tableID, string passwd);

    bool FastJoinTable(CGamePlayer *pGamePlayer);

    void SetRoomCfg(stRoomCfg &cfg);

    uint16_t GetRoomID();

    //消费类型
    uint8_t GetConsume();

    //最小进入
    int64_t GetEnterMin();

    //最大进入
    int64_t GetEnterMax();

    //底分
    int32_t GetBaseScore();

    //房间类型
    uint8_t GetRoomType();

    //坐下携带
    int64_t GetSitDown();

    //是否需要搓桌
    bool IsNeedMarry();

    //玩家数量
    int32_t GetPlayerNum();

    //游戏类型
    uint16_t GetGameType();

    bool CanEnterRoom(CGamePlayer *pGamePlayer);

    bool CanLeaveRoom(CGamePlayer *pGamePlayer);

    void GetRoomInfo(net::room_info *pRoom);

    //发送金币场桌子信息给玩家
    void SendTableListToPlayer(CGamePlayer *pGamePlayer, int64_t tableID);

    //查询桌子列表信息
    void QueryTableListToPlayer(CGamePlayer *pGamePlayer, uint32_t start, uint32_t end);

    //拷贝房间配置信息
    void CopyRoomCfgToTableCfg(stTableConf &conf);

    CGameTable *MallocTable();

    void FreeTable(CGameTable *pTable);

    // 获取桌子ID桌子
    CGameTable *GetTable(int64_t tableID);

    // 获取密码桌子
    CGameTable *GetTableByPasswd(string passwd);

    // 获得自己的桌子
    void GetTableHostID(uint32_t hostID, vector<CGameTable *> &vecTables);

    // 获得自己参与的桌子
    void GetSelfJoinTables(uint32_t playerID, vector<CGameTable *> &vecTables);

    // 获得俱乐部桌子
    void GetClubTables(int64_t clubID, vector<CGameTable *> &vecTables);

    // 获得俱乐部桌子
    CGameTable *GetClubTable(int64_t clubID, int64_t floor, uint16_t pos);

    // 能否创建桌子
    bool CanCreatePlayerTable(stTableConf &cfg);

    // 同步创建桌子
    CGameTable *CreatePlayerTable(stTableConf &cfg);

    // 加载桌子
    bool LoadPrivateTable(stPrivateTable &table);

    // 检测回收空桌子
    void CheckRecover();

    // 检测是否需要生成新桌子
    void CheckNewTable();

    // 匹配桌子
    void MarryTable();

    // 进入空闲桌子
    bool JoinNoFullTable(CGamePlayer *pPlayer, uint32_t excludeID);

    // 加入匹配
    void JoinMarry(CGamePlayer *pPlayer, uint32_t excludeID);

    // 离开匹配
    void LeaveMarry(CGamePlayer *pPlayer);

    // 是否在匹配中
    bool IsJoinMarry(CGamePlayer *pPlayer);

    // 空闲桌子数
    uint32_t GetFreeTableNum();

protected:
    void CalcShowOnline();

protected:
    using QUEUE_TABLE = queue<CGameTable *>;
    using MAP_TABLE = unordered_map<uint32_t, CGameTable *>;
    using QUEUE_PLAYER = map<CGamePlayer *, uint32_t>;

    MAP_TABLE m_mpTables;                // 在玩桌子
    QUEUE_TABLE m_freeTable;             // 空闲桌子
    QUEUE_PLAYER m_marryPlayers;         // 匹配队列玩家
    int64_t m_roomIndex;                 // 分配房间索引
    uint32_t m_playerNum;                // 玩家人数
    uint32_t m_showonline;               // 显示在线
    stRoomCfg m_roomCfg;                 // 房间配置
    uint16_t m_gameType;                 // 游戏类型
    CCooling m_coolMarry;                // 搓桌CD
    CCooling m_coolRecover;              // 回收桌子CD
    CCooling m_coolNewTable;             // 检测生成新桌子
    MemberTimerEvent<CGameRoom, &CGameRoom::OnTimer> m_timer;

};


