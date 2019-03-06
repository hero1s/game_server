
#pragma once

#include "utility/time_function.h"
#include "utility/singleton.h"
#include "modern/timer_wheel.h"
#include "math/rand_table.h"
#include "network/IOCPServer.h"
#include "modern/sol/sol.hpp"
#include <memory>
#include <map>
#include "boost/asio.hpp"
#include "cmdline.h"

using namespace std;
using namespace Network;

extern int FrameworkMain(int argc, char *argv[]);

class CApplication : public AutoDeleteSingleton<CApplication> {
public:
    CApplication();

    ~CApplication();

public:
    bool PreInit();

    void OverShutDown();

    void PreTick();

    void SetServerID(unsigned int svrid);

    uint32_t GetServerID();

    //状态
    void SetStatus(uint8_t status);

    uint8_t GetStatus();

    //定时器
    void schedule(TimerEventInterface *event, uint64_t delta);

    void schedule_in_range(TimerEventInterface *event, uint64_t start, uint64_t end);

    //网络模块
    IOCPServer& GetIOCPServer();

    //获得sol模块
    sol::state &GetSolLuaState();

    boost::asio::io_context &GetAsioContext();

public:
//具体实例去实现
    bool Initialize();

    void ShutDown();

    void ConfigurationChanged();

    void Tick();

    void ExceptionHandle();

private:
    unsigned int m_uiServerID;
    uint8_t m_status;                 // 服务器状态
    uint64_t m_lastTick;              // 上次tick时间
    TimerWheel m_timers;            // wheel定时器
    IOCPServer m_iocpServer;        // 网络服务模块
    sol::state m_solLua;            // sol lua模块
    boost::asio::io_context m_ioContext;

};


