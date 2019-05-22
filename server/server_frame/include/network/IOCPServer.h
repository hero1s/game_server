
#pragma once

#include <unordered_map>
#include <pthread.h>
#include <string.h>
#include "unistd.h"
#include <functional>
#include "NetworkObject.h"

namespace Network
{
class IoHandler;

using AcceptAllocFunc = std::function<NetworkObject*()>;
using AcceptFreeFunc = std::function<void(NetworkObject*)>;

struct stIOHANDLER_DESC
{
  uint32_t ioHandlerKey;
  uint32_t maxAcceptSession;      // 最大接受连接数
  uint32_t maxConnectBuffSize;    // 主动连接BuffSize
  uint32_t sendBufferSize;        // 发送缓存
  uint32_t recvBufferSize;        // 接受缓存
  uint32_t timeOut;               // 超时断开(秒)
  uint32_t maxPacketSize;         // 最大包长
  AcceptAllocFunc allocFunc;
  AcceptFreeFunc freeFunc;

  bool openMsgQueue;              // 是否开启消息队列(防止服务器突发阻塞爆掉缓存)
  bool webSocket;				  // 是否websocket

  stIOHANDLER_DESC()
  {
	  memset(this, 0, sizeof(stIOHANDLER_DESC));
	  maxAcceptSession = 10000;
	  allocFunc = nullptr;
	  freeFunc  = [](NetworkObject* p){ delete p; };
	  openMsgQueue = false;
	  webSocket	   = false;
  }
};

class IOCPServer
{
	friend void* send_thread(void* param);

public:
	IOCPServer();
	virtual ~IOCPServer();

    bool AddIoHandler(stIOHANDLER_DESC& lpDesc);
	bool StartListen(uint32_t dwIoHandlerKey, const char* pIP, uint16_t wPort);
	void Update();
	void Shutdown();
	uint32_t Connect(uint32_t dwIoHandlerKey, NetworkObject* pNetworkObject, const char* pszIP, uint16_t wPort);
	int IsListening(uint32_t dwIoHandlerKey);
	uint32_t GetNumberOfConnections(uint32_t dwIoHandlerKey);

	//白名单
	void AddWhiteListIP(uint32_t dwIoHandlerKey, uint32_t ip);
	void ClearWhiteListIP(uint32_t dwIoHandlerKey);
	bool IsWhiteIP(uint32_t dwIoHandlerKey, uint32_t ip);
private:
	void CreateIoHandler(stIOHANDLER_DESC& lpDesc);
    void CreateSendThread();

	pthread_t                                m_hSendThread;
	int                                      m_bShutdown;
	std::unordered_map<uint32_t, IoHandler*> m_mapIoHandlers;
};

}


