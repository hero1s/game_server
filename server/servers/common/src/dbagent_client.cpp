
#include "dbagent_client.h"
#include "data_cfg_mgr.h"
#include "player_mgr.h"
#include "player_base.h"

using namespace std;
using namespace svrlib;

CDBAgentNetObj::CDBAgentNetObj()
{

}

CDBAgentNetObj::~CDBAgentNetObj()
{

}

void CDBAgentNetObj::ConnectorOnDisconnect()
{
	LOG_DEBUG("dbagent ondisconnect");
	CDBAgentClientMgr::Instance().OnCloseClient(this);
}

int CDBAgentNetObj::OnRecv(uint8_t* pMsg, uint16_t wSize)
{
	return CDBAgentClientMgr::Instance().OnHandleClientMsg(this, pMsg, wSize);
}

void CDBAgentNetObj::ConnectorOnConnect(bool bSuccess, uint32_t dwNetworkIndex)
{
	LOG_DEBUG("dbagent server OnConnect :{},{}", bSuccess, dwNetworkIndex);
	CDBAgentClientMgr::Instance().OnConnect(bSuccess, this);
}

//----------------------------------------------------------------------------------------------------------------------------

CDBAgentClientMgr::CDBAgentClientMgr()
		:m_timer(this)
{
	m_pNetObj    = NULL;
	m_isRun      = false;

	bind_handler(this, net::DBA2S_MSG_REGISTER_DBA_REP, &CDBAgentClientMgr::handle_msg_register_svr_rep);

}
CDBAgentClientMgr::~CDBAgentClientMgr()
{

}
void CDBAgentClientMgr::OnTimer()
{
	CApplication::Instance().schedule(&m_timer, 3000);
}

bool CDBAgentClientMgr::Init(int32_t ioKey,const net::svr::server_info& info, string ip,uint32_t port)
{
	IOCPServer& iocpServer = CApplication::Instance().GetIOCPServer();

	CDBAgentNetObj* pNetObj    = new CDBAgentNetObj();
	pNetObj->SetUID(info.svrid());
	pNetObj->Init(&iocpServer, ioKey,ip, port);

	m_curSvrInfo = info;
	m_pNetObj = pNetObj;
	m_isRun   = false;

	CApplication::Instance().schedule(&m_timer, 3000);

	return true;
}

void CDBAgentClientMgr::Register()
{
	net::svr::msg_register_dbagent_svr_req msg;
	net::svr::server_info* info = msg.mutable_info();
	*info = m_curSvrInfo;

	SendMsg2Center(&msg, net::S2DBA_MSG_REGISTER_DBA, 0);
	LOG_DEBUG("register dbagent server svrid:{} svrtype:{}--gameType:{}", msg.info().svrid(), msg.info().svr_type(), msg.info().game_type());
}
void CDBAgentClientMgr::RegisterRep(uint16_t svrid, bool rep)
{
	LOG_DEBUG("register dbagent server Rep svrid:{}--res:{}", svrid, rep);

	m_isRun = rep;
}
void CDBAgentClientMgr::OnConnect(bool bSuccess, CDBAgentNetObj* pNetObj)
{
	LOG_ERROR("dbagent on connect {},{}", bSuccess, pNetObj->GetUID());
	if (bSuccess)
	{
		m_isRun = true;
		Register();
	}
}

void CDBAgentClientMgr::OnCloseClient(CDBAgentNetObj* pNetObj)
{
	LOG_ERROR("dbagent OnClose:{}", pNetObj->GetUID());
	m_isRun = false;
}
bool CDBAgentClientMgr::IsRun()
{
	return m_isRun;
}
void CDBAgentClientMgr::SendMsg2Center(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin, uint8_t route, uint32_t routeID)
{
	if (!m_isRun || m_pNetObj == NULL)return;
	pkg_inner::SendProtobufMsg(m_pNetObj, msg, msg_type, uin, route, routeID);
}
//·þÎñÆ÷×¢²á
int CDBAgentClientMgr::handle_msg_register_svr_rep()
{
	net::svr::msg_register_dbagent_svr_rep msg;
	PARSE_MSG(msg);

	LOG_DEBUG("dbagent server register result :{}", msg.result());
	if (msg.result() == 1)
	{
		CDBAgentClientMgr::Instance().RegisterRep(_pNetObj->GetUID(), true);
	}
	else
	{
		CDBAgentClientMgr::Instance().RegisterRep(_pNetObj->GetUID(), false);
		LOG_ERROR("dbagent server register fail {} -->:{}", _pNetObj->GetUID(), CApplication::Instance().GetServerID());
	}
	return 0;
}










