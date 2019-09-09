
#include "center_client.h"
#include "data_cfg_mgr.h"
#include "player_mgr.h"

using namespace std;
using namespace svrlib;

//----------------------------------------------------------------------------------------------------------------------------

CCenterClientMgr::CCenterClientMgr()
{

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
        //pPlayer->SendMsgToClient(_pkt_buf, _buf_len, _head->cmd);
    }
    else
    {
        LOG_DEBUG("中心服转发消息客户端不存在:{}", _head->uin);
    }
    return 0;
}

std::shared_ptr<CGamePlayer> CCenterClientMgr::GetPlayer(){
    auto pPlayer = std::dynamic_pointer_cast<CGamePlayer>(CPlayerMgr::Instance().GetPlayer(_head->uin));
    return pPlayer;
}












