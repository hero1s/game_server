
#include "framework.h"
#include <signal.h>
#include <iostream>
#include "helper/filehelper.h"
#include "helper/helper.h"
#include "cmdline.h"
#include "spdlog/spdlog.h"
#include "utility/comm_macro.h"
#include "utility/timeutility.h"

#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/ioctl.h>
#include<stdlib.h>
#include "asio.hpp"
#include <chrono>
#include <ctime>

using namespace svrlib;
using std::vector;

#define TICK_MAX_INTERVAL 100
#define TICK_MIN_INTERVAL 10

CFrameWork::CFrameWork(CApplication& app)
:m_application(app)
{
    m_sleepTime = TICK_MIN_INTERVAL;
}

CFrameWork::~CFrameWork()
{
}

void CFrameWork::Run()
{

    m_pTimer = make_shared<asio::system_timer>(m_application.GetAsioContext());
    m_pTimer->expires_from_now(std::chrono::milliseconds(m_sleepTime));
    m_pTimer->async_wait(std::bind(&CFrameWork::TimerTick, this, std::placeholders::_1));

    try {
        m_application.GetAsioContext().run();
    }catch (std::exception& e){
        std::cout << "asio error " << e.what() << std::endl;
        LOG_ERROR("asio error:{}", e.what());
    }

}

void CFrameWork::ShutDown(){
    m_application.ShutDown();
    m_application.OverShutDown();
}

void CFrameWork::TimerTick(const std::error_code& err)
{
    uint64_t startTime = 0;
    uint64_t endTime = 0;
    uint64_t sleepTime = 0;
    if (!err) {
        startTime = getTickCount64();
        m_application.PreTick();
        m_application.Tick();

        endTime = getTickCount64();
        sleepTime = endTime-startTime;
        if (sleepTime>TICK_MAX_INTERVAL) {
            LOG_ERROR("************** preccess tick time out:{} *********", sleepTime);
        }

        m_pTimer->expires_from_now(std::chrono::milliseconds(m_sleepTime));
        m_pTimer->async_wait(std::bind(&CFrameWork::TimerTick, this, std::placeholders::_1));
    }
    else {
        LOG_ERROR("asio timer is error,shutdown");
        m_application.GetAsioContext().stop();
    }

}

void CFrameWork::InitializeEnvironment(int argc, char* argv[])
{
    ParseInputParam(argc, argv);
    asio::signal_set sigset(m_application.GetAsioContext(), SIGUSR2, SIGUSR1);
    sigset.async_wait(std::bind(&CFrameWork::SignalHandler, this, std::placeholders::_1, std::placeholders::_2));

    LoadConfig();
    //提前写入进程id,防止多次启动
    WritePidToFile();

    m_application.PreInit();
    m_application.GetSolLuaState()["set_server_cfg"](m_application.GetServerID(), &m_serverCfg);
    InitSpdlog();
    InitMysqlSpdlog();

    m_application.OverPreInit();
    bool bRet = m_application.Initialize();
    if (bRet==false) {
        exit(1);
    }
    else {
        LOG_DEBUG("start server success....");
        WritePidToFile();
    }
}

void CFrameWork::InitSpdlog()
{
    if (m_serverCfg.logasync>0) {//异步模式
        auto log = spdlog::rotating_logger_st("log", m_serverCfg.logname, m_serverCfg.logsize, m_serverCfg.logdays);
        log->flush_on(spdlog::level::err);
        log->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%n][%l][tid %t] %v");
        log->set_level(spdlog::level::level_enum(m_serverCfg.loglv));
        spdlog::set_async_mode(m_serverCfg.logasync);

    }
    else {
        auto log = spdlog::rotating_logger_mt("log", m_serverCfg.logname, m_serverCfg.logsize, m_serverCfg.logdays);
        log->flush_on(spdlog::level::trace);
        log->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%n][%l][tid %t] %v");
        log->set_level(spdlog::level::level_enum(m_serverCfg.loglv));
        spdlog::set_sync_mode();

    }
}

void CFrameWork::InitMysqlSpdlog()
{
    if(m_serverCfg.logmysql.length()<2){
        LOG_DEBUG("mysql log length < 2,not open mysql log {}",m_serverCfg.logmysql);
        return;
    }

    if (m_serverCfg.logasync>0) {//异步模式
        auto logmysql = spdlog::rotating_logger_st("mysql", m_serverCfg.logmysql, m_serverCfg.logsize,
                                                   m_serverCfg.logdays);
        logmysql->flush_on(spdlog::level::err);
        logmysql->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%n][%l][tid %t] %v");
        logmysql->set_level(spdlog::level::level_enum(m_serverCfg.loglv));
        spdlog::set_async_mode(m_serverCfg.logasync);
    }
    else {
        auto logmysql = spdlog::rotating_logger_mt("mysql", m_serverCfg.logmysql, m_serverCfg.logsize,
                                                   m_serverCfg.logdays);
        logmysql->flush_on(spdlog::level::trace);
        logmysql->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%n][%l][tid %t] %v");
        logmysql->set_level(spdlog::level::level_enum(m_serverCfg.loglv));
    }
}

void CFrameWork::SetTickTime(unsigned int tick)
{
    m_sleepTime = tick;
    RANGE(m_sleepTime, TICK_MIN_INTERVAL, TICK_MAX_INTERVAL);
}

void CFrameWork::ParseInputParam(int argc, char* argv[])
{
    cmdline::parser a;
    a.add<int>("sid", '\0', "server id", false, 1, cmdline::range(1, 100000));
    a.add<int>("tick", '\0', "tick time", false, 10, cmdline::range(10, 100));
    a.add<string>("cfg", '\0', "cfg file name", false, "");

    bool ok = a.parse(argc, argv);
    if (!ok) {
        printf("%s : %s \n\r", a.error().c_str(), a.usage().c_str());
        exit(1);
        return;
    }
    m_application.SetServerID(a.get<int>("sid"));
    SetTickTime(a.get<int>("tick"));
    string cfgName = a.get<string>("cfg");

    m_confFilename = cfgName;
}

void CFrameWork::LoadConfig()
{
    m_application.GetSolLuaState().do_file(m_confFilename);
}

void CFrameWork::SignalHandler(const std::error_code& err, int signal)
{
    switch (signal) {
    case SIGUSR1: {
        m_application.GetAsioContext().stop();
        LOG_DEBUG("program exiting...");
    }
        break;
    case SIGUSR2: {
        LOG_DEBUG("program reload config...");
        m_application.GetAsioContext().post([this]() {
            m_application.GetSolLuaState().do_file(m_confFilename);
            m_application.ConfigurationChanged();
        });
    }
        break;
    default:
        break;
    }
}

void CFrameWork::WritePidToFile()
{
    std::ostringstream oss;
    oss << getpid();
    std::string strShFileName = CHelper::GetExeDir();
    strShFileName = CStringUtility::FormatToString("pid_%d.txt", m_application.GetServerID());

    CFileHelper oFile(strShFileName.c_str(), CFileHelper::MOD_WRONLY_TRUNC);
    oFile.Write(0, oss.str().c_str(), oss.str().length());
    oFile.Close();
    CTimeUtility::Sleep(10);
    oss.str("");
    oss << "chmod 777 " << strShFileName;

    ::system(oss.str().c_str());
}






