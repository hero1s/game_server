
#include "servers_msg.pb.h"
#include "redis_mgr.h"
#include "game_define.h"
#include <stdlib.h>
#include "data_cfg_mgr.h"
#include "utility/puid.hpp"
#include "snappy/snappy.h"

using namespace std;
using namespace svrlib;

namespace {

};

CRedisMgr::CRedisMgr()
        : m_timer(this) {

}

CRedisMgr::~CRedisMgr() {

}

void CRedisMgr::OnTimer() {
    CApplication::Instance().schedule(&m_timer, 10000);

    m_asyncClient->command("PING", {}, [](const redisclient::RedisValue &v)
    {
        //LOG_DEBUG("PING repeat:{}",v.toString());
    });
    try
    {
        auto result = m_syncClient->command("PING", {});
        //LOG_DEBUG("PING rep:{}",result.toString());
    }
    catch (const asio::system_error &e)
    {
        LOG_ERROR("redis throw error:{}", e.what());
        Reconnect(true, false);
        return;
    }
    Test001(rand_range(0,1) == 0 ? true : false);
    Test002(rand_range(0,1)  == 0 ? true : false);
}

bool CRedisMgr::Init(asio::io_context &context, stRedisConf &conf) {
    m_conf = conf;
    CApplication::Instance().schedule(&m_timer, 20000);

    //ͬ���ͻ���
    m_syncClient = std::make_shared<redisclient::RedisSyncClient>(context);
    m_syncClient->setConnectTimeout(std::chrono::seconds(3));
    m_syncClient->setCommandTimeout(std::chrono::seconds(3));
    //�첽�ͻ���
    m_asyncClient = std::make_shared<redisclient::RedisAsyncClient>(context);

    m_syncClient->installErrorHandler(std::bind(&CRedisMgr::HandSyncError, this, std::placeholders::_1));
    m_asyncClient->installErrorHandler(std::bind(&CRedisMgr::HandAsyncError, this, std::placeholders::_1));

    return Reconnect(true, true);
}

void CRedisMgr::ShutDown() {
    m_timer.cancel();

}

bool CRedisMgr::Reconnect(bool bSync, bool bAsync) {
    asio::ip::address address = asio::ip::address::from_string(m_conf.redisHost);
    const unsigned short port = m_conf.redisPort;
    asio::ip::tcp::endpoint endpoint(address, port);
    asio::error_code ec;
    redisclient::RedisValue result;
    //ͬ���ͻ���
    if (bSync)
    {
        try
        {
            //ͬ���ͻ���
            m_syncClient->disconnect();
            m_syncClient->connect(endpoint, ec);
            if (ec)
            {
                LOG_ERROR("Can't connect to redis: {}", ec.message());
                return false;
            }
            else
            {
                result = m_syncClient->command("AUTH", {m_conf.redisPasswd});
                if (result.isOk() && result.toString() == "OK")
                {
                    LOG_DEBUG("connect redis ok and auth is ok");
                    {//test
                        redisclient::RedisValue result;
                        result = m_syncClient->command("SET", {"key", "value"});
                        if (result.isError())
                        {
                            LOG_ERROR("SET error: {}", result.toString());
                            return false;
                        }
                        result = m_syncClient->command("GET", {"key"});
                        if (result.isOk())
                        {
                            LOG_DEBUG("GET: {}", result.toString());
                        }
                        else
                        {
                            LOG_ERROR("GET error: {}", result.toString());
                            return false;
                        }
                    }
                }
                else
                {
                    LOG_ERROR("redis auth is error:{}", result.toString());
                }
            }
        }
        catch (const asio::system_error &e)
        {
            LOG_ERROR("redis throw error:{}", e.what());
            return false;
        }
    }
    //�첽�ͻ���
    if (bAsync)
    {
        try
        {
            //�첽�ͻ���
            m_asyncClient->disconnect();
            m_asyncClient->connect(endpoint, [this](asio::error_code ec)
            {
                if (!ec)
                {
                    m_asyncClient->command("AUTH", {m_conf.redisPasswd}, [this](const redisclient::RedisValue &v)
                    {
                        if (v.isOk() && v.toString() == "OK")
                        {
                            LOG_DEBUG("async redis client connect ok and auth ok");
                            {//test
                                try
                                {
                                    redisclient::RedisValue result;
                                    m_asyncClient->command("SET", {"key", "value"},
                                                           [this](const redisclient::RedisValue &v)
                                                           {
                                                               LOG_DEBUG("async SET: {}", v.toString());
                                                               m_asyncClient->command("GET", {"key"},
                                                                                      [this](const redisclient::RedisValue &v)
                                                                                      {
                                                                                          LOG_DEBUG("async GET: {}",
                                                                                                    v.toString());
                                                                                      });
                                                           });
                                }
                                catch (const asio::system_error &e)
                                {
                                    LOG_ERROR("redis throw error:{}", e.what());
                                }
                            }
                        }
                        else
                        {
                            LOG_ERROR("async redis client connect error:{}", v.toString());
                        }
                    });
                }
                else
                {
                    LOG_ERROR("Can't connect to redis: {}", ec.message());
                }
            });
        }
        catch (const asio::system_error &e)
        {
            LOG_ERROR("redis throw error:{}", e.what());
            return false;
        }
    }
    return true;
}

void CRedisMgr::HandSyncError(const std::string &err) {
    LOG_ERROR("redis sync error:{}", err);
    //m_syncClient->disconnect();
    Reconnect(true, false);
}

void CRedisMgr::HandAsyncError(const std::string &err) {
    LOG_ERROR("redis async error:{}", err);
    //m_asyncClient->disconnect();
    Reconnect(false, true);
}

void CRedisMgr::Test001(bool bLongLen) {
    if(!m_syncClient->isConnected())
        return;

    auto key = svrlib::uuid::generate();
    auto value = svrlib::uuid::generate();
    if (bLongLen)
    {
        auto maxlen = rand_range(1, 5);
        for (int i = 0; i < maxlen; ++i)
        {
            value += value;
        }
    }

    auto result = m_syncClient->command("SET", {key, value});
    if (result.isError())
    {
        LOG_ERROR("sync set:{}--{},result:{}", key, value, result.toString());
    }
    auto resultGet = m_syncClient->command("GET", {key});
    if (result.isError())
    {
        LOG_ERROR("sync get:{},result:{}", key, resultGet.toString());
    }
    else
    {
        if (resultGet.toString() != value)
        {
            LOG_ERROR("get and set is not same:{}--{}", value, resultGet.toString());
        }
        m_syncClient->command("DEL",{key});
    }
}

void CRedisMgr::Test002(bool bLongLen) {
    if(!m_asyncClient->isConnected())
        return;

    auto key = svrlib::uuid::generate();
    auto value = svrlib::uuid::generate();
    if (bLongLen)
    {
        auto maxlen = rand_range(1, 5);
        for (int i = 0; i < maxlen; ++i)
        {
            value += value;
        }
    }
    m_asyncClient->command("SET", {key, value}, [this, key, value](const redisclient::RedisValue &v)
    {
        if (v.isError())
        {
            LOG_ERROR("async set:{}--{},result:{}", key, value, v.toString());
        }
        m_asyncClient->command("GET", {key}, [this, key, value](const redisclient::RedisValue &v)
        {
            if (v.isError())
            {
                LOG_ERROR("async get:{},result:{}", key, v.toString());
            }
            else
            {
                if (v.toString() != value)
                {
                    LOG_ERROR("get and set is not same:{}--{}", value, v.toString());
                }
            }
            m_asyncClient->command("DEL",{key});
        });
    });
}
