

#pragma once

#include <string>
#include "svrlib.h"
#include <string.h>
#include "config/config.h"
#include "game_define.h"
#include "modern/sol/sol.hpp"

using namespace std;
using namespace svrlib;

/**
 * 单例，用于存放配置
 */

struct GameServerConfig : public AutoDeleteSingleton<GameServerConfig>
{
public:
	stDBConf DBConf[DB_INDEX_TYPE_MAX];
	stRedisConf redisConf;

	stDBConf* GetDBConf(uint8_t index)
	{
		if (index < DB_INDEX_TYPE_MAX)
		{
			return &DBConf[index];
		}
		return NULL;
	}

	string GetDBName(uint8_t index)
	{
		if (index < DB_INDEX_TYPE_MAX)
		{
			return DBConf[index].sDBName;
		}
		return "";
	}
	stRedisConf* GetRedisConf()
	{
		return &redisConf;
	}

};

// 导出Lua函数
extern void defLuaConfig(sol::state& lua);

