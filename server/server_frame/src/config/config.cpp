
#include "config/config.h"
#include "svrlib.h"

using namespace svrlib;
using namespace std;

// ��ӡ��־
void c2l_Log(const char* logStr)
{
	LOG_DEBUG("{}",logStr);
}

// ����Lua����
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





