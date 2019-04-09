//
// Created by toney on 2019/4/9.
//

#pragma once

#include "svrlib.h"
#include "fundamental/multiprocesshashtable.h"
#include <unordered_map>
#include "asio.hpp"

using namespace svrlib;
using namespace std;

namespace {
    static const uint32_t s_CACHE_MAX_COUNT = 500*100;            // Cache 大小
    static const uint32_t s_LAST_CLEAR_TIME = 24*60*60/1000;       // 清除时间 24小时
    static const uint32_t s_LAST_SAVE_TIME = 5*60/10;             // 保存时间
    static const uint32_t s_CHECK_DATA_TIME = 10*60/10;           // 检测数据时间
}

// 缓存数据最大长度4K(单字段),超过的不缓存

#pragma pack(1)
template <uint32_t data_len>
struct stCacheData {
    uint32_t lastUpdateTime;
    uint32_t lastSaveTime;
    uint32_t uid;
    uint8_t cacheType;
    uint32_t dateLen;
    uint8_t szData[data_len];

    stCacheData(){
        Reset();
    }
    static uint32_t MaxDataLen(){ return data_len; }
    void SetValue(uint32_t _uid, uint8_t _cacheType, const string& data){
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
        memset(this, 0, sizeof(stCacheData));
    }
};
#pragma pack()


using handSaveFunc = function<void(uint32_t uid, uint8_t cacheType, const string& data)>;
template <uint32_t data_len>
class CDataCacheMgr
{
public:
    CDataCacheMgr(){};

    virtual ~CDataCacheMgr(){};

    bool Init(uint32_t shmKey, bool bReset,handSaveFunc func)
    {
        if (false==m_hpPlayerCache.InitShm(shmKey, s_CACHE_MAX_COUNT)) {
            LOG_ERROR("init player cach fail");
            return false;
        }
        if (bReset) {
            LOG_ERROR("清除共享内存数据");
            m_hpPlayerCache.Clear();// 重置共享内存
        }
        m_handSaveFunc = func;

        m_pTimer = make_shared<asio::system_timer>(CApplication::Instance().GetAsioContext());
        m_pTimer->expires_from_now(std::chrono::milliseconds(s_CHECK_DATA_TIME*1000));
        m_pTimer->async_wait(std::bind(&CDataCacheMgr::TimerCheck, this, std::placeholders::_1));

        return true;
    }
    void ShutDown()
    {
        SaveAllNeedData();
        if(m_pTimer)m_pTimer->cancel();
    }
    bool SetPlayerCacheData(uint32_t uid, uint8_t cacheType, const string& data)
    {
        uint64_t key = MakeKey(uid, cacheType);
        if (data.length()>=data_len) {
            LOG_ERROR("超出缓存最大长度:{}，不再缓存", data.length());
            DelPlayerCacheData(key);
            return false;
        }
        stCacheData<data_len>* pData = m_hpPlayerCache.GetValuePtr(key);
        if (pData!=NULL) {
            pData->SetValue(uid, cacheType, data);
            LOG_DEBUG("更新节点数据:key:{},uid:{},type:{}", key, uid, cacheType);
        }
        else {
            LOG_DEBUG("插入新节点数据:key:{},uid:{},type:{}", key, uid, cacheType);

            stCacheData<data_len> value;
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
    bool GetPlayerCacheData(uint32_t uid, uint8_t cacheType, string& data)
    {
        uint64_t key = MakeKey(uid, cacheType);
        stCacheData<data_len>* pData = m_hpPlayerCache.GetValuePtr(key);
        if (pData!=NULL) {
            pData->GetValue(data);
            LOG_DEBUG("获取缓存数据成功uid {} cacheType {} len {}",uid,cacheType,data.length());
            return true;
        }
        return false;
    }
    bool DelPlayerCacheData(uint32_t uid, uint8_t cacheType)
    {
        uint64_t key = MakeKey(uid, cacheType);
        return DelPlayerCacheData(key);
    }
    void SaveAllNeedData()
    {
        m_hpPlayerCache.BeginIterate();
        const HASH_ITEM* pItem = m_hpPlayerCache.GetNext();
        while (pItem) {
            if (pItem->m_oValue.lastUpdateTime>pItem->m_oValue.lastSaveTime) {
                string data;
                pItem->m_oValue.GetValue(data);
                m_handSaveFunc(pItem->m_oValue.uid, pItem->m_oValue.cacheType, data);
                FlushSaveTime(pItem->m_oKey);
            }
            pItem = m_hpPlayerCache.GetNext();
        }
    }

protected:
    void TimerCheck(const std::error_code& err)
    {
        CheckClearData();
        CheckSaveData();

        m_pTimer->expires_from_now(std::chrono::milliseconds(s_CHECK_DATA_TIME*1000));
        m_pTimer->async_wait(std::bind(&CDataCacheMgr::TimerCheck, this, std::placeholders::_1));
    }

    void CheckClearData()
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

    void CheckSaveData()
    {
        uint32_t curTick = getSysTime();
        vector<uint64_t> vecDels;
        auto it = m_mapOnlines.begin();
        for (; it!=m_mapOnlines.end(); ++it) {
            uint64_t key = it->first;
            uint32_t updateTime = it->second;
            if (curTick-updateTime<s_LAST_SAVE_TIME)
                continue;

            stCacheData<data_len>* pData = m_hpPlayerCache.GetValuePtr(key);
            if (pData==NULL) {
                LOG_ERROR("缓存数据未找到:%d", key);
                vecDels.push_back(key);
                continue;
            }
            if (pData->lastUpdateTime>pData->lastSaveTime && curTick-pData->lastSaveTime>s_LAST_SAVE_TIME) {
                //to do 保存数据到数据库
                string data;
                pData->GetValue(data);
                m_handSaveFunc(pData->uid, pData->cacheType, data);

                pData->lastSaveTime = curTick;
                LOG_DEBUG("保存缓存数据:{}", key);
                vecDels.push_back(key);
            }
        }
        for (uint32_t i = 0; i<vecDels.size(); ++i) {
            m_mapOnlines.erase(vecDels[i]);
        }
    }

private:
    uint64_t MakeKey(uint32_t uid, uint8_t cacheType)
    {
        uint64_t key = uid;
        key = key*100+cacheType;
        return key;
    }
    bool DelPlayerCacheData(uint64_t key)
    {
        LOG_DEBUG("删除缓存:key:{}", key);
        m_mapOnlines.erase(key);

        return m_hpPlayerCache.Delete(key);
    }
    void FlushSaveTime(uint64_t key)
    {
        stCacheData<data_len>* pData = m_hpPlayerCache.GetValuePtr(key);
        if (pData!=NULL) {
            pData->lastSaveTime = getSysTime();
        }
    }
    typedef CMultiProcessHashTable<uint64_t, stCacheData<data_len>, true> CACHE_HASH;
    typedef CHashItem<uint64_t, stCacheData<data_len>, true> HASH_ITEM;
    CACHE_HASH m_hpPlayerCache;
    std::unordered_map<uint64_t, uint32_t> m_mapOnlines;// 更新玩家数据key
    std::shared_ptr<asio::system_timer> m_pTimer;
    handSaveFunc m_handSaveFunc;
};
