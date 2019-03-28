
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

//����ؽӿ�
class CNetworkObjPool
{
public:
	virtual NetworkObject* CreateAcceptedObject() = 0;
	virtual void DestroyAcceptedObject(NetworkObject* pNetworkObject) = 0;
	virtual void DestroyConnectedObject(NetworkObject* pNetworkObject) = 0;
};
//����ؽӿ�ģ��
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
  uint32_t maxAcceptSession;      // ������������
  uint32_t maxConnectSession;     // �������������
  uint32_t maxConnectBuffSize;    // ��������BuffSize
  uint32_t sendBufferSize;        // ���ͻ���
  uint32_t recvBufferSize;        // ���ܻ���
  uint32_t timeOut;               // ��ʱ�Ͽ�(��)
  uint32_t maxPacketSize;         // ������
  uint8_t  ioThreadNum;				// IO�߳���(���16)
  CNetworkObjPool* pool;            // �����
  bool openMsgQueue;                // �Ƿ�����Ϣ����(��ֹ������ͻ��������������)
  bool webSocket;					// �Ƿ�websocket

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

	//������
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


