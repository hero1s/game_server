
#pragma once

#include <string>
#include "utility/basic_types.h"
#include "modern/sol/sol.hpp"

namespace svrlib {
struct stDBConf {
  uint32_t      uPort;
  std::string sHost;
  std::string sDBName;
  std::string sUser;
  std::string sPwd;

  stDBConf()
  {
      uPort   = 0;
      sHost   = "";
      sDBName = "";
      sUser   = "";
      sPwd    = "";
  }

  void SetDBInfo(const char* host, uint32_t port, const char* dbName, const char* userName, const char* pwd)
  {
      sHost   = host;
      uPort   = port;
      sDBName = dbName;
      sUser   = userName;
      sPwd    = pwd;
  }
};

struct stRedisConf {
  std::string redisHost;
  uint32_t      redisPort;
  uint8_t       redisIndex;
  std::string redisPasswd;
  uint8_t       redisRole;

  void SetRedisHost(const char* host, uint32_t port, uint32_t index, const char* passwd, uint32_t role)
  {
      redisHost   = host;
      redisPort   = port;
      redisIndex  = index;
      redisPasswd = passwd;
      redisRole   = role;
  }
};

struct stServerCfg {
    uint8_t  loglv            = 0;
    uint32_t logsize          = 52428800;
    uint16_t logdays          = 5;
    uint32_t logasync         = 0;
    std::string logname     = "log";
    std::string logmysql    = "mysql_error";

    stServerCfg(){

    }
    void SetLogInfo(uint8_t _loglv,uint32_t _logsize,uint16_t _logdays,uint32_t _logasync,const char* _logname, const char* _logmysql)
    {
        loglv        = _loglv;
        logsize      = _logsize;
        logdays      = _logdays;
        logasync     = _logasync;
        logname      = _logname;
        logmysql     = _logmysql;
    }

};


};

// 打印日志
extern void c2l_Log(const char* logStr);

// 导出Lua函数
extern void defLuaBaseConfig(sol::state& lua);

