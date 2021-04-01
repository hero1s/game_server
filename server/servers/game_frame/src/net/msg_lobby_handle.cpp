
#include "msg_lobby_handle.h"
#include "game_define.h"
#include "game_player.h"
#include "msg_define.pb.h"
#include "net/center_client.h"
#include "player_mgr.h"
#include "servers_msg.pb.h"
#include <data_cfg_mgr.h>

using namespace Network;
using namespace svrlib;
using namespace net;

CHandleLobbyMsg::CHandleLobbyMsg()
{
    bind_handler(this, net::svr::L2GS_MSG_NOTIFY_NET_STATE, &CHandleLobbyMsg::handle_msg_notify_net_state);
    bind_handler(this, net::svr::L2GS_MSG_ENTER_INTO_SVR, &CHandleLobbyMsg::handle_msg_enter_svr);
    bind_handler(this, net::C2S_MSG_BACK_LOBBY, &CHandleLobbyMsg::handle_msg_back_lobby);
}
CHandleLobbyMsg::~CHandleLobbyMsg()
{
}

int CHandleLobbyMsg::OnRecvClientMsg()
{
    LOG_DEBUG("收到大厅服务器消息:uin:{}--cmd:{}", m_head->uid, m_head->msgID);
    if (CProtobufHandleBase::OnRecvClientMsg() == 1) {
        return handle_msg_gameing_oper();
    }
    return 0;
}

// 游戏内消息
int CHandleLobbyMsg::handle_msg_gameing_oper()
{
    LOG_DEBUG("游戏内部消息:uid:{}--msg:{}", m_head->uid, m_head->msgID);
    auto pGamePlayer = GetGamePlayer();
    if (pGamePlayer == nullptr) {
        return 0;
    }
    pGamePlayer->OnMessage(m_head->msgID, m_pkt_buf, m_buf_len);

    return 0;
}

// 通知网络状态
int CHandleLobbyMsg::handle_msg_notify_net_state()
{
    net::svr::msg_notify_net_state msg;
    PARSE_MSG(msg);
    uint32_t uid = msg.uid();
    uint8_t state = msg.state();
    uint32_t loginIp = msg.newip();
    uint8_t noPlayer = msg.no_player();
    LOG_DEBUG("通知网络状态:{}-->{}-->noplayer:{}", uid, state, noPlayer);
    auto pGamePlayer = GetGamePlayer();
    if (pGamePlayer != nullptr) {
        if (noPlayer == 1) {
            //pGamePlayer->SetNetStateNoPlayer();
        } else {
            pGamePlayer->SetIP(loginIp);
            pGamePlayer->SetNetState(state);
        }
    }
    return 0;
}

// 进入游戏服务器
int CHandleLobbyMsg::handle_msg_enter_svr()
{
    net::svr::msg_enter_into_game_svr msg;
    PARSE_MSG(msg);
    uint32_t uid = m_head->uid;
    LOG_DEBUG("进入游戏服务器:{}", uid);

    auto pGamePlayer = GetGamePlayer();
    if (pGamePlayer != nullptr) {
        pGamePlayer->ReLogin();
    } else {
        if (msg.player_type() == PLAYER_TYPE_ONLINE) {
            pGamePlayer = std::make_shared<CGamePlayer>(PLAYER_TYPE_ONLINE);
            pGamePlayer->SetPlayerGameData(msg);
            pGamePlayer->OnLogin();
            CPlayerMgr::Instance().AddPlayer(pGamePlayer);
        } else {
            LOG_ERROR("error player type :{}", msg.player_type());
            return 0;
        }
    }
    pGamePlayer->SetLoginLobbySvrID(m_connPtr->GetUID());

    net::cli::msg_enter_gamesvr_rep msgrep;
    msgrep.set_result(RESULT_CODE_SUCCESS);
    msgrep.set_svrid(CApplication::Instance().GetServerID());

    pGamePlayer->SendMsgToClient(&msgrep, net::S2C_MSG_ENTER_SVR_REP);

    //测试中心服转发消息给指定玩家 test toney
    CCenterClientMgr::Instance().SendMsg2Svr(&msgrep, net::S2C_MSG_ENTER_SVR_REP, uid, 0, 0, emSERVER_TYPE_LOBBY, 0);

    return 0;
}

// 返回大厅
int CHandleLobbyMsg::handle_msg_back_lobby()
{
    net::cli::msg_back_lobby_req msg;
    PARSE_MSG(msg);
    uint32_t uid = m_head->uid;
    LOG_DEBUG("请求返回大厅:{} -- {}", uid, msg.is_action());
    auto pGamePlayer = GetGamePlayer();
    if (pGamePlayer != nullptr) {
        if (!pGamePlayer->CanBackLobby()) {
            net::cli::msg_back_lobby_rep rep;
            rep.set_result(RESULT_CODE_FAIL);
            rep.set_reason(RESULT_CODE_GAMEING);
            pGamePlayer->SendMsgToClient(&rep, net::S2C_MSG_BACK_LOBBY_REP);
            LOG_DEBUG("游戏状态中不能返回大厅");
            return 0;
        } else {
            CPlayerMgr::Instance().RecoverPlayer(pGamePlayer);
        }
        return 0;
    } else {
        if (msg.is_action() > 0) {
            LOG_ERROR("服务器主动请求返回大厅:{}");
            net::svr::msg_leave_svr msg;
            msg.set_uid(uid);
            ReplyMsg(&msg, net::svr::GS2L_MSG_LEAVE_SVR, uid);
        }
    }

    LOG_DEBUG("发送返回大厅消息:{}", uid);
    net::cli::msg_back_lobby_rep rep;
    rep.set_result(RESULT_CODE_SUCCESS);
    rep.set_reason(RESULT_CODE_SUCCESS);
    ReplyMsg(&rep, net::S2C_MSG_BACK_LOBBY_REP, uid);
    return 0;
}

std::shared_ptr<CGamePlayer> CHandleLobbyMsg::GetGamePlayer()
{
    auto pPlayer = std::dynamic_pointer_cast<CGamePlayer>(CPlayerMgr::Instance().GetPlayer(m_head->uid));
    if (pPlayer == nullptr) {
        LOG_DEBUG("游戏玩家不存在:{}", m_head->uid);
    } else {
        pPlayer->ResetHeart();
    }
    return pPlayer;
}

void CHandleLobbyMsg::ReplyMsg(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin)
{
    pkg_inner::SendProtobufMsg(m_connPtr, msg, msg_type, uin, 0, 0, 0, 0);
}
