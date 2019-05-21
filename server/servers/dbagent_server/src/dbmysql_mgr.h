
#pragma once

#include <string>
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

#include "utility/shm_cache_mgr.h"

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

	// �����������
	void AsyncLoadPlayerData(uint32_t uid, std::function<void(shared_ptr<CDBEventRep>& pRep)> callBack);
	// ������һ�������
	void SavePlayerBaseInfo(uint32_t uid,const string& data,uint32_t offlineTime);

	// ����첽SQL���
	void AddAsyncSql(uint8_t dbType, string strSql);
// ���ݿ�����ӿ�    
public:

// ͬ�����ݿ����
public:
	CDBOperator& GetSyncDBOper(uint8_t dbIndex);

protected:
	string GetDBName(uint8_t dbType);
	// ���DBEvent
	void AddAsyncDBEvent(uint8_t dbType, shared_ptr<CDBEventReq>& pReq);
public:

protected:
	string GetDayGameTableName(int64_t time);
	string GetDayTranscTableName(int64_t time);

private:
	// ������־�첽�߳�
	bool StartAsyncDB();
	// ֹͣ��־�첽�߳�
	bool StopAsyncDB();
	// �����������ݿ�
	bool ConnectSyncDB();

private:
	// ͬ�����ݿ����
	CDBOperator m_syncDBOper;                           // ͬ�����ݿ�
	// �첽���ݿ����
	CDBTask* m_pAsyncTask[DB_INDEX_TYPE_MAX];           // �첽���ݿ��߳�

	stDBConf m_DBConf[DB_INDEX_TYPE_MAX];               // ���ݿ�������Ϣ

	uint16_t                                             m_svrID;
	MemberTimerEvent<CDBMysqlMgr, &CDBMysqlMgr::OnTimer> m_reportTimer;

	SQLJoin m_sqlJoinData;
	SQLJoin m_sqlJoinWhere;
};

class CPlayerCacheMgr : public CDataCacheMgr<4096,60>, public AutoDeleteSingleton<CPlayerCacheMgr> {
public:
	CPlayerCacheMgr(){};
	virtual ~CPlayerCacheMgr(){};

};

