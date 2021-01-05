

#include "game_server_mgr.h"
#include "msg_define.pb.h"
#include "game_define.h"
#include "utility/profile_manager.h"
#include "player.h"
#include "player_mgr.h"

using namespace svrlib;
using namespace Network;

namespace {

}

//--------------------------------------------------------------------------------------------
CGameServerMgr::CGameServerMgr() {
    bind_handler(this, net::svr::GS2L_MSG_REPORT, &CGameServerMgr::handle_msg_report);
    bind_handler(this, net::svr::GS2L_MSG_LEAVE_SVR, &CGameServerMgr::handle_msg_leave_svr);
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

int CGameServerMgr::OnRecvClientMsg() {
    if (CProtobufHandleBase::OnRecvClientMsg() == 1) {
        return route_to_client();
    }
    return 0;
}

// 转发给客户端
int CGameServerMgr::route_to_client() {
    LOG_DEBUG("转发给客户端消息:uid:{}--cmd:{}", m_head->uin, m_head->msgID);
    auto pPlayer = GetPlayer();
    if (pPlayer != nullptr) {
        pPlayer->SendMsgToClient(m_pkt_buf, m_buf_len, m_head->msgID);
    } else {
        LOG_DEBUG("转发消息客户端不存在，通知游戏服断线:{}", m_head->uin);
        net::svr::msg_notify_net_state msg;
        msg.set_uid(m_head->uin);
        msg.set_state(0);
        msg.set_newip(0);
        msg.set_no_player(1);
        pkg_inner::SendProtobufMsg(m_connPtr, &msg, net::svr::L2GS_MSG_NOTIFY_NET_STATE, m_head->uin, 0, 0);
    }
    return 0;
}

// 服务器上报信息
int CGameServerMgr::handle_msg_report() {
    net::svr::msg_report_svr_info msg;
    PARSE_MSG(msg);

    uint32_t players = msg.onlines();

    //LOG_DEBUG("游戏服上报信息:sid {}--{}",_connPtr->GetUID(),players);

    return 0;
}

// 返回大厅
int CGameServerMgr::handle_msg_leave_svr() {
    net::svr::msg_leave_svr msg;
    PARSE_MSG(msg);

    uint32_t uid = msg.uid();
    LOG_DEBUG("通知返回大厅:{}", uid);
    auto pPlayer = GetPlayer();
    if (pPlayer != nullptr) {
        pPlayer->BackLobby();
        pPlayer->NotifyClientBackLobby(RESULT_CODE_SUCCESS, RESULT_CODE_SUCCESS);
    } else {
        LOG_DEBUG("返回大厅玩家不存在:{}", uid);
    }
    return 0;
}


std::shared_ptr<CPlayer> CGameServerMgr::GetPlayer() {
    auto pPlayer = std::dynamic_pointer_cast<CPlayer>(CPlayerMgr::Instance().GetPlayer(m_head->uin));
    return pPlayer;
}




















