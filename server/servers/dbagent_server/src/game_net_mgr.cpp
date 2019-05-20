

#include "game_net_mgr.h"
#include "msg_define.pb.h"
#include "server_mgr.h"
#include "utility/profile_manager.h"

using namespace svrlib;
using namespace std;
using namespace Network;

// server ����
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
    AutoProfile ap("CenterNetObj::OnRecv");
	return CCenterMgr::Instance().OnHandleClientMsg(this, pMsg, wSize);
}

void CCenterNetObj::OnConnect(bool bSuccess, uint32_t dwNetworkIndex)
{
	LOG_DEBUG("center server OnConnect,{},{},{}", bSuccess, dwNetworkIndex,this->GetIP());
}
















