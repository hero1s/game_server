
#pragma once

#include "svrlib.h"
#include "fundamental/multiprocesshashtable.h"
#include <unordered_map>
#include "boost/asio.hpp"

using namespace svrlib;
using namespace std;

// 缓存数据最大长度4K(单字段),超过的不缓存
#define MAX_CACHE_DATA_LEN  4*1024

#pragma pack(1)

struct stPlayerCacheData {
    uint32_t lastUpdateTime;
    uint32_t lastSaveTime;
    uint32_t uid;
    uint8_t cacheType;
    uint32_t dateLen;
    uint8_t szData[MAX_CACHE_DATA_LEN];

    stPlayerCacheData()
    {
        memset(this, 0, sizeof(stPlayerCacheData));
    }

    void SetValue(uint32_t _uid, uint8_t _cacheType, const string& data)
    {
        uid = _uid;
        cacheType = _cacheType;
        dateLen = data.length();
        memcpy(szData, data.data(), data.length());
        lastUpdateTime = getSysTime();
        lastSaveTime = 0;
    }

    void GetValue(string& data) const
    {
        data.resize(dateLen);
        memcpy((char*) data.data(), szData, dateLen);
    }

    void Reset()
    {
        memset(this, 0, sizeof(stPlayerCacheData));
    }
};

#pragma pack()

class CPlayerCacheMgr : public AutoDeleteSingleton<CPlayerCacheMgr> {
public:
    CPlayerCacheMgr();

    virtual ~CPlayerCacheMgr();

    bool Init(uint32_t shmKey, bool bReset);

    void ShutDown();

    bool SetPlayerCacheData(uint32_t uid, uint8_t cacheType, const string& data);

    bool GetPlayerCacheData(uint32_t uid, uint8_t cacheType, string& data);

    bool DelPlayerCacheData(uint32_t uid, uint8_t cacheType);

    void SaveAllNeedData();

protected:
    void TimerCheck(const std::error_code& err);

    void CheckClearData();

    void CheckSaveData();

private:
    uint64_t MakeKey(uint32_t uid, uint8_t cacheType);

    bool DelPlayerCacheData(uint64_t key);

    void FlushSaveTime(uint64_t key);

    void SaveDataToDB(uint32_t uid, uint8_t cacheType, const string& data);

    typedef CMultiProcessHashTable<uint64_t, stPlayerCacheData, true> CACHE_HASH;
    typedef CHashItem<uint64_t, stPlayerCacheData, true> HASH_ITEM;

    CACHE_HASH m_hpPlayerCache;
    std::unordered_map<uint64_t, uint32_t> m_mapOnlines;// 更新玩家数据key
    std::shared_ptr<boost::asio::system_timer> m_pTimer;
};



