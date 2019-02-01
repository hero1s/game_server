
#include "gobal_event_mgr.h"
#include "stdafx.h"
#include "player.h"


using namespace svrlib;

namespace
{

};

CGobalEventMgr::CGobalEventMgr()
{
}

CGobalEventMgr::~CGobalEventMgr()
{

}

bool CGobalEventMgr::Init()
{

	return true;
}

void CGobalEventMgr::ShutDown()
{

}

void CGobalEventMgr::ProcessTime()
{
	static uint64_t uProcessTime = 0;

	uint64_t uTime = getSysTime();
	uint64_t uTick = getSystemTick64();
	if (!uProcessTime)
		uProcessTime = uTime;
	if (uTime == uProcessTime)
		return;// 一秒一次

	bool bNewDay = (diffTimeDay(uProcessTime, uTime) != 0);
	if (bNewDay)
	{
		OnNewDay();
	}
	uProcessTime = uTime;
}

void CGobalEventMgr::OnNewDay()
{
	bool bNewWeek  = false;
	bool bNewMonth = false;

	// 新的一天
	tm     local_time;
	uint64_t uTime   = getTime();
	getLocalTime(&local_time, uTime);
	// 跨周        0-6
	if (local_time.tm_wday == 0)
	{
		bNewWeek = true;
		OnNewWeek();
	}
	// 跨月        1-31
	if (local_time.tm_mday == 1)
	{
		bNewMonth = true;
		OnNewMonth();
	}

}

void CGobalEventMgr::OnNewWeek()
{

}

void CGobalEventMgr::OnNewMonth()
{

}






















