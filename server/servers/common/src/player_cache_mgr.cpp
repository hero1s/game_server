

#include "player_cache_mgr.h"
#include <functional>

using namespace svrlib;
using namespace std;

namespace {
    static const uint32_t s_CACHE_MAX_COUNT = 500*100;            // Cache 大小
    static const uint32_t s_LAST_CLEAR_TIME = 24*60*60/1000;       // 清除时间 24小时
    static const uint32_t s_LAST_SAVE_TIME = 5*60/10;             // 保存时间
    static const uint32_t s_CHECK_DATA_TIME = 10*60/10;           // 检测数据时间
}

CPlayerCacheMgr::CPlayerCacheMgr()
{

}

CPlayerCacheMgr::~CPlayerCacheMgr()
{
}

bool CPlayerCacheMgr::Init(uint32_t shmKey, bool bReset)
{
    if (false==m_hpPlayerCache.InitShm(shmKey, s_CACHE_MAX_COUNT)) {
        LOG_ERROR("init player cach fail");
        return false;
    }
    if (bReset) {
        LOG_ERROR("清除共享内存数据");
        m_hpPlayerCache.Clear();// 重置共享内存
    }

    m_pTimer = make_shared<boost::asio::system_timer>(CApplication::Instance().GetAsioContext());
    m_pTimer->expires_from_now(std::chrono::milliseconds(s_CHECK_DATA_TIME*1000));
    m_pTimer->async_wait(std::bind(&CPlayerCacheMgr::TimerCheck, this, std::placeholders::_1));

    return true;
}

void CPlayerCacheMgr::ShutDown()
{
    SaveAllNeedData();
    if(m_pTimer)m_pTimer->cancel();
}

bool CPlayerCacheMgr::SetPlayerCacheData(uint32_t uid, uint8_t cacheType, const string& data)
{
    uint64_t key = MakeKey(uid, cacheType);
    if (data.length()>=MAX_CACHE_DATA_LEN) {
        LOG_ERROR("超出缓存最大长度:{}，不再缓存", data.length());
        DelPlayerCacheData(key);
        return false;
    }
    stPlayerCacheData* pData = m_hpPlayerCache.GetValuePtr(key);
    if (pData!=NULL) {
        pData->SetValue(uid, cacheType, data);
        LOG_DEBUG("更新节点数据:key:{},uid:{},type:{}", key, uid, cacheType);
    }
    else {
        LOG_DEBUG("插入新节点数据:key:{},uid:{},type:{}", key, uid, cacheType);

        stPlayerCacheData value;
        value.SetValue(uid, cacheType, data);

        int8_t bRet = m_hpPlayerCache.Insert(key, value);
        if (bRet==INSERT_FAIL) {
            LOG_ERROR("更新失败");
        }
        else if (bRet==INSERT_SUCCESS) {
        }
        else if (bRet==INSERT_SUCCESS_WILL_FULL) {
            LOG_ERROR("更新成功但是HashTable快满了，调用者应扩容了");
        }
        else if (bRet==INSERT_SUCCESS_BUT_LRU) {
            LOG_ERROR("更新成功但是淘汰了一个老的结点");
        }
    }

    // 记录更新时间
    m_mapOnlines.insert(make_pair(key, getSysTime()));

    return true;
}

bool CPlayerCacheMgr::GetPlayerCacheData(uint32_t uid, uint8_t cacheType, string& data)
{
    uint64_t key = MakeKey(uid, cacheType);
    stPlayerCacheData* pData = m_hpPlayerCache.GetValuePtr(key);
    if (pData!=NULL) {
        pData->GetValue(data);
        LOG_DEBUG("获取缓存数据成功uid {} cacheType {} len {}",uid,cacheType,data.length());
        return true;
    }
    return false;
}

bool CPlayerCacheMgr::DelPlayerCacheData(uint32_t uid, uint8_t cacheType)
{
    uint64_t key = MakeKey(uid, cacheType);
    return DelPlayerCacheData(key);
}

