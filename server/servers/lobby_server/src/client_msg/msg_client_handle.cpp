

#include "msg_client_handle.h"
#include "data_cfg_mgr.h"
#include "game_server_config.h"
#include "player.h"
#include "player_mgr.h"
#include "common_logic.h"

using namespace Network;
using namespace svrlib;
using namespace std;

namespace
{

};
CHandleClientMsg::CHandleClientMsg()
{
	bind_handler(this, net::C2S_MSG_HEART, &CHandleClientMsg::handle_msg_heart);
	bind_handler(this, net::C2S_MSG_LOGIN, &CHandleClientMsg::handle_msg_login);

}
CHandleClientMsg::~CHandleClientMsg()
{

}

#ifndef CHECK_PLAYER_PLAY
#define CHECK_PLAYER_PLAY   \
    CPlayer* pPlayer = GetPlayer(pNetObj);\
    if(pPlayer == NULL || !pPlayer->IsPlaying())\
        return -1;
#endif

// 心跳包
int CHandleClientMsg::handle_msg_heart()
{
	net::cli::msg_heart_test msg;
	msg.set_svr_time(time::getSysTime());
	pkg_client::SendProtobufMsg(_connPtr, &msg, net::C2S_MSG_HEART, 0);
	return 0;
}

//登录
int CHandleClientMsg::handle_msg_login()
{
	net::cli::msg_login_req msg;
	PARSE_MSG(msg);

	LOG_ERROR("recv player login msg:uid:{}-deviceid:{}-key:{}", msg.uid(), msg.deviceid(), msg.key());
	uint32_t             uid = msg.uid();
	net::cli::msg_login_rep repmsg;
	repmsg.set_server_time(time::getSysTime());

	string strDecyPHP = msg.key();
	auto pPlayerObj = std::dynamic_pointer_cast<CPlayer>(CPlayerMgr::Instance().GetPlayer(_connPtr->GetUID()));
	auto pPlayerUid = std::dynamic_pointer_cast<CPlayer>(CPlayerMgr::Instance().GetPlayer(uid));

	// 校验密码
	if (pPlayerUid == NULL || pPlayerUid->GetLoginKey() != strDecyPHP)
	{
		string strDecy = CCommonLogic::VerifyPasswd(uid, msg.check_time());
		if (strDecy != strDecyPHP || std::abs(int64_t(time::getSysTime()) - int64_t(msg.check_time())) > DAY)
		{
			LOG_ERROR("check passwd error {}-PHP:{}--c++:{}", uid, strDecyPHP, strDecy);
			LOG_ERROR("the ip is:{},svrtime:{},sendtime:{}", _connPtr->GetRemoteAddress(), time::getSysTime(), msg.check_time());
			repmsg.set_result(-1);
			pkg_client::SendProtobufMsg(_connPtr, &repmsg, net::S2C_MSG_LOGIN_REP, 0);
			return -1;
		}
	}

	//维护状态，只有在玩玩家可以进
	if (CApplication::Instance().GetStatus() == emSERVER_STATE_REPAIR)
	{
		if (pPlayerUid == NULL)
		{
			repmsg.set_result(0);
			pkg_client::SendProtobufMsg(_connPtr, &repmsg, net::S2C_MSG_LOGIN_REP, 0);
			LOG_ERROR("服务器维护状态，只有在玩玩家能进入");
			_connPtr->Close();
			return 0;
		}
	}

	if (pPlayerObj != NULL)
	{
		LOG_ERROR("more send login msg");
		if (pPlayerObj->GetPlayerState() == PLAYER_STATE_LOAD_DATA)
		{
			pPlayerObj->OnLogin();
		}
		else
		{
			pPlayerObj->NotifyEnterGame();
			pPlayerObj->SendAllPlayerData2Client();
		}
		pPlayerObj->SetLoginKey(strDecyPHP);
		return 0;
	}
	else
	{
		if (pPlayerUid != NULL)// 在玩
		{
			auto pOldSock = pPlayerUid->GetSession();
			uint32_t diffTime = time::getSysTime() - pPlayerUid->GetReloginTime();
			LOG_ERROR("remove old player :{} oldsock {},difftime {}", uid, pOldSock->GetRemoteAddress(), diffTime);
			if (diffTime < 1)
			{
				LOG_ERROR("登录挤号过于频繁:{}", pPlayerUid->GetUID());
				return -1;
			}
			if (pOldSock != NULL)
			{
				pPlayerUid->NotifyLoginOut(0, msg.deviceid());
				pOldSock->SetUID(0);
				pOldSock->Close();
			}

			_connPtr->SetUID(uid);
			pPlayerUid->SetSession(_connPtr);
			pPlayerUid->SetLoginKey(strDecyPHP);

			if (pPlayerUid->GetPlayerState() == PLAYER_STATE_PLAYING)
			{
				pPlayerUid->ReLogin();
			}
			else
			{
				pPlayerUid->OnLogin();
			}

			return 0;
		}
	}
	auto pPlayer = std::make_shared<CPlayer>(PLAYER_TYPE_ONLINE);
	_connPtr->SetUID(uid);
	pPlayer->SetSession(_connPtr);
	pPlayer->SetUID(uid);
	CPlayerMgr::Instance().AddPlayer(pPlayer);
	pPlayer->OnLogin();
	pPlayer->SetLoginKey(strDecyPHP);

	return 0;
}

std::shared_ptr<CPlayer> CHandleClientMsg::GetPlayer(const TCPConnPtr& connPtr)
{
	auto pPlayer = std::dynamic_pointer_cast<CPlayer>(CPlayerMgr::Instance().GetPlayer(connPtr->GetUID()));
	if (pPlayer == NULL || !pPlayer->IsPlaying())
	{
		LOG_DEBUG("玩家不存在，或者玩家不在在线状态:{}", connPtr->GetUID());
		return NULL;
	}
	return pPlayer;
}



















