
#pragma once

#include "framework/application.h"
#include "asio.hpp"
#include <memory>
#include "config/config.h"

class CFrameWork
{
public:
	CFrameWork(CApplication& app);
	~CFrameWork();

	void Run();
	void ShutDown();
	void TimerTick(const std::error_code& err);
	void InitializeEnvironment(int argc, char* argv[]);
	void InitSpdlog();
	void InitMysqlSpdlog();

	void SignalHandler(const std::error_code& err, int signal);

	void SetTickTime(uint32_t tick);
private:
	void ParseInputParam(int argc, char* argv[]);
	void LoadConfig();
	void WritePidToFile();

	uint64_t       m_sleepTime;
	std::shared_ptr<asio::system_timer> m_pTimer;
	svrlib::stServerCfg m_serverCfg;
	string m_confFilename = "";
	CApplication& m_application;
};



