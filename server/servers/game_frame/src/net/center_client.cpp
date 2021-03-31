
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
// ת�����ͻ���
int CCenterClientMgr::route_to_client()
{
    LOG_DEBUG("���ķ�ת�����ͻ�����Ϣ:uid:{}--cmd:{}",m_head->uid,m_head->msgID);
    auto pPlayer = GetPlayer();
    if (pPlayer != nullptr)
    {
        //pPlayer->SendMsgToClient(_pkt_buf, _buf_len, _head->cmd);
    }
    else
    {
        LOG_DEBUG("���ķ�ת����Ϣ�ͻ��˲�����:{}", m_head->uin);
    }
    return 0;
}

std::shared_ptr<CGamePlayer> CCenterClientMgr::GetPlayer(){
    auto pPlayer = std::dynamic_pointer_cast<CGamePlayer>(CPlayerMgr::Instance().GetPlayer(m_head->uid));
    return pPlayer;
}












