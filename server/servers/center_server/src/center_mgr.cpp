

#include "center_mgr.h"
#include "msg_define.pb.h"
#include "game_define.h"
#include "utility/profile_manager.h"

using namespace svrlib;
using namespace Network;

namespace {

}

//--------------------------------------------------------------------------------------------
CCenterMgr::CCenterMgr()
{

}

CCenterMgr::~CCenterMgr()
{
}

void CCenterMgr::OnTimer()
{
    CServerClientMgr::OnTimer();
    UpdateServerList();//test
}

bool CCenterMgr::Init()
{
    CServerClientMgr::Init();

    return true;
}

void CCenterMgr::ShutDown()
{

}

























