
#pragma once

#include "svrlib.h"
#include "config/config.h"
#include <string>
#include "redisclient/redisasyncclient.h"
#include "redisclient/redissyncclient.h"
#include "asio.hpp"

using namespace std;
using namespace svrlib;

using reply_callback_t = std::function<void(redisclient::RedisValue v)>;

class CRedisMgr : public AutoDeleteSingleton<CRedisMgr> {
public:
    CRedisMgr();

    ~CRedisMgr();

    void OnTimer();

    bool Init(asio::io_context &context, vector<stRedisConf> &confs);

    void ShutDown();

protected:
    bool Reconnect(bool bSync, bool bAsync);

    void TestConnect(bool bSync, bool bAsync);

    void HandSyncError(const std::string &err);

    void HandAsyncError(const std::string &err);

    void Test001(bool bLongLen);

    void Test002(bool bLongLen);

    void TestPileline();

    redisclient::RedisValue SafeSyncCommond(const std::string &cmd, std::deque<redisclient::RedisBuffer> args);

    void SafeAsyncCommond(const std::string &cmd, std::deque<redisclient::RedisBuffer> args,
                          std::function<void(redisclient::RedisValue)> handler = [](redisclient::RedisValue){});

private:
    MemberTimerEvent<CRedisMgr, &CRedisMgr::OnTimer> m_timer;
    stRedisConf m_conf;

    std::shared_ptr<redisclient::RedisSyncClient> m_syncClient = nullptr;
    std::shared_ptr<redisclient::RedisAsyncClient> m_asyncClient = nullptr;

    bool m_asyncConnecting = false;
};
/*
    注意：connect,commond,pipeline命令报错会抛异常
*/


























































































































































