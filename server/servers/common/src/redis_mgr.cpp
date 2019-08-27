
#include "servers_msg.pb.h"
#include "redis_mgr.h"
#include "utility/puid.hpp"
#include "snappy/snappy.h"

using namespace std;
using namespace svrlib;

namespace {

};

CRedisMgr::CRedisMgr()
        : m_timer(this) {
    m_asyncConnecting = false;
}

CRedisMgr::~CRedisMgr() {
}

void CRedisMgr::OnTimer() {
    CApplication::Instance().schedule(&m_timer, 5000);

    try
    {
        m_asyncClient->command("PING", {}, [this](const redisclient::RedisValue &v)
        {
            if (v.isError() || v.toString() != "PONG")
            {
                LOG_DEBUG("PING error rep:{}", v.toString());
                Reconnect(true, false);
            }
        });
    }
    catch (const asio::system_error &e)
    {
        LOG_ERROR("async redis throw error:{}", e.what());
        Reconnect(false, true);
        return;
    }
    try
    {
        auto result = m_syncClient->command("PING", {});
        if (result.isError() || result.toString() != "PONG")
        {
            LOG_DEBUG("PING error rep:{}", result.toString());
            Reconnect(true, false);
        }
    }
    catch (const asio::system_error &e)
    {
        LOG_ERROR("sync redis throw error:{}", e.what());
        Reconnect(true, false);
        return;
    }
    for (auto i = 0; i < 50; ++i)
    {
        Test001(rand_range(0, 1) == 0 ? true : false);
        Test002(rand_range(0, 1) == 0 ? true : false);
        TestPileline();
    }
}

bool CRedisMgr::Init(asio::io_context &context, stRedisConf &conf) {
    m_conf = conf;
    CApplication::Instance().schedule(&m_timer, 20000);

    //同步客户端
    m_syncClient = std::make_shared<redisclient::RedisSyncClient>(context);
    m_syncClient->setConnectTimeout(std::chrono::seconds(5)).setCommandTimeout(std::chrono::seconds(5));

    //异步客户端
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
    //同步客户端
    if (bSync)
    {
        try
        {
            //同步客户端
            m_syncClient->disconnect();
            m_syncClient->connect(endpoint, ec);
            if (ec)
            {
                LOG_ERROR("Can't connect to redis: {},{}", ec.message(), endpoint.address().to_string());
                return false;
            }
            else
            {
                result = m_syncClient->command("AUTH", {m_conf.redisPasswd});
                if (result.isOk() && result.toString() == "OK")
                {
                    LOG_DEBUG("connect redis {} ok and auth is ok", endpoint.address().to_string());
                    TestConnect(true, false);
                }
                else
                {
                    LOG_ERROR("redis auth is error:{}", result.toString());
                    return false;
                }
            }
        }
        catch (const asio::system_error &e)
        {
            LOG_ERROR("sync redis connect throw error:{},{}", e.what(), endpoint.address().to_string());
            return false;
        }
    }
    //异步客户端
    if (bAsync)
    {
        try
        {
            //异步客户端
            if (m_asyncConnecting)
            {
                LOG_ERROR("asyncConnecting...");
                return false;
            }
            m_asyncConnecting = true;
            m_asyncClient->disconnect();
            m_asyncClient->connect(endpoint, [this](asio::error_code ec)
            {
                if (!ec)
                {
                    m_asyncClient->command("AUTH", {m_conf.redisPasswd}, [&](const redisclient::RedisValue &v)
                    {
                        m_asyncConnecting = false;
                        if (v.isOk() && v.toString() == "OK")
                        {
                            LOG_DEBUG("async redis client connect ok and auth ok");
                            TestConnect(false, true);
                        }
                        else
                        {
                            LOG_ERROR("async redis client connect error:{}", v.toString());
                        }
                    });
                }
                else
                {
                    m_asyncConnecting = false;
                    LOG_ERROR("Can't connect to redis: {}", ec.message());
                }
            });
        }
        catch (const asio::system_error &e)
        {
            LOG_ERROR("async redis connect throw error:{},{}", e.what(), endpoint.address().to_string());
            m_asyncConnecting = false;
            return false;
        }
    }
    return true;
}

