

#include "game_server_mgr.h"
#include "msg_define.pb.h"
#include "game_define.h"
#include "utility/profile_manager.h"

using namespace svrlib;
using namespace Network;

namespace {

}

//--------------------------------------------------------------------------------------------
CGameServerMgr::CGameServerMgr() {
    bind_handler(this,net::svr::GS2L_MSG_REPORT,&CGameServerMgr::handle_msg_report);

}

CGameServerMgr::~CGameServerMgr() {
}

bool CGameServerMgr::Init() {
    CServerClientMgr::Init();
    return true;
}

void CGameServerMgr::ShutDown() {
    CServerClientMgr::ShutDown();
}

// 服务器上报信息
int CGameServerMgr::handle_msg_report()
{
    net::svr::msg_report_svr_info msg;
    PARSE_MSG(msg);

    uint32_t players = msg.onlines();

    LOG_DEBUG("游戏服上报信息:sid {}--{}",_connPtr->GetUID(),players);

    return 0;
}
























