

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

// ������
int CHandleClientMsg::handle_msg_heart()
{
	net::msg_heart_test msg;
	msg.set_svr_time(getSysTime());
	SendProtobufMsg(_pNetObj, &msg, net::C2S_MSG_HEART, 0);
	return 0;
}

//��¼
int CHandleClientMsg::handle_msg_login()
{
	net::msg_login_req msg;
	PARSE_MSG(msg);

	LOG_ERROR("recv player login msg:uid:{}-deviceid:{}-key:{}", msg.uid(), msg.deviceid(), msg.key());
	uint32_t             uid = msg.uid();
	net::msg_login_rep repmsg;
	repmsg.set_server_time(getSysTime());

	string strDecyPHP = msg.key();
	CPlayer* pPlayerObj = (CPlayer*) CPlayerMgr::Instance().GetPlayer(_pNetObj->GetUID());
	CPlayer* pPlayerUid = (CPlayer*) CPlayerMgr::Instance().GetPlayer(uid);

	// У������
	if (pPlayerUid == NULL || pPlayerUid->GetLoginKey() != strDecyPHP)
	{
		string strDecy = CCommonLogic::VerifyPasswd(uid, msg.check_time());
		if (strDecy != strDecyPHP || std::abs(int64_t(getSysTime()) - int64_t(msg.check_time())) > SECONDS_IN_ONE_DAY)
		{
			LOG_ERROR("check passwd error {}-PHP:{}--c++:{}", uid, strDecyPHP, strDecy);
			LOG_ERROR("the ip is:{},svrtime:{},sendtime:{}", _pNetObj->GetIP(), getSysTime(), msg.check_time());
			repmsg.set_result(-1);
			SendProtobufMsg(_pNetObj, &repmsg, net::S2C_MSG_LOGIN_REP, 0);
			return -1;
		}
	}

	//ά��״̬��ֻ��������ҿ��Խ�
	if (CApplication::Instance().GetStatus() == emSERVER_STATE_REPAIR)
	{
		if (pPlayerUid == NULL)
		{
			repmsg.set_result(0);
			SendProtobufMsg(_pNetObj, &repmsg, net::S2C_MSG_LOGIN_REP, 0);
			LOG_ERROR("������ά��״̬��ֻ����������ܽ���");
			_pNetObj->Disconnect(false);
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
		if (pPlayerUid != NULL)// ����
		{
			NetworkObject* pOldSock = pPlayerUid->GetSession();
			uint32_t diffTime = getSysTime() - pPlayerUid->GetReloginTime();
			LOG_ERROR("remove old player :{} oldsock {},difftime {}", uid, (void*) pOldSock, diffTime);
			if (diffTime < 1)
			{
				LOG_ERROR("��¼���Ź���Ƶ��:{}", pPlayerUid->GetUID());
				return -1;
			}
			if (pOldSock != NULL)
			{
				pPlayerUid->NotifyLoginOut(0, msg.deviceid());
				pOldSock->SetUID(0);
				pOldSock->Disconnect(false);
			}

			_pNetObj->SetUID(uid);
			pPlayerUid->SetSession(_pNetObj);
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
	CPlayer* pPlayer = new CPlayer(PLAYER_TYPE_ONLINE);
	_pNetObj->SetUID(uid);
	pPlayer->SetSession(_pNetObj);
	pPlayer->SetUID(uid);
	CPlayerMgr::Instance().AddPlayer(pPlayer);
	pPlayer->OnLogin();
	pPlayer->SetLoginKey(strDecyPHP);

	return 0;
}

CPlayer* CHandleClientMsg::GetPlayer(NetworkObject* pNetObj)
{
	CPlayer* pPlayer = (CPlayer*) CPlayerMgr::Instance().GetPlayer(pNetObj->GetUID());
	if (pPlayer == NULL || !pPlayer->IsPlaying())
	{
		LOG_DEBUG("��Ҳ����ڣ�������Ҳ�������״̬:{}", pNetObj->GetUID());
		return NULL;
	}
	return pPlayer;
}



















