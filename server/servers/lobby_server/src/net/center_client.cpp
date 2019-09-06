
#include "center_client.h"
#include "data_cfg_mgr.h"
#include "player_mgr.h"
#include "player_base.h"

using namespace std;
using namespace svrlib;

//----------------------------------------------------------------------------------------------------------------------------

CCenterClientMgr::CCenterClientMgr()
{
    bind_handler(this, net::svr::GS2L_MSG_NOTIFY_PLAYER_LOBBY_LOGIN, &CCenterClientMgr::handle_msg_player_login_lobby);

}

CCenterClientMgr::~CCenterClientMgr() {

}
int CCenterClientMgr::OnRecvClientMsg()
{
    if (CProtobufHandleBase::OnRecvClientMsg()==1)
    {
        return route_to_client();
    }
    return 0;
}
// 转发给客户端
int CCenterClientMgr::route_to_client()
{
    LOG_DEBUG("中心服转发给客户端消息:uid:{}--cmd:{}",_head->uin,_head->cmd);
    auto pPlayer = GetPlayer();
    if (pPlayer != nullptr)
    {
        pPlayer->SendMsgToClient(_pkt_buf, _buf_len, _head->cmd);
    }
    else
    {
        LOG_DEBUG("中心服转发消息客户端不存在:{}", _head->uin);
    }
    return 0;
}
// 玩家登录通知
int CCenterClientMgr::handle_msg_player_login_lobby()
{
    net::svr::msg_notify_player_lobby_login loginmsg;
    PARSE_MSG(loginmsg);

    LOG_DEBUG("登录通知:lobby:{}--uid:{}", loginmsg.lobby_id(), loginmsg.uid());
    auto pPlayer = std::dynamic_pointer_cast<CPlayer>(CPlayerMgr::Instance().GetPlayer(loginmsg.uid()));
    if (pPlayer != nullptr && loginmsg.lobby_id() != CApplication::Instance().GetServerID())
    {
        LOG_DEBUG("玩家已经别的大厅服登录，回收玩家:{}", loginmsg.uid());
        pPlayer->SetNeedRecover(true);
        net::cli::msg_loginout_rep loginoutmsg;
        loginoutmsg.set_reason(net::RESULT_CODE_LOGIN_OTHER);
        pPlayer->SendMsgToClient(&loginoutmsg, net::S2C_MSG_LOGINOUT_REP);
    }
    return 0;
}



std::shared_ptr<CPlayer> CCenterClientMgr::GetPlayer(){
    auto pPlayer = std::dynamic_pointer_cast<CPlayer>(CPlayerMgr::Instance().GetPlayer(_head->uin));
    return pPlayer;
}










