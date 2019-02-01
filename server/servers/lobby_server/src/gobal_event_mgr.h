
#pragma once

#include "svrlib.h"
#include "game_define.h"
#include "msg_define.pb.h"

using namespace std;
using namespace svrlib;

class CPlayer;

class CGobalEventMgr : public AutoDeleteSingleton<CGobalEventMgr>
{
public:
	CGobalEventMgr();

	~CGobalEventMgr();

	bool Init();

	void ShutDown();

	void ProcessTime();

private:
	void OnNewDay();

	void OnNewWeek();

	void OnNewMonth();

private:

};










































