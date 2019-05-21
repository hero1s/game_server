
#pragma once

#include <string>
#include "svrlib.h"
#include "db_struct_define.h"
#include <vector>
#include "game_define.h"
#include <unordered_map>
#include <unordered_set>
#include "res.pb.h"

using namespace std;
using namespace svrlib;

/*************************************************************/
class CDataCfgMgr : public AutoDeleteSingleton<CDataCfgMgr>
{
public:
	CDataCfgMgr();

	virtual ~CDataCfgMgr();

public:
	bool Init();

	void ShutDown();

	bool Reload();

protected:


};

