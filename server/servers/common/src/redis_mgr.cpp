
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
    m_client->ping();
    m_client->sync_commit();
}

bool CRedisMgr::Init(stRedisConf &conf) {
    m_conf = conf;
    CApplication::Instance().schedule(&m_timer, 5000);

    try
    {
        // Enable logging
        //cpp_redis::active_logger = std::unique_ptr<cpp_redis::logger>(new cpp_redis::logger);

        m_client = std::make_shared<cpp_redis::client>();
        m_client->connect(conf.redisHost, conf.redisPort,
                [&](const std::string &host, std::size_t port, cpp_redis::connect_state status) {
                    if (status == cpp_redis::connect_state::dropped) {
                        LOG_ERROR("client disconnected from {}:{} ", host, port);
                    } else {
                        m_client->auth(conf.redisPasswd, [&](const cpp_redis::reply &reply) {
                            LOG_DEBUG("redis auth reply: passwd {} {}", conf.redisPasswd, reply.as_string());
                        });
                    }
                },5000,1000,1000);

        // same as client.send({ "SET", "hello", "42" }, ...)
        m_client->set("hello", "42", [](cpp_redis::reply &reply) {
            LOG_DEBUG("set hello 42: {}", reply.as_string());
        });

        // same as client.send({ "DECRBY", "hello", 12 }, ...)
        m_client->decrby("hello", 12, [](cpp_redis::reply &reply) {
            LOG_DEBUG("decrby hello 12: {}", reply.as_integer());
        });

        // same as client.send({ "GET", "hello" }, ...)
        m_client->get("hello", [](cpp_redis::reply &reply) {
            LOG_DEBUG("get hello: {}", reply.as_string());
        });

        // commands are pipelined and only sent when client.commit() is called
        // client.commit();

        // synchronous commit, no timeout
        m_client->sync_commit();

        // synchronous commit, timeout
        // client.sync_commit(std::chrono::milliseconds(100));

    }
    catch(cpp_redis::redis_error e)
    {
        LOG_ERROR("redis error : {}",e.what());
    }

    return true;
}

void CRedisMgr::ShutDown() {
    m_timer.cancel();

}

