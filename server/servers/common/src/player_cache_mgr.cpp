

#include "player_cache_mgr.h"
#include <functional>

using namespace svrlib;
using namespace std;

namespace {
    static const uint32_t s_CACHE_MAX_COUNT = 500*100;            // Cache ��С
    static const uint32_t s_LAST_CLEAR_TIME = 24*60*60/1000;       // ���ʱ�� 24Сʱ
    static const uint32_t s_LAST_SAVE_TIME = 5*60/10;             // ����ʱ��
    static const uint32_t s_CHECK_DATA_TIME = 10*60/10;           // �������ʱ��
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
        LOG_ERROR("��������ڴ�����");
        m_hpPlayerCache.Clear();// ���ù����ڴ�
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
        LOG_ERROR("����������󳤶�:{}�����ٻ���", data.length());
        DelPlayerCacheData(key);
        return false;
    }
    stPlayerCacheData* pData = m_hpPlayerCache.GetValuePtr(key);
    if (pData!=NULL) {
        pData->SetValue(uid, cacheType, data);
        LOG_DEBUG("���½ڵ�����:key:{},uid:{},type:{}", key, uid, cacheType);
    }
    else {
        LOG_DEBUG("�����½ڵ�����:key:{},uid:{},type:{}", key, uid, cacheType);

        stPlayerCacheData value;
        value.SetValue(uid, cacheType, data);

        int8_t bRet = m_hpPlayerCache.Insert(key, value);
        if (bRet==INSERT_FAIL) {
            LOG_ERROR("����ʧ��");
        }
        else if (bRet==INSERT_SUCCESS) {
        }
        else if (bRet==INSERT_SUCCESS_WILL_FULL) {
            LOG_ERROR("���³ɹ�����HashTable�����ˣ�������Ӧ������");
        }
        else if (bRet==INSERT_SUCCESS_BUT_LRU) {
            LOG_ERROR("���³ɹ�������̭��һ���ϵĽ��");
        }
    }

    // ��¼����ʱ��
    m_mapOnlines.insert(make_pair(key, getSysTime()));

    return true;
}

bool CPlayerCacheMgr::GetPlayerCacheData(uint32_t uid, uint8_t cacheType, string& data)
{
    uint64_t key = MakeKey(uid, cacheType);
    stPlayerCacheData* pData = m_hpPlayerCache.GetValuePtr(key);
    if (pData!=NULL) {
        pData->GetValue(data);
        LOG_DEBUG("��ȡ�������ݳɹ�uid {} cacheType {} len {}",uid,cacheType,data.length());
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
        // ����ʱ��δ���µ�����,���ڿ��Լ�����������toney
        if (curTick-pItem->m_oValue.lastUpdateTime>s_LAST_CLEAR_TIME) {
            LOG_DEBUG("������ڵ�Cache���ݣ�key {},uid:{},cacheType:{}", pItem->m_oKey, pItem->m_oValue.uid,
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
            LOG_ERROR("��������δ�ҵ�:%d", key);
            vecDels.push_back(key);
            continue;
        }
        if (pData->lastUpdateTime>pData->lastSaveTime && curTick-pData->lastSaveTime>s_LAST_SAVE_TIME) {
            //to do �������ݵ����ݿ�
            string data;
            pData->GetValue(data);
            SaveDataToDB(pData->uid, pData->cacheType, data);

            pData->lastSaveTime = curTick;
            LOG_DEBUG("���滺������:{}", key);
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
    LOG_DEBUG("ɾ������:key:{}", key);
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