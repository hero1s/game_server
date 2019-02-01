#include "error_code.pb.h"
#include "data_cfg_mgr.h"
#include "common_logic.h"
#include "crypt/md5.h"
#include "db_struct_define.h"
#include "msg_define.pb.h"
#include "modern/nlohmann/json_wrap.h"

using namespace std;
using namespace svrlib;

CDataCfgMgr::CDataCfgMgr()
{

}

CDataCfgMgr::~CDataCfgMgr()
{

}

bool CDataCfgMgr::Init()
{
	if (!Reload())return false;

	return true;
}

void CDataCfgMgr::ShutDown()
{

}

bool CDataCfgMgr::Reload()
{

	return true;
}























