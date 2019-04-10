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
    static const uint32_t s_CACHE_MAX_COUNT = 500 * 100;             // Cache ��С
    static const uint32_t s_LAST_CLEAR_TIME = 24 * 60 * 60 / 1000;   // ���ʱ�� 24Сʱ
    static const uint32_t s_CHECK_DATA_TIME = 10 * 60;               // �������ʱ��
}

// ����������󳤶�4K(���ֶ�),�����Ĳ�����

#pragma pack(1)

template<uint32_t data_len>
struct stCacheData {
    uint32_t lastUpdateTime;
    uint32_t lastSaveTime;
    uint32_t uid;
    uint8_t cacheType;
    uint32_t dateLen;
    uint8_t szData[data_len];

    stCacheData() {
        Reset();
    }

    static uint32_t MaxDataLen() { return data_len; }

    void SetValue(uint32_t _uid, uint8_t _cacheType, const string &data) {
        uid = _uid;
        cacheType = _cacheType;
        dateLen = data.length();
        memcpy(szData, data.data(), data.length());
        lastUpdateTime = getSysTime();
        lastSaveTime = 0;
    }

    void GetValue(string &data) const {
        data.resize(dateLen);
        memcpy((char *) data.data(), szData, dateLen);
    }

    void Reset() {
        memset(this, 0, sizeof(stCacheData));
    }
};

#pragma pack()


using handSaveFunc = function<void(uint32_t uid, uint8_t cacheType, const string &data)>;

template<uint32_t data_len, uint32_t save_time>
class CDataCacheMgr {
public:
    CDataCacheMgr() {};

    virtual ~CDataCacheMgr() {};

    bool Init(uint32_t shmKey, bool bReset, handSaveFunc func) {
        if (false == m_hpPlayerCache.InitShm(shmKey, s_CACHE_MAX_COUNT)) {
            LOG_ERROR("init player cach fail");
            return false;
        }
        if (bReset) {
            LOG_ERROR("clear shm data");
            m_hpPlayerCache.Clear();// ���ù����ڴ�
        }
        m_handSaveFunc = func;

        m_pTimer = make_shared<asio::system_timer>(CApplication::Instance().GetAsioContext());
        m_pTimer->expires_from_now(std::chrono::milliseconds(s_CHECK_DATA_TIME * 1000));
        m_pTimer->async_wait(std::bind(&CDataCacheMgr::TimerCheck, this, std::placeholders::_1));

        return true;
    }

    void ShutDown() {
        SaveAllNeedData();
        if (m_pTimer)m_pTimer->cancel();
    }

    bool SetPlayerCacheData(uint32_t uid, uint8_t cacheType, const string &data) {
        uint64_t key = MakeKey(uid, cacheType);
        if (data.length() >= data_len) {
            LOG_ERROR("more than max cache data len :{}��don't cache", data.length());
            DelPlayerCacheData(key);
            return false;
        }
        stCacheData<data_len> *pData = m_hpPlayerCache.GetValuePtr(key);
        if (pData != NULL) {
            pData->SetValue(uid, cacheType, data);
            LOG_DEBUG("update node cache data :key:{},uid:{},type:{}", key, uid, cacheType);
        } else {
            LOG_DEBUG("insert new node cache data :key:{},uid:{},type:{}", key, uid, cacheType);

            stCacheData<data_len> value;
            value.SetValue(uid, cacheType, data);

            int8_t bRet = m_hpPlayerCache.Insert(key, value);
            if (bRet == INSERT_FAIL) {
                LOG_ERROR("update cache fail");
            } else if (bRet == INSERT_SUCCESS) {
            } else if (bRet == INSERT_SUCCESS_WILL_FULL) {
                LOG_ERROR("update success but want full,please more extra");
            } else if (bRet == INSERT_SUCCESS_BUT_LRU) {
                LOG_ERROR("update success and remove old node");
            }
        }
        // ��¼����ʱ��
        m_mapOnlines.insert(make_pair(key, getSysTime()));
        return true;
    }

    bool GetPlayerCacheData(uint32_t uid, uint8_t cacheType, string &data) {
        uint64_t key = MakeKey(uid, cacheType);
        stCacheData<data_len> *pData = m_hpPlayerCache.GetValuePtr(key);
        if (pData != NULL) {
            pData->GetValue(data);
            LOG_DEBUG("get cache node data success  uid {} cacheType {} len {}", uid, cacheType, data.length());
            return true;
        }
        return false;
    }

