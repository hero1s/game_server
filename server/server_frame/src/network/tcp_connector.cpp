//
// Created by Administrator on 2018/7/4.
//
#include "network/tcp_connector.h"
#include "svrlib.h"
#include <functional>

namespace Network
{
CTcpConnector::CTcpConnector()
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
	if (m_pTimer)m_pTimer->cancel();
}
bool CTcpConnector::Init(asio::io_context &context,IOCPServer* pServer, uint32_t ioKey, std::string ip, uint32_t port)
{
	m_pServer = pServer;
	m_ioKey   = ioKey;
	memset(m_szRemoteIp, 0, sizeof(m_szRemoteIp));
	strcpy(m_szRemoteIp, ip.c_str());
	m_RemotePort = port;
	m_netState   = emNet_DISCONNECT;

	m_pTimer = make_shared<asio::system_timer>(context);
	m_pTimer->expires_from_now(std::chrono::milliseconds(1000));
	m_pTimer->async_wait(std::bind(&CTcpConnector::OnTimer, this, std::placeholders::_1));
	return true;
}
void CTcpConnector::ShutDown()
{
	Disconnect(false);
	if (m_pTimer)m_pTimer->cancel();
}
void CTcpConnector::OnTimer(const std::error_code &err)
{
	ReConnect();
	m_pTimer->expires_from_now(std::chrono::milliseconds(2000));
	m_pTimer->async_wait(std::bind(&CTcpConnector::OnTimer, this, std::placeholders::_1));
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
	}
	ConnectorOnConnect(bSuccess);
}

};



