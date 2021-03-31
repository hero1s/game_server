

#include "dbmysql_mgr.h"
#include "svrlib.h"
#include <string.h>
#include <svrlib.h>
#include "common_logic.h"
#include "player_mgr.h"
#include "data_cfg_mgr.h"
#include "game_server_config.h"
#include "dbmysql/db_wrap.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

using namespace std;
using namespace svrlib;

namespace
{
	static string s_PlayerDataFields[] = {"Base","Task"};//玩家二进制数据字段名
/* 测试用表
 CREATE TABLE `t_player` (
  `PlayerID` int NOT NULL DEFAULT '0' COMMENT 'uid',
  `Base` varbinary(500) NOT NULL DEFAULT '' COMMENT 'base',
  `Task` varbinary(500) NOT NULL DEFAULT '' COMMENT 'Task',
  `OfflineTime` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`PlayerID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci
*/
};

CDBMysqlMgr::CDBMysqlMgr()
		:m_reportTimer(this)
{
	m_svrID = 0;
}

CDBMysqlMgr::~CDBMysqlMgr()
{

}

void CDBMysqlMgr::OnTimer()
{
	CApplication::Instance().Schedule(&m_reportTimer, 20*1000);
}

bool CDBMysqlMgr::Init(const vector<stDBConf>& DBConfs)
{
	m_svrID = CApplication::Instance().GetServerID();
	m_DBConfs.clear();
	for (uint32_t i = 0; i < DBConfs.size(); ++i)
	{
		m_DBConfs.push_back(DBConfs[i]);
	}
	if (!ConnectSyncDB())
	{
		return false;
	}
	StartAsyncDB();

	CApplication::Instance().Schedule(&m_reportTimer, 5*1000);

	return true;
}

void CDBMysqlMgr::ShutDown()
{
	StopAsyncDB();
	m_syncDBOper.dbClose();
	m_reportTimer.cancel();
}

// 启动异步线程
bool CDBMysqlMgr::StartAsyncDB()
{
	for(uint16_t i=0;i<m_DBConfs.size();++i) {
		auto pAsyncTask = std::make_shared<CDBTask>(CApplication::Instance().GetAsioContext());
		pAsyncTask->SetDBName(m_DBConfs[i].sDBName);
		pAsyncTask->Init(m_DBConfs[i]);
		pAsyncTask->Start();
		m_pAsyncTasks.push_back(pAsyncTask);
	}
	return true;
}
// 停止日志异步线程
bool CDBMysqlMgr::StopAsyncDB()
{
	for(uint16_t i=0;i<m_pAsyncTasks.size();++i) {
		if (m_pAsyncTasks[i] != nullptr) {
			m_pAsyncTasks[i]->Stop();
			m_pAsyncTasks[i]->Join();
		}
	}
	m_pAsyncTasks.clear();
	return true;
}
// 连接配置服务器
bool CDBMysqlMgr::ConnectSyncDB()
{
	stDBConf& refConf = m_DBConfs[0];
	bool bRet = m_syncDBOper.dbOpen(refConf.sHost, refConf.sUser, refConf.sPwd, refConf.sDBName, refConf.uPort);
	if (bRet == false)
	{
		LOG_ERROR("connect config database fail :");
		return false;
	}
	LOG_DEBUG("connect config database successful ");
	return true;
}

void CDBMysqlMgr::AddAsyncSql(uint8_t dbType, string strSql)
{
	if (dbType >= m_pAsyncTasks.size())
	{
		LOG_ERROR("overstep dbIndexTypeMax:{}", dbType);
		return;
	}
	if (m_pAsyncTasks[dbType] != nullptr)
	{
		m_pAsyncTasks[dbType]->PushAndSelectDB("", strSql);
	}
	else
	{
		LOG_ERROR("m_pAsyncTask is null");
	}
}

CDBOperator& CDBMysqlMgr::GetSyncDBOper(uint8_t dbIndex)
{
	m_syncDBOper.dbSelect(GetDBName(dbIndex).c_str());
	return m_syncDBOper;
}

string CDBMysqlMgr::GetDBName(uint8_t dbType)
{
	if (dbType < m_DBConfs.size())
	{
		return m_DBConfs[dbType].sDBName;
	}
	return "";
}

// 添加DBEvent
void CDBMysqlMgr::AddAsyncDBEvent(uint8_t dbType, shared_ptr<CDBEventReq>& pReq)
{
	if(dbType >= m_pAsyncTasks.size()){
		LOG_ERROR("dbType is more than max index:{}",dbType);
		return;
	}
	if (m_pAsyncTasks[dbType] != nullptr)
	{
		m_pAsyncTasks[dbType]->AsyncQuery(pReq);
	}
	else
	{
		LOG_ERROR("m_pAsyncTask is null");
	}
}
string CDBMysqlMgr::GetDayGameTableName(int64_t time)
{
	return CStringUtility::FormatToString("daygame%s", time::date_format(time));
}

string CDBMysqlMgr::GetDayTranscTableName(int64_t time)
{
	return CStringUtility::FormatToString("daytrans%s", time::date_format(time));
}

void CDBMysqlMgr::AsyncLoadPlayerData(uint32_t uid,uint8_t dataType, std::function<void(shared_ptr<CDBEventRep>& pRep)> callBack)
{
	shared_ptr<CDBEventReq> pReq = m_pAsyncTasks[DB_INDEX_TYPE_ACC]->MallocDBEventReq();
	pReq->callBack = callBack;
	pReq->sqlStr = CStringUtility::FormatToString("SELECT %s as data,OfflineTime FROM t_player WHERE PlayerID =%u limit 1;",s_PlayerDataFields[dataType].c_str(),uid);
	AddAsyncDBEvent(DB_INDEX_TYPE_ACC, pReq);

}
// 保存玩家基础数据
void CDBMysqlMgr::SavePlayerDataInfo(uint32_t uid,uint8_t dataType,const string& data,uint32_t offlineTime)
{
	shared_ptr<CDBEventReq> pReq = m_pAsyncTasks[DB_INDEX_TYPE_ACC]->MallocDBEventReq();
	pReq->sqlStr = CStringUtility::FormatToString("UPDATE t_player SET `%s`=?,OfflineTime=? WHERE PlayerID=? limit 1;",s_PlayerDataFields[dataType].c_str());
	pReq->pushBlobParam(data.c_str(),data.length()).pushParam(offlineTime).pushParam(uid);
	AddAsyncDBEvent(DB_INDEX_TYPE_ACC, pReq);
}






