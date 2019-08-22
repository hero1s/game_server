
#pragma once

#include "svrlib.h"
#include "config/config.h"
#include <string>
#include "redisclient/redisasyncclient.h"
#include "redisclient/redissyncclient.h"
#include "asio.hpp"

using namespace std;
using namespace svrlib;

class CRedisMgr : public AutoDeleteSingleton<CRedisMgr> {
public:
    CRedisMgr();

    ~CRedisMgr();

    void OnTimer(const std::error_code& err);

    bool Init(asio::io_context& context, stRedisConf& conf);

    void ShutDown();

protected:
    bool Reconnect(bool bSync,bool bAsync);

    void HandSyncError(const std::string& err);
    void HandAsyncError(const std::string& err);

    void Test001(bool bLongLen);
    void Test002(bool bLongLen);
    void TestPileline();

private:
    std::shared_ptr<asio::system_timer> m_pTimer = nullptr;
    stRedisConf m_conf;

    std::shared_ptr<redisclient::RedisSyncClient>   m_syncClient;
    std::shared_ptr<redisclient::RedisAsyncClient>  m_asyncClient;

};
/*
    注意：connect,commond,pipeline命令报错会抛异常
*/


























































































































































