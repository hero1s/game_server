
#pragma once

#include "svrlib.h"
#include "config/config.h"
#include <string>
#include "redis/redisclient.h"
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>

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
    void onConnect(boost::system::error_code ec);
    void onSet(const redisclient::RedisValue &value);
    void onGet(const redisclient::RedisValue &value);
private:
    MemberTimerEvent<CRedisMgr, &CRedisMgr::OnTimer> m_timer;

    std::shared_ptr<redisclient::RedisAsyncClient> m_asyncClient;//异步客户端
    std::shared_ptr<redisclient::RedisSyncClient>  m_syncClient; //同步客户端
    stRedisConf m_conf;
};


























































































































































