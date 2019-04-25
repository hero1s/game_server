
#include "servers_msg.pb.h"
#include "redis_mgr.h"
#include "game_define.h"
#include <stdlib.h>
#include "data_cfg_mgr.h"

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
    CApplication::Instance().schedule(&m_timer, 5000);

}

bool CRedisMgr::Init(asio::io_context & context, stRedisConf & conf) {
    m_conf = conf;
    CApplication::Instance().schedule(&m_timer, 5000);
    asio::ip::address address = asio::ip::address::from_string(m_conf.redisHost);
    const unsigned short port = m_conf.redisPort;
    asio::ip::tcp::endpoint endpoint(address, port);
    asio::error_code ec;
    redisclient::RedisValue result;
    //同步客户端
    m_syncClient = std::make_shared<redisclient::RedisSyncClient>(context);
    try {
        m_syncClient->connect(endpoint, ec);
        if (ec) {
            LOG_ERROR("Can't connect to redis: {}", ec.message());
            return false;
        } else {
            result = m_syncClient->command("AUTH", {m_conf.redisPasswd});
            if (result.isOk() && result.toString() == "OK") {
                LOG_DEBUG("connect redis ok and auth is ok");
            } else {
                LOG_ERROR("redis auth is error:{}", result.toString());
            }
        }
    } catch (asio::system_error e) {
        LOG_ERROR("redis throw error:{}", e.what());
        return false;
    }

    //异步客户端
    m_asyncClient = std::make_shared<redisclient::RedisAsyncClient>(context);
    try {
        m_asyncClient->connect(endpoint, [this](asio::error_code ec) {
            if (!ec) {
                m_asyncClient->command("AUTH", {m_conf.redisPasswd}, [this](const redisclient::RedisValue &v) {
                    if (v.isOk() && v.toString() == "OK") {
                        LOG_DEBUG("async redis client connect ok and auth ok");
                        test_client();
                    } else {
                        LOG_ERROR("async redis client connect error:{}", v.toString());
                    }
                });
            } else {
                LOG_ERROR("Can't connect to redis: {}", ec.message());
            }
        });
    } catch (asio::system_error e) {
        LOG_ERROR("redis throw error:{}", e.what());
        return false;
    }

    return true;
}

void CRedisMgr::ShutDown() {
    m_timer.cancel();

}

void CRedisMgr::test_client() {
    asio::error_code ec;
    redisclient::RedisValue result;

    result = m_syncClient->command("SET", {"key", "value"});
    if (result.isError()) {
        LOG_ERROR("SET error: {}", result.toString());
        return;
    }
    result = m_syncClient->command("GET", {"key"});
    if (result.isOk()) {
        LOG_DEBUG("GET: {}", result.toString());
    } else {
        LOG_ERROR("GET error: {}", result.toString());
        return;
    }

    m_asyncClient->command("SET", {"key", "value"}, [this](const redisclient::RedisValue &v) {
        LOG_DEBUG("async SET: {}", v.toString());
        m_asyncClient->command("GET", {"key"}, [this](const redisclient::RedisValue &v) {
            LOG_DEBUG("async GET: {}", v.toString());
        });
    });

}
