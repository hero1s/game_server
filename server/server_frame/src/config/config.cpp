
#include "config/config.h"
#include "svrlib.h"

using namespace svrlib;
using namespace std;

// 打印日志
void c2l_Log(const char* logStr)
{
	LOG_DEBUG("{}",logStr);
}

// 导出Lua函数
void defLuaBaseConfig(sol::state& lua)
{
	lua.new_usertype<stDBConf>
			   (
					   "stDBConf",
					   "SetDBInfo", &stDBConf::SetDBInfo
			   );
	lua.new_usertype<stRedisConf>
			   (
					   "stRedisConf",
					   "SetRedisHost", &stRedisConf::SetRedisHost
			   );
	lua.new_usertype<stServerCfg>
			(
				"stServerCfg",
				"SetLogInfo",&stServerCfg::SetLogInfo
			);

	lua.set_function("c_log", c2l_Log);
}





