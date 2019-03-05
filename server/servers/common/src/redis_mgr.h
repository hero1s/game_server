
#pragma once

#include "svrlib.h"
#include "config/config.h"
#include <string>
#include <cpp_redis/cpp_redis>
#include "redis/redisclient.h"

using namespace std;
using namespace svrlib;

class CRedisMgr : public AutoDeleteSingleton<CRedisMgr> {
public:
    CRedisMgr();

    ~CRedisMgr();

    void OnTimer();

    bool Init(stRedisConf& conf);

    void ShutDown();

private:
    MemberTimerEvent<CRedisMgr, &CRedisMgr::OnTimer> m_timer;
    std::shared_ptr<cpp_redis::client> m_client;

    std::shared_ptr<redisclient::RedisAsyncClient> m_asyncClient;//异步客户端
    std::shared_ptr<redisclient::RedisSyncClient>  m_syncClient; //同步客户端
};


























































































































































