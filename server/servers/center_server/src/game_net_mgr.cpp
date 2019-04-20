

#include "game_net_mgr.h"
#include "msg_define.pb.h"
#include "center_mgr.h"

using namespace svrlib;
using namespace std;
using namespace Network;

// server Á¬½Ó
CCenterNetObj::CCenterNetObj()
{

}

CCenterNetObj::~CCenterNetObj()
{

}
void CCenterNetObj::OnDisconnect()
{
	LOG_ERROR("center server on disconnect:{}--{}", GetUID(), this->GetIP());
	CCenterMgr::Instance().RemoveServer(this);
}

int CCenterNetObj::OnRecv(uint8_t* pMsg, uint16_t wSize)
{
	return CCenterMgr::Instance().OnHandleClientMsg(this, pMsg, wSize);
}

void CCenterNetObj::OnConnect(bool bSuccess, uint32_t dwNetworkIndex)
{
	LOG_DEBUG("center server OnConnect,{},{},{}", bSuccess, dwNetworkIndex,this->GetIP());
}
