void CPlayerCacheMgr::SaveAllNeedData()
{
    m_hpPlayerCache.BeginIterate();
    const HASH_ITEM* pItem = m_hpPlayerCache.GetNext();
    while (pItem) {
        if (pItem->m_oValue.lastUpdateTime>pItem->m_oValue.lastSaveTime) {
            string data;
            pItem->m_oValue.GetValue(data);
            SaveDataToDB(pItem->m_oValue.uid, pItem->m_oValue.cacheType, data);
            FlushSaveTime(pItem->m_oKey);
        }
        pItem = m_hpPlayerCache.GetNext();
    }
}

void CPlayerCacheMgr::TimerCheck(const std::error_code& err)
{
    CheckClearData();
    CheckSaveData();

    m_pTimer->expires_from_now(std::chrono::milliseconds(s_CHECK_DATA_TIME*1000));
    m_pTimer->async_wait(std::bind(&CPlayerCacheMgr::TimerCheck, this, std::placeholders::_1));
}

void CPlayerCacheMgr::CheckClearData()
{
    uint32_t curTick = getSysTime();
    m_hpPlayerCache.BeginIterate();
    const HASH_ITEM* pItem = m_hpPlayerCache.GetNext();
    while (pItem) {
        // 清理长时间未更新的数据,后期可以加入容量控制toney
        if (curTick-pItem->m_oValue.lastUpdateTime>s_LAST_CLEAR_TIME) {
            LOG_DEBUG("清理过期的Cache数据：key {},uid:{},cacheType:{}", pItem->m_oKey, pItem->m_oValue.uid,
                    pItem->m_oValue.cacheType);
            m_hpPlayerCache.Delete(pItem->m_oKey);
        }
        pItem = m_hpPlayerCache.GetNext();
    }
}

void CPlayerCacheMgr::CheckSaveData()
{
    uint32_t curTick = getSysTime();

    vector<uint64_t> vecDels;
    auto it = m_mapOnlines.begin();
    for (; it!=m_mapOnlines.end(); ++it) {
        uint64_t key = it->first;
        uint32_t updateTime = it->second;
        if (curTick-updateTime<s_LAST_SAVE_TIME)
            continue;

        stPlayerCacheData* pData = m_hpPlayerCache.GetValuePtr(key);
        if (pData==NULL) {
            LOG_ERROR("缓存数据未找到:%d", key);
            vecDels.push_back(key);
            continue;
        }
        if (pData->lastUpdateTime>pData->lastSaveTime && curTick-pData->lastSaveTime>s_LAST_SAVE_TIME) {
            //to do 保存数据到数据库
            string data;
            pData->GetValue(data);
            SaveDataToDB(pData->uid, pData->cacheType, data);

            pData->lastSaveTime = curTick;
            LOG_DEBUG("保存缓存数据:{}", key);
            vecDels.push_back(key);
        }
    }
    for (uint32_t i = 0; i<vecDels.size(); ++i) {
        m_mapOnlines.erase(vecDels[i]);
    }
}

uint64_t CPlayerCacheMgr::MakeKey(uint32_t uid, uint8_t cacheType)
{
    uint64_t key = uid;
    key = key*100+cacheType;
    return key;
}

bool CPlayerCacheMgr::DelPlayerCacheData(uint64_t key)
{
    LOG_DEBUG("删除缓存:key:{}", key);
    m_mapOnlines.erase(key);

    return m_hpPlayerCache.Delete(key);
}

void CPlayerCacheMgr::CPlayerCacheMgr::FlushSaveTime(uint64_t key)
{
    stPlayerCacheData* pData = m_hpPlayerCache.GetValuePtr(key);
    if (pData!=NULL) {
        pData->lastSaveTime = getSysTime();
    }
}

void CPlayerCacheMgr::SaveDataToDB(uint32_t uid, uint8_t cacheType, const string& data)
{

}