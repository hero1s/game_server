
#pragma once

#include "svrlib.h"
#include "config/config.h"
#include <string>
//#include "redis/redisclient.h"
#include "redisclient/redisasyncclient.h"
#include "redisclient/redissyncclient.h"
#include "asio.hpp"

using namespace std;
using namespace svrlib;

class CRedisMgr : public AutoDeleteSingleton<CRedisMgr> {
public:
    CRedisMgr();

    ~CRedisMgr();

    void OnTimer();

    bool Init(asio::io_context& context, stRedisConf& conf);

    void ShutDown();

protected:
    void test_client();


private:
    MemberTimerEvent<CRedisMgr, &CRedisMgr::OnTimer> m_timer;
    stRedisConf m_conf;

    std::shared_ptr<redisclient::RedisSyncClient>   m_syncClient;
    std::shared_ptr<redisclient::RedisAsyncClient>  m_asyncClient;

};


























































































































































