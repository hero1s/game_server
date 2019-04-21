
#pragma once

#include "svrlib.h"
#include "config/config.h"
#include <string>
#include <cpp_redis/cpp_redis>
#include "redisclient.h"

using namespace std;
using namespace svrlib;

class CRedisMgr : public AutoDeleteSingleton<CRedisMgr> {
public:
    CRedisMgr();

    ~CRedisMgr();

    void OnTimer();

    bool Init(stRedisConf& conf);

    void ShutDown();

protected:
    void test_client();


private:
    MemberTimerEvent<CRedisMgr, &CRedisMgr::OnTimer> m_timer;
    std::shared_ptr<cpp_redis::client> m_client;
    stRedisConf m_conf;

    std::shared_ptr<redisclient::RedisSyncClient>   m_syncClient;
    std::shared_ptr<redisclient::RedisAsyncClient>  m_asyncClient;

};


























































































































































