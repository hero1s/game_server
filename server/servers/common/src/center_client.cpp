
#include "center_client.h"
#include "data_cfg_mgr.h"
#include "player_mgr.h"
#include "player_base.h"

using namespace std;
using namespace svrlib;

//----------------------------------------------------------------------------------------------------------------------------

CCenterClientMgr::CCenterClientMgr()
:CSvrConnectorMgr(net::svr::S2CS_MSG_REGISTER_CENTER)
{
    bind_handler(this, net::svr::CS2S_MSG_REGISTER_CENTER_REP, &CCenterClientMgr::handle_msg_register_svr_rep);
    bind_handler(this, net::svr::CS2S_MSG_SERVER_LIST_REP, &CCenterClientMgr::handle_msg_server_list_rep);
}

CCenterClientMgr::~CCenterClientMgr() {

}

//服务器注册
int CCenterClientMgr::handle_msg_register_svr_rep() {
    net::svr::msg_register_center_svr_rep msg;
    PARSE_MSG(msg);

    LOG_DEBUG("center server register result :{}", msg.result());
    if (msg.result() == 1) {
        CCenterClientMgr::Instance().RegisterRep(_pNetObj->GetUID(), true);
    } else {
        CCenterClientMgr::Instance().RegisterRep(_pNetObj->GetUID(), false);
        LOG_ERROR("center server register fail {} -->:{}", _pNetObj->GetUID(), CApplication::Instance().GetServerID());
    }
    return 0;
}

//更新服务器列表
int CCenterClientMgr::handle_msg_server_list_rep() {
    net::svr::msg_server_list_rep msg;
    PARSE_MSG(msg);

    LOG_DEBUG("center server rep svrlist :{}", msg.server_list_size());


    return 0;

}









