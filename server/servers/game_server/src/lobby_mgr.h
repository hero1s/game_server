
#pragma once

#include "fundamental/noncopyable.h"
#include "svrlib.h"
#include <string.h>
#include "network/NetworkObject.h"
#include "packet/inner_protobuf_pkg.h"
#include "game_net_mgr.h"
#include <unordered_map>

using namespace std;
using namespace svrlib;
using namespace Network;

/**
 * 管理大厅服务器
 */
struct stLobbyServer
{
  uint16_t svrID;
  CLobbyNetObj* pNetObj;
  bool isRun;

  stLobbyServer()
  {
	  svrID   = 0;
	  pNetObj = 0;
	  isRun   = false;
  }
};

class CLobbyMgr : public AutoDeleteSingleton<CLobbyMgr>
{
public:
	CLobbyMgr();

	void OnTimer();

	bool Init();

	void Register(uint16_t svrid);

	void RegisterRep(uint16_t svrid, bool rep);
	// 发送消息到客户端
	bool SendMsg2Client(const google::protobuf::Message* msg, uint16_t msg_type, uint32_t uin, uint32_t svrid);
	// 发送消息到大厅
	bool SendMsg2Lobby(const google::protobuf::Message* msg, uint16_t msg_type, uint16_t svrid, uint8_t encrypt = 0);
	// 发送消息到全部大厅
	bool SendMsg2AllLobby(const google::protobuf::Message* msg, uint16_t msg_type, uint8_t encrypt = 0);
	// 大厅连接回调
	void OnConnect(bool bSuccess, CLobbyNetObj* pNetObj);
	// 大厅断开
	void OnCloseClient(CLobbyNetObj* pNetObj);

	// 通知大厅修改数值
	void NotifyLobbyChangeAccValue(uint32_t uid, int32_t operType, int32_t subType, int64_t coin, int64_t safeCoin, const string& chessid = "");

	// 通知房卡更新
	void FlushRoomCardValueToLobby(uint32_t uid, int64_t roomCard);
	// 刷新俱乐部房卡
	void FlushClubInfo(uint32_t uid, int64_t clubID, int64_t roomCard, int64_t forze);

	// 变更房卡
	bool AtomChangePlayerRoomCard(uint32_t uid, uint16_t operType, uint16_t subType, int64_t roomCard);
	// 冻结房卡
	bool FrozenPlayerRoomCard(uint32_t uid, int64_t clubID, int64_t roomCard);
	bool UnfrozenPlayerRoomCard(uint32_t uid, int64_t clubID, int64_t roomCard);
	bool MoveFrozenPlayerRoomCard(uint32_t uid, int64_t clubID, uint16_t operType, uint16_t subType, int64_t roomCard);

private:
	stLobbyServer* GetLobbySvrBySvrID(uint16_t svrid);

	stLobbyServer* GetLobbySvrByUin(uint32_t uin);

	void ReportInfo2Lobby();

private:
	using MAP_LOBBY = unordered_map<uint32_t, stLobbyServer>;
	MAP_LOBBY                                        m_lobbySvrs;// 大厅服务器
	MemberTimerEvent<CLobbyMgr, &CLobbyMgr::OnTimer> m_timer;

};










