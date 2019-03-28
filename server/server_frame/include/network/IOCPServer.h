
#pragma once

#include <unordered_map>
#include <pthread.h>
#include <string.h>
#include "unistd.h"
#include "sys/sysinfo.h"

namespace Network
{

class NetworkObject;
class IoHandler;

//对象池接口
class CNetworkObjPool
{
public:
	virtual NetworkObject* CreateAcceptedObject() = 0;
	virtual void DestroyAcceptedObject(NetworkObject* pNetworkObject) = 0;
	virtual void DestroyConnectedObject(NetworkObject* pNetworkObject) = 0;
};
//对象池接口模板
template<typename TYPE>
class CNetworkObjPoolTemplete : public CNetworkObjPool
{
public:
	virtual NetworkObject* CreateAcceptedObject()
	{
		return new TYPE();
	}
	virtual void DestroyAcceptedObject(NetworkObject* pNetworkObject)
	{
		if (pNetworkObject != NULL)delete (pNetworkObject);
	};
	virtual void DestroyConnectedObject(NetworkObject* pNetworkObject)
	{

	};
};

//-------------------------------------------------------------------------------------------------
/// I/O 
//-------------------------------------------------------------------------------------------------
struct stIOHANDLER_DESC
{
  uint32_t ioHandlerKey;
  uint32_t maxAcceptSession;      // 最大接受连接数
  uint32_t maxConnectSession;     // 最大主动连接数
  uint32_t maxConnectBuffSize;    // 主动连接BuffSize
  uint32_t sendBufferSize;        // 发送缓存
  uint32_t recvBufferSize;        // 接受缓存
  uint32_t timeOut;               // 超时断开(秒)
  uint32_t maxPacketSize;         // 最大包长
  uint8_t  ioThreadNum;				// IO线程数(最大16)
  CNetworkObjPool* pool;            // 对象池
  bool openMsgQueue;                // 是否开启消息队列(防止服务器突发阻塞爆掉缓存)
  bool webSocket;					// 是否websocket

  stIOHANDLER_DESC()
  {
	  memset(this, 0, sizeof(stIOHANDLER_DESC));
	  ioThreadNum  = get_nprocs();
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


