

#include "game_net_mgr.h"
#include "msg_define.pb.h"
#include "server_mgr.h"
#include "utility/profile_manager.h"

using namespace svrlib;
using namespace std;
using namespace Network;

// server Á¬½Ó
CServerNetObj::CServerNetObj()
{

}

CServerNetObj::~CServerNetObj()
{

}
void CServerNetObj::OnDisconnect()
{
	LOG_ERROR("center server on disconnect:{}--{}", GetUID(), this->GetIP());
	CServerMgr::Instance().RemoveServer(this);
}

int CServerNetObj::OnRecv(uint8_t* pMsg, uint16_t wSize)
{
    AutoProfile ap("CenterNetObj::OnRecv");
	return CServerMgr::Instance().OnHandleClientMsg(this, pMsg, wSize);
}

void CServerNetObj::OnConnect(bool bSuccess, uint32_t dwNetworkIndex)
{
	LOG_DEBUG("center server OnConnect,{},{},{}", bSuccess, dwNetworkIndex,this->GetIP());
}
















