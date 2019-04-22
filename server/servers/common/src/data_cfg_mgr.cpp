#include "error_code.pb.h"
#include "data_cfg_mgr.h"
#include "common_logic.h"
#include "crypt/md5.h"
#include "db_struct_define.h"
#include "msg_define.pb.h"
#include "nlohmann/json_wrap.h"
#include "modern/file.hpp"
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>

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
	//º”‘ÿΩ«…´≈‰÷√±Ì
	string file_name = "../server_config/scp/roles.txt";
	string strRoles = file::read_all(file_name);
	conf::Roles roles;
	if(google::protobuf::TextFormat::ParseFromString(strRoles,&roles)){
		LOG_DEBUG("load roles success:{}",roles.list_size());
	}else{
		LOG_ERROR("load roles failed:file: {},file_str size: {}",file_name,strRoles.length());
	}

	return true;
}























