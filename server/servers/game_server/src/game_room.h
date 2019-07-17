
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

// ��������
enum emROOM_TYPE {
    emROOM_TYPE_COMMON = 0, // ��ͨ��
    emROOM_TYPE_PRIVATE,    // ˽�˷�
    emROOM_TYPE_MATCH,      // ������
};

class CGameRoom {
public:
    CGameRoom();

    virtual ~CGameRoom();

    // ��������
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

    //��������
    uint8_t GetConsume();

    //��С����
    int64_t GetEnterMin();

    //������
    int64_t GetEnterMax();

    //�׷�
    int32_t GetBaseScore();

    //��������
    uint8_t GetRoomType();

    //����Я��
    int64_t GetSitDown();

    //�Ƿ���Ҫ����
    bool IsNeedMarry();

    //�������
    int32_t GetPlayerNum();

    //��Ϸ����
    uint16_t GetGameType();

    bool CanEnterRoom(CGamePlayer *pGamePlayer);

    bool CanLeaveRoom(CGamePlayer *pGamePlayer);

    void GetRoomInfo(net::room_info *pRoom);

    //���ͽ�ҳ�������Ϣ�����
    void SendTableListToPlayer(CGamePlayer *pGamePlayer, int64_t tableID);

    //��ѯ�����б���Ϣ
    void QueryTableListToPlayer(CGamePlayer *pGamePlayer, uint32_t start, uint32_t end);

    //��������������Ϣ
    void CopyRoomCfgToTableCfg(stTableConf &conf);

    CGameTable *MallocTable();

    void FreeTable(CGameTable *pTable);

    // ��ȡ����ID����
    CGameTable *GetTable(int64_t tableID);

    // ��ȡ��������
    CGameTable *GetTableByPasswd(string passwd);

    // ����Լ�������
    void GetTableHostID(uint32_t hostID, vector<CGameTable *> &vecTables);

    // ����Լ����������
    void GetSelfJoinTables(uint32_t playerID, vector<CGameTable *> &vecTables);

    // ��þ��ֲ�����
    void GetClubTables(int64_t clubID, vector<CGameTable *> &vecTables);

    // ��þ��ֲ�����
    CGameTable *GetClubTable(int64_t clubID, int64_t floor, uint16_t pos);

    // �ܷ񴴽�����
    bool CanCreatePlayerTable(stTableConf &cfg);

    // ͬ����������
    CGameTable *CreatePlayerTable(stTableConf &cfg);

    // ��������
    bool LoadPrivateTable(stPrivateTable &table);

    // �����տ�����
    void CheckRecover();

    // ����Ƿ���Ҫ����������
    void CheckNewTable();

    // ƥ������
    void MarryTable();

    // �����������
    bool JoinNoFullTable(CGamePlayer *pPlayer, uint32_t excludeID);

    // ����ƥ��
    void JoinMarry(CGamePlayer *pPlayer, uint32_t excludeID);

    // �뿪ƥ��
    void LeaveMarry(CGamePlayer *pPlayer);

    // �Ƿ���ƥ����
    bool IsJoinMarry(CGamePlayer *pPlayer);

    // ����������
    uint32_t GetFreeTableNum();

protected:
    void CalcShowOnline();

protected:
    using QUEUE_TABLE = queue<CGameTable *>;
    using MAP_TABLE = unordered_map<uint32_t, CGameTable *>;
    using QUEUE_PLAYER = map<CGamePlayer *, uint32_t>;

    MAP_TABLE m_mpTables;                // ��������
    QUEUE_TABLE m_freeTable;             // ��������
    QUEUE_PLAYER m_marryPlayers;         // ƥ��������
    int64_t m_roomIndex;                 // ���䷿������
    uint32_t m_playerNum;                // �������
    uint32_t m_showonline;               // ��ʾ����
    stRoomCfg m_roomCfg;                 // ��������
    uint16_t m_gameType;                 // ��Ϸ����
    CCooling m_coolMarry;                // ����CD
    CCooling m_coolRecover;              // ��������CD
    CCooling m_coolNewTable;             // �������������
    MemberTimerEvent<CGameRoom, &CGameRoom::OnTimer> m_timer;

};


