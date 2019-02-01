
#pragma once

#include "svrlib.h"
#include "db_struct_define.h"
#include "msg_define.pb.h"
#include<bitset>

using namespace svrlib;
using namespace std;
using namespace Network;

enum PLAYER_TYPE
{
  PLAYER_TYPE_ONLINE = 0,        // 在线玩家
  PLAYER_TYPE_ROBOT,             // 机器人
};
enum PLAYER_STATE
{
  PLAYER_STATE_NULL,              // 空状态
  PLAYER_STATE_LOAD_DATA,         // 拉取数据
  PLAYER_STATE_PLAYING,           // 游戏状态
  PLAYER_STATE_LOGINOUT,          // 下线
};

class CPlayerBase
{
public:
	CPlayerBase(uint8_t type);

	virtual ~CPlayerBase();

	bool IsLoadData(uint8_t dataType);
	void SetLoadState(uint8_t dataType);
	//是否加载完成
	bool IsLoadOver();
	//玩家状态
	void SetPlayerState(uint8_t state);

	uint8_t GetPlayerState();

	bool IsPlaying();

	uint32_t GetUID();
	void SetUID(uint32_t uid);

	uint8_t GetPlayerType();
	string GetPlayerName();
	uint8_t GetSex();
	void SetSession(NetworkObject* pSession);
	NetworkObject* GetSession();
	void SetIP(uint32_t ip);
	uint32_t GetIP();
	string GetIPStr();

	virtual bool SendMsgToClient(const google::protobuf::Message* msg, uint16_t msg_type, uint8_t compress = 0, uint8_t encrypt = 0);
	virtual bool SendMsgToClient(const void* msg, uint16_t msg_len, uint16_t msg_type, uint8_t compress = 0, uint8_t encrypt = 0);

	virtual void OnLoginOut();

	virtual void OnLogin();

	virtual void OnGetAllData();

	virtual void ReLogin();

	virtual void OnTimeTick(uint64_t uTime, bool bNewDay);

	// 是否需要回收
	virtual bool NeedRecover();
	// 设置回收
	void SetNeedRecover(bool bNeed);
	// 登陆key
	void SetLoginKey(const string& key);
	string GetLoginKey();

	void GetPlayerBaseData(net::base_info* pInfo);
	void SetPlayerBaseData(const net::base_info& info);
    void SetOfflineTime(uint32_t _time){ m_offlinetime = _time;};

private:

protected:
	uint32_t m_uid;
	uint8_t  m_bPlayerType;
	NetworkObject* m_pSession;
	uint8_t  m_bPlayerState;
	bool   m_needRecover;                    	  // 需要下线回收
	string m_loginKey;                       	  // 登陆key
    uint32_t m_offlinetime;                         // 最后登录时间
	net::base_info                	m_baseInfo;   // 基础信息
	std::bitset<emACCDATA_TYPE_MAX> m_loadState;  // 数据加载状态

};