void CRedisMgr::TestConnect(bool bSync, bool bAsync) {
    if (bSync)
    {
        auto result = SafeSyncCommond("SET", {"key", "value"});
        if (result.isError())
        {
            LOG_ERROR("SET error: {}", result.toString());
            return;
        }
        result = SafeSyncCommond("GET", {"key"});
        if (result.isOk())
        {
            LOG_DEBUG("GET: {}", result.toString());
        }
        else
        {
            LOG_ERROR("GET error: {}", result.toString());
            return;
        }
    }
    if (bAsync)
    {
        SafeAsyncCommond("SET", {"key", "value"},
                         [this](const redisclient::RedisValue &v)
                         {
                             LOG_DEBUG("async SET: {}", v.toString());
                             SafeAsyncCommond("GET", {"key"},
                                              [this](const redisclient::RedisValue &v)
                                              {
                                                  LOG_DEBUG("async GET: {}",
                                                            v.toString());
                                              });
                         });
    }
}

void CRedisMgr::HandSyncError(const std::string &err) {
    LOG_ERROR("redis sync error:{}", err);
    //m_syncClient->disconnect();
    //Reconnect(true, false);
}

void CRedisMgr::HandAsyncError(const std::string &err) {
    LOG_ERROR("redis async error:{}", err);
    //m_asyncClient->disconnect();
    //Reconnect(false, true);
}

void CRedisMgr::Test001(bool bLongLen) {

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

    auto result = SafeSyncCommond("SET", {key, value});
    if (result.isError())
    {
        LOG_ERROR("sync set:{}--{},result:{}", key, value, result.toString());
    }
    auto resultGet = SafeSyncCommond("GET", {key});
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
        SafeSyncCommond("DEL", {key});
    }

}

void CRedisMgr::Test002(bool bLongLen) {

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
    SafeAsyncCommond("SET", {key, value}, [this, key, value](const redisclient::RedisValue &v)
    {
        if (v.isError())
        {
            LOG_ERROR("async set:{}--{},result:{}", key, value, v.toString());
        }
        SafeAsyncCommond("GET", {key}, [this, key, value](const redisclient::RedisValue &v)
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
            SafeAsyncCommond("DEL", {key});
        });
    });

    auto testStr = R"({"uid":"756386_1565940975","room_id":528158,"club_id":1805991,
                           "club_master":180599,"room_owner":330468,"create_time":1565940975,"room_ju":10,
                            "played_ju":10,"game_type":"51",
                            "isfinished":1,"pay_type":0,"colltype":2,"proxy_mid":180599,
                            "fixed_index":4,"appid":112,"now_time":1565941413,"apptype":3,
                            "room_players":[{"mid":330468,"seatno":1,"roomscore":46,"ranking":0,"partnerid":0},
                            {"mid":182596,"seatno":2,"roomscore":-46,"ranking":0,"partnerid":0}],
                            "pay_params":{"pay_type":0,"params":{"roomcost":8,"payplayer":180599,"payreal":8,"isproxy":1}}})";
    SafeAsyncCommond("LPUSH", {"testqueue", testStr}, [this, key, value](const redisclient::RedisValue &v)
    {
        //LOG_DEBUG("LPUSH :{}",v.toString());
    });

}

redisclient::RedisValue CRedisMgr::SafeSyncCommond(const std::string &cmd, std::deque<redisclient::RedisBuffer> args) {
    try
    {
        return m_syncClient->command(cmd, args);
    }
    catch (const asio::system_error &e)
    {
        LOG_ERROR("sync redis throw error:{}", e.what());
    }
    redisclient::RedisValue::ErrorTag tag;
    return redisclient::RedisValue({}, tag);
}

void CRedisMgr::SafeAsyncCommond(const std::string &cmd, std::deque<redisclient::RedisBuffer> args,
                                 std::function<void(redisclient::RedisValue)> handler) {
    try
    {
        m_asyncClient->command(cmd, args, handler);
    }
    catch (const asio::system_error &e)
    {
        LOG_ERROR("async redis throw error:{}", e.what());
        //Reconnect(false, true);
    }
}

void CRedisMgr::TestPileline() {
    try
    {
        redisclient::Pipeline pipeline = m_syncClient->pipelined().command("SET", {"key", "value"})
                .command("INCR", {"counter"})
                .command("INCR", {"counter"})
                .command("DECR", {"counter"})
                .command("GET", {"counter"})
                .command("DEL", {"counter"})
                .command("DEL", {"key"});
        redisclient::RedisValue result = pipeline.finish();

        if (result.isOk())
        {
            for (const auto &i: result.toArray())
            {
                //LOG_DEBUG("Result: {}", i.inspect());
            }
        }
        else
        {
            LOG_ERROR("GET error: {}", result.toString());
        }
    }
    catch (const asio::system_error &e)
    {
        LOG_ERROR("sync redis pipeline throw error:{}", e.what());
        //Reconnect(true, false);
    }
}



