
#pragma once

#include <string>
#include "fundamental/noncopyable.h"
#include "svrlib.h"
#include "dbmysql/dbmysql.h"
#include <vector>
#include "db_operator.h"
#include "db_struct_define.h"
#include <queue>
#include "dbmysql/db_task.h"
#include "game_define.h"
#include "config/config.h"
#include <memory>

using namespace std;
using namespace svrlib;

class CDBMysqlMgr : public AutoDeleteSingleton<CDBMysqlMgr>
{
public:
	CDBMysqlMgr();

	~CDBMysqlMgr();

	void OnTimer();

	bool Init(stDBConf szDBConf[]);

	void ShutDown();

	// 加载玩家数据
	void AsyncLoadPlayerData(uint32_t uid, std::function<void(shared_ptr<CDBEventRep>& pRep)> callBack);
	// 保存玩家基础数据
	void SavePlayerBaseInfo(uint32_t uid,string& data,uint32_t offlineTime);


	// 添加异步SQL语句
	void AddAsyncSql(uint8_t dbType, string strSql);
// 数据库操作接口    
public:
	// 上报服务器在线人数
	void ReportOnlines();
	// 上报服务器启动
	void ReportStartServer(bool bReset);

// 同步数据库操作
public:
	CDBOperator& GetSyncDBOper(uint8_t dbIndex);

protected:
	// 调用通用sql
	void SendCommonLog(uint8_t dbType);
	string GetDBName(uint8_t dbType);
	// 添加DBEvent
	void AddAsyncDBEvent(uint8_t dbType, shared_ptr<CDBEventReq>& pReq);
public:

protected:
	string GetDayGameTableName(int64_t time);
	string GetDayTranscTableName(int64_t time);

private:
	// 启动日志异步线程
	bool StartAsyncDB();
	// 停止日志异步线程
	bool StopAsyncDB();
	// 连接配置数据库
	bool ConnectSyncDB();

private:
	// 同步数据库操作
	CDBOperator m_syncDBOper;                           // 同步数据库
	// 异步数据库操作
	CDBTask* m_pAsyncTask;                              // 异步数据库线程

	stDBConf m_DBConf[DB_INDEX_TYPE_MAX];               // 数据库配置信息

	uint16_t                                               m_svrID;
	string                                               m_strSql;
	MemberTimerEvent<CDBMysqlMgr, &CDBMysqlMgr::OnTimer> m_reportTimer;

	SQLJoin m_sqlJoinData;
	SQLJoin m_sqlJoinWhere;
};



