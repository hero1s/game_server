
#pragma once

#include "svrlib.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdlib.h>

class CPlayer;

class CPlayerBase;

using namespace std;
using namespace svrlib;

class CCommonLogic
{
public:
	// 检测时间是否在区间
	static bool IsJoinTime(string startTime, string endTime);
	// 获得校验码
	static string VerifyPasswd(uint32_t uid, uint32_t checkTime);
	// 判断是否重置信息
	static bool IsNeedReset(time_t lastTime, time_t curTime);

};