    bool DelPlayerCacheData(uint32_t uid, uint8_t cacheType) {
        uint64_t key = MakeKey(uid, cacheType);
        return DelPlayerCacheData(key);
    }

    void SaveAllNeedData() {
        m_hpPlayerCache.BeginIterate();
        const HASH_ITEM *pItem = m_hpPlayerCache.GetNext();
        while (pItem) {
            if (pItem->m_oValue.lastUpdateTime > pItem->m_oValue.lastSaveTime) {
                string data;
                pItem->m_oValue.GetValue(data);
                m_handSaveFunc(pItem->m_oValue.uid, pItem->m_oValue.cacheType, data);
                FlushSaveTime(pItem->m_oKey);
            }
            pItem = m_hpPlayerCache.GetNext();
        }
    }

protected:
    void TimerCheck(const std::error_code &err) {
        CheckClearData();
        CheckSaveData();

        m_pTimer->expires_from_now(std::chrono::milliseconds(s_CHECK_DATA_TIME * 1000));
        m_pTimer->async_wait(std::bind(&CDataCacheMgr::TimerCheck, this, std::placeholders::_1));
    }

    void CheckClearData() {
        uint32_t curTick = getSysTime();
        m_hpPlayerCache.BeginIterate();
        const HASH_ITEM *pItem = m_hpPlayerCache.GetNext();
        while (pItem) {
            // ����ʱ��δ���µ�����,���ڿ��Լ�����������toney
            if (curTick - pItem->m_oValue.lastUpdateTime > s_LAST_CLEAR_TIME) {
                LOG_DEBUG("clear time out cache node ��key {},uid:{},cacheType:{}", pItem->m_oKey, pItem->m_oValue.uid,
                          pItem->m_oValue.cacheType);
                m_hpPlayerCache.Delete(pItem->m_oKey);
            }
            pItem = m_hpPlayerCache.GetNext();
        }
    }

    void CheckSaveData() {
        uint32_t curTick = getSysTime();
        vector<uint64_t> vecDels;
        auto it = m_mapOnlines.begin();
        for (; it != m_mapOnlines.end(); ++it) {
            uint64_t key = it->first;
            uint32_t updateTime = it->second;
            if (curTick - updateTime < save_time)
                continue;

            stCacheData<data_len> *pData = m_hpPlayerCache.GetValuePtr(key);
            if (pData == NULL) {
                LOG_ERROR("cache data not find :{}", key);
                vecDels.push_back(key);
                continue;
            }
            if (pData->lastUpdateTime > pData->lastSaveTime && curTick - pData->lastSaveTime > save_time) {
                //to do �������ݵ����ݿ�
                string data;
                pData->GetValue(data);
                m_handSaveFunc(pData->uid, pData->cacheType, data);

                pData->lastSaveTime = curTick;
                LOG_DEBUG("save cache node data :{}", key);
                vecDels.push_back(key);
            }
        }
        for (uint32_t i = 0; i < vecDels.size(); ++i) {
            m_mapOnlines.erase(vecDels[i]);
        }
    }

private:
    uint64_t MakeKey(uint32_t uid, uint8_t cacheType) {
        uint64_t key = uid;
        key = key * 100 + cacheType;
        return key;
    }

    bool DelPlayerCacheData(uint64_t key) {
        LOG_DEBUG("delete cache node key:{}", key);
        m_mapOnlines.erase(key);

        return m_hpPlayerCache.Delete(key);
    }

    void FlushSaveTime(uint64_t key) {
        stCacheData<data_len> *pData = m_hpPlayerCache.GetValuePtr(key);
        if (pData != NULL) {
            pData->lastSaveTime = getSysTime();
        }
    }

    typedef CMultiProcessHashTable<uint64_t, stCacheData<data_len>, true> CACHE_HASH;
    typedef CHashItem<uint64_t, stCacheData<data_len>, true> HASH_ITEM;
    CACHE_HASH m_hpPlayerCache;
    std::unordered_map<uint64_t, uint32_t> m_mapOnlines;// �����������key
    std::shared_ptr<asio::system_timer> m_pTimer = nullptr;
    handSaveFunc m_handSaveFunc = nullptr;
};
