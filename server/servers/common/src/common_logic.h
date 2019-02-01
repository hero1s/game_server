
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
	// ���ʱ���Ƿ�������
	static bool IsJoinTime(string startTime, string endTime);
	// ���У����
	static string VerifyPasswd(uint32_t uid, uint32_t checkTime);
	// �ж��Ƿ�������Ϣ
	static bool IsNeedReset(uint32_t lastTime, uint32_t curTime);

};

