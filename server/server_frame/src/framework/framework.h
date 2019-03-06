
#pragma once

#include "framework/application.h"
#include "utility/singleton.h"
#include "boost/asio.hpp"
#include <memory>
#include "config/config.h"

class CFrameWork : public AutoDeleteSingleton<CFrameWork>
{
public:
	CFrameWork();
	~CFrameWork();

	void Run();
	void TimerTick(const std::error_code& err);
	void InitializeEnvironment(int argc, char* argv[]);
	void InitSpdlog();
	void InitMysqlSpdlog();

	void SignalHandler(const std::error_code& err, int signal);

	void SetServerID(uint32_t svrID);
	uint32_t GetServerID();
	void SetTickTime(unsigned int tick);

private:
	void ParseInputParam(int argc, char* argv[]);
	void LoadConfig();
	void WritePidToFile();

	uint64_t       m_sleepTime;
	std::shared_ptr<boost::asio::system_timer> m_pTimer;
	svrlib::stServerCfg m_serverCfg;
};

extern string g_strConfFilename;


