

#include "game_server_config.h"

using namespace svrlib;
using namespace std;

// 导出Lua函数
void defLuaConfig(sol::state& lua)
{
	lua.new_usertype<GameServerConfig>
			   (
					   "GameServerConfig",
					   "GetDBConf", &GameServerConfig::GetDBConf,
					   "GetRedisConf",&GameServerConfig::GetRedisConf
			   );
	LOG_DEBUG("导出sol lua 函数");
}



































