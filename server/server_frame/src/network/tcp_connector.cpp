//
// Created by Administrator on 2018/7/4.
//
#include "network/tcp_connector.h"
#include "svrlib.h"

namespace Network
	{
CTcpConnector::CTcpConnector()
		:m_timer(this)
{
	m_netState = emNet_NULL;
	m_pServer  = NULL;
	m_ioKey    = 0;
}
CTcpConnector::~CTcpConnector()
{
	m_netState = emNet_NULL;
	m_pServer  = NULL;
	m_ioKey    = 0;
}
bool CTcpConnector::Init(IOCPServer* pServer, uint32_t ioKey, std::string ip, uint32_t port)
{
	m_pServer = pServer;
	m_ioKey   = ioKey;
	memset(m_szRemoteIp, 0, sizeof(m_szRemoteIp));
	strcpy(m_szRemoteIp, ip.c_str());
	m_RemotePort = port;
	m_netState   = emNet_DISCONNECT;

	CApplication::Instance().schedule(&m_timer, 1000);
	return true;
}
void CTcpConnector::ShutDown()
{
	Disconnect(false);
	m_timer.cancel();
}
void CTcpConnector::OnTimer()
{
	ReConnect();
	//CApplication::Instance().schedule(&m_timer, 2000);
}
void CTcpConnector::ReConnect()
{
	if (m_netState == emNet_DISCONNECT)//断开连接
	{
		uint32_t index = m_pServer->Connect(m_ioKey, this, m_szRemoteIp, m_RemotePort);
		LOG_DEBUG("TcpConnector connect:{}--{}", m_szRemoteIp, m_RemotePort);
		m_netState = emNet_CONNECTING;//连接中
	}
}
void CTcpConnector::OnDisconnect()
{
	m_netState = emNet_DISCONNECT;
	ConnectorOnDisconnect();
	CApplication::Instance().schedule(&m_timer, 2000);
}
void CTcpConnector::OnConnect(bool bSuccess)
{
	if (bSuccess)
	{
		m_netState = emNet_CONNECTED;
		LOG_DEBUG("TcpConnector connect success :{}--{}", m_szRemoteIp, m_RemotePort);
	}
	else
	{
		m_netState = emNet_DISCONNECT;
		LOG_DEBUG("TcpConnector connect fail :{}--{}", m_szRemoteIp, m_RemotePort);
		CApplication::Instance().schedule(&m_timer, 2000);
	}
	ConnectorOnConnect(bSuccess);
}

};



