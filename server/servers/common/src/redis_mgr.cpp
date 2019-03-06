
#include "servers_msg.pb.h"
#include "redis_mgr.h"
#include "game_define.h"
#include <stdlib.h>
#include "data_cfg_mgr.h"


using namespace std;
using namespace svrlib;

namespace {
    //异步
    static const std::string redisKey = "unique-redis-key-example";
    static const std::string redisValue = "unique-redis-value";
};

CRedisMgr::CRedisMgr()
        : m_timer(this) {

}

CRedisMgr::~CRedisMgr() {

}

void CRedisMgr::OnTimer() {
    CApplication::Instance().schedule(&m_timer, 5000);
    if(!m_asyncClient->isConnected()){//重连

    }
    if(!m_syncClient->isConnected()){//重连

    }
}

bool CRedisMgr::Init(stRedisConf &conf) {
    m_conf = conf;
    CApplication::Instance().schedule(&m_timer, 5000);

    boost::asio::ip::address address = boost::asio::ip::address::from_string(conf.redisHost);
    const unsigned short port = conf.redisPort;
    boost::asio::ip::tcp::endpoint endpoint(address, port);

    m_syncClient = std::make_shared<redisclient::RedisSyncClient>(CApplication::Instance().GetAsioContext());
    m_asyncClient = std::make_shared<redisclient::RedisAsyncClient>(CApplication::Instance().GetAsioContext());
    boost::system::error_code ec;

    m_syncClient->connect(endpoint, ec);
    if(ec){
        LOG_ERROR("Can't connect to redis: {}",ec.message());
        return false;
    }
    redisclient::RedisValue result;
    result = m_syncClient->command("AUTH",{conf.redisPasswd});
    if(result.isOk()){
        LOG_DEBUG("AUTH reply:{}",result.toString());
    }
    result = m_syncClient->command("SET", {"key", "value"});
    if( result.isError() ){
        LOG_ERROR("SET error: {}",result.toString());
        return false;
    }
    result = m_syncClient->command("GET", {"key"});
    if( result.isOk() ){
        LOG_DEBUG("GET: {}",result.toString());
    }else{
        LOG_ERROR("GET error: {}",result.toString());
        return false;
    }
    m_asyncClient->connect(endpoint,std::bind(&CRedisMgr::onConnect,this,std::placeholders::_1));

    return true;
}

void CRedisMgr::ShutDown() {
    m_timer.cancel();
}
void CRedisMgr::onConnect(boost::system::error_code ec){
    if(ec){
        LOG_ERROR("Can't connect to redis: {}",ec.message());
    }else{
        m_asyncClient->command("AUTH",{m_conf.redisPasswd},[this](const redisclient::RedisValue &value){
            if(value.toString() == "OK"){
                m_asyncClient->command("SET",  {redisKey, redisValue},
                                       std::bind(&CRedisMgr::onSet, this, std::placeholders::_1));
            }
        });
    }
}
void CRedisMgr::onSet(const redisclient::RedisValue &value)
{
    LOG_DEBUG("SET: {}",value.toString());
    if( value.toString() == "OK" ){
       m_asyncClient->command("GET",  {redisKey},
                            std::bind(&CRedisMgr::onGet, this, std::placeholders::_1));
    }else{
        LOG_ERROR("Invalid value from redis: {}" , value.toString());
    }
}
void CRedisMgr::onGet(const redisclient::RedisValue &value)
{
    LOG_DEBUG("GET {}",value.toString());
    if( value.toString() != redisValue ){
        LOG_ERROR("Invalid value from redis: {}",value.toString());
    }

    m_asyncClient->command("DEL", {redisKey});
}


















