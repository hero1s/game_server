
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

    void OnTimer();

    bool Init(asio::io_context& context, stRedisConf& conf);

    void ShutDown();

protected:
    bool Reconnect(bool bSync,bool bAsync);

    void HandSyncError(const std::string& err);
    void HandAsyncError(const std::string& err);

    void Test001(bool bLongLen);
    void Test002(bool bLongLen);

private:
    MemberTimerEvent<CRedisMgr, &CRedisMgr::OnTimer> m_timer;
    stRedisConf m_conf;

    std::shared_ptr<redisclient::RedisSyncClient>   m_syncClient;
    std::shared_ptr<redisclient::RedisAsyncClient>  m_asyncClient;

};
//同步客户端的断开不会回调到errorhandle处理,需要try cacth操作
//异步的已实现断线重连,在重连期间要做try cacth处理

























































































































































