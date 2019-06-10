

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
    bind_handler(this, net::svr::S2CS_MSG_REGISTER, &CCenterMgr::handle_msg_register_svr);
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

// 更新服务器列表给全部服务器
void CCenterMgr::UpdateServerList()
{
    net::svr::msg_server_list_rep svrList;
    for (auto& it : m_mpServers) {
        auto pServer = it.second;
        net::svr::server_info* info = svrList.add_server_list();
        *info = pServer->m_info;
    }

    SendMsg2All(&svrList, net::svr::CS2S_MSG_SERVER_LIST_REP, 0);
}

int CCenterMgr::OnRecvClientMsg()
{
    m_msgMinCount++;
    if (_head->route>0) {
        return OnRouteDispMsg();
    }

    return CProtobufHandleBase::OnRecvClientMsg();
}

//路由分发消息
int CCenterMgr::OnRouteDispMsg()
{
    switch (_head->route) {
    case emROUTE_TYPE_ALL_GAME: {
        //LOG_DEBUG("转发全部游戏服{}", head->cmd);
        SendMsg2AllGameServer(_head->routeID, _pkt_buf, _buf_len, _head->cmd, _head->uin);
        break;
    }
    case emROUTE_TYPE_ONE_GAME: {
        //LOG_DEBUG("转发单个游戏服{}--{}--{}", head->cmd, head->routeMain, head->routeSub);
        SendMsg2Server(_head->routeID, _pkt_buf, _buf_len, _head->cmd, _head->uin);
        break;
    }
    default: {
        LOG_ERROR("route type error :{}", _head->route);
        break;
    }
    }
    return 0;
}

//服务器注册
int CCenterMgr::handle_msg_register_svr()
{
    net::svr::msg_register_svr_req msg;
    PARSE_MSG(msg);

    LOG_DEBUG("Server Register svrid:{}--svrType {}--gameType:{}--subType:{}", msg.info().svrid(),
            msg.info().svr_type(),
            msg.info().game_type(), msg.info().game_subtype());
    net::svr::msg_register_svr_rep repmsg;

    bool bRet = AddServer(_pNetObj, msg.info());
    if (!bRet) {
        LOG_ERROR("Register Server fail svrid:{}", msg.info().svrid());
    }
    repmsg.set_result(bRet);

    pkg_inner::SendProtobufMsg(_pNetObj, &repmsg, net::svr::CS2S_MSG_REGISTER_REP, 0,0,0);

    return 0;
}
























