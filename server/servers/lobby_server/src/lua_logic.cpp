
#include "lua_logic.h"
#include "stdafx.h"

using namespace svrlib;
using namespace std;


// 导出Lua函数
void defLuaLogic(sol::state& lua)
{
	lua.new_usertype<CPlayer>
			   (
					   "CPlayer",
					   "GetUID", &CPlayer::GetUID
			   );

	LOG_DEBUG("导出Lua逻辑函数");
}


