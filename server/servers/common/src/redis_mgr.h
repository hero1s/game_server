
#pragma once

#include "svrlib.h"
#include "config/config.h"
#include <string>
#include "redis/redis.h"
#include <cpp_redis/cpp_redis>

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
    //std::shared_ptr<Redis> m_client;
    std::shared_ptr<cpp_redis::client> m_client;

};


























































































































































