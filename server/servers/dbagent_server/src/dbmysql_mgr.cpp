

#include "dbmysql_mgr.h"
#include "svrlib.h"
#include <string.h>
#include <svrlib.h>
#include "common_logic.h"
#include "player_mgr.h"
#include "data_cfg_mgr.h"
#include "game_server_config.h"
#include "center_client.h"
#include "dbmysql/db_wrap.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

using namespace std;
using namespace svrlib;

namespace
{

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
	ReportOnlines();
	CApplication::Instance().schedule(&m_reportTimer, 20*1000);
}

bool CDBMysqlMgr::Init(stDBConf szDBConf[])
{
	m_svrID = CApplication::Instance().GetServerID();

	for (int32_t i = 0; i < DB_INDEX_TYPE_MAX; ++i)
	{
		m_DBConf[i] = szDBConf[i];
	}
	if (!ConnectSyncDB())
	{
		return false;
	}
	StartAsyncDB();

	CApplication::Instance().schedule(&m_reportTimer, 5*1000);

	return true;
}

void CDBMysqlMgr::ShutDown()
{
	StopAsyncDB();
	m_syncDBOper.dbClose();
	m_reportTimer.cancel();
}

// �����첽�߳�
bool CDBMysqlMgr::StartAsyncDB()
{
	for(uint16_t i=0;i<DB_INDEX_TYPE_MAX;++i) {
		m_pAsyncTask[i] = new CDBTask(CApplication::Instance().GetAsioContext());
		m_pAsyncTask[i]->SetDBName(m_DBConf[i].sDBName);
		m_pAsyncTask[i]->Init(m_DBConf[i]);
		m_pAsyncTask[i]->Start();
	}
	return true;
}
// ֹͣ��־�첽�߳�
bool CDBMysqlMgr::StopAsyncDB()
{
	for(uint16_t i=0;i<DB_INDEX_TYPE_MAX;++i) {
		if (m_pAsyncTask[i] != NULL) {
			m_pAsyncTask[i]->Stop();
			m_pAsyncTask[i]->Join();

			delete m_pAsyncTask[i];
			m_pAsyncTask[i] = NULL;
		}
	}
	return true;
}
// �������÷�����
bool CDBMysqlMgr::ConnectSyncDB()
{
	stDBConf& refConf = m_DBConf[0];
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
	if (dbType >= DB_INDEX_TYPE_MAX)
	{
		LOG_ERROR("overstep dbIndexTypeMax:{}", dbType);
		return;
	}
	if (m_pAsyncTask[dbType] != NULL)
	{
		m_pAsyncTask[dbType]->PushAndSelectDB("", strSql);
	}
	else
	{
		LOG_ERROR("m_pAsyncTask is null");
	}
}

// �ϱ���������������
void CDBMysqlMgr::ReportOnlines()
{
	uint32_t onlines = CPlayerMgr::Instance().GetOnlines();
	m_strSql = CStringUtility::FormatToString("update serverinfo set onlines=%d,report_time=%lld,repair_state=%d where svrid=%d", onlines,
	                                          getSysTime(), CApplication::Instance().GetStatus(), m_svrID);
	SendCommonLog(DB_INDEX_TYPE_CFG);
}
// �ϱ�����������
void CDBMysqlMgr::ReportStartServer(bool bReset)
{
	if (bReset)
	{
		m_strSql = CStringUtility::FormatToString("update serverinfo set start_count=1,start_time=%lld where svrid=%d", getSysTime(), m_svrID);
	}
	else
	{
		m_strSql = CStringUtility::FormatToString("update serverinfo set start_count=start_count+1,start_time=%lld where svrid=%d", getSysTime(), m_svrID);
	}
	SendCommonLog(DB_INDEX_TYPE_CFG);
}
CDBOperator& CDBMysqlMgr::GetSyncDBOper(uint8_t dbIndex)
{
	m_syncDBOper.dbSelect(GetDBName(dbIndex).c_str());
	return m_syncDBOper;
}

// ����ͨ��sql
void CDBMysqlMgr::SendCommonLog(uint8_t dbType)
{
	AddAsyncSql(dbType, m_strSql);
}

string CDBMysqlMgr::GetDBName(uint8_t dbType)
{
	if (dbType < DB_INDEX_TYPE_MAX)
	{
		return m_DBConf[dbType].sDBName;
	}
	return "";
}

// ���DBEvent
void CDBMysqlMgr::AddAsyncDBEvent(uint8_t dbType, shared_ptr<CDBEventReq>& pReq)
{
	if(dbType >= DB_INDEX_TYPE_MAX){
		LOG_ERROR("dbType is more than max index:{}",dbType);
		return;
	}
	if (m_pAsyncTask[dbType] != NULL)
	{
		m_pAsyncTask[dbType]->AsyncQuery(pReq);
	}
	else
	{
		LOG_ERROR("m_pAsyncTask is null");
	}
}
string CDBMysqlMgr::GetDayGameTableName(int64_t time)
{
	return CStringUtility::FormatToString("daygame%s", date_format(time));
}

string CDBMysqlMgr::GetDayTranscTableName(int64_t time)
{
	return CStringUtility::FormatToString("daytrans%s", date_format(time));
}

void CDBMysqlMgr::AsyncLoadPlayerData(uint32_t uid, std::function<void(shared_ptr<CDBEventRep>& pRep)> callBack)
{
	shared_ptr<CDBEventReq> pReq = m_pAsyncTask[DB_INDEX_TYPE_ACC]->MallocDBEventReq();
	pReq->callBack = callBack;
	pReq->sqlStr = CStringUtility::FormatToString("SELECT Base,OfflineTime FROM t_player WHERE PlayerID =%u limit 1;", uid);
	AddAsyncDBEvent(DB_INDEX_TYPE_ACC, pReq);

}
// ������һ�������
void CDBMysqlMgr::SavePlayerBaseInfo(uint32_t uid,const string& data,uint32_t offlineTime)
{
	shared_ptr<CDBEventReq> pReq = m_pAsyncTask[DB_INDEX_TYPE_ACC]->MallocDBEventReq();
	pReq->sqlStr = CStringUtility::FormatToString("UPDATE t_player SET `Base`=?,OfflineTime=? WHERE PlayerID=? limit 1;",offlineTime, uid);
	pReq->pushBlobParam(data.c_str(),data.length()).pushParam(offlineTime).pushParam(uid);
	AddAsyncDBEvent(DB_INDEX_TYPE_ACC, pReq);
}






