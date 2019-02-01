
#include <iostream>
#include "framework.h"
#include "framework/application.h"
#include <malloc.h>
#include "modern/sol/sol.hpp"
#include "helper/helper.h"
#include "lua/lua.h"
#include "modern/spdlog/spdlog.h"
#include "utility/stringutility.h"
#include "utility/comm_macro.h"
#include "config/config.h"

using namespace svrlib;

int FrameworkMain(int argc, char *argv[]) {
    if (argc <= 0 || argv == NULL)
    {
        throw "The input argument for FrameworkMain is illegal!";
    }
    mallopt(M_ARENA_MAX, 1);

    try
    {
        CFrameWork &frameWork = CFrameWork::Instance();
        frameWork.InitializeEnvironment(argc, argv);

        frameWork.Run();
        LOG_DEBUG("application shutdown");

        CApplication::Instance().ShutDown();
        CApplication::Instance().OverShutDown();
        //
    }
    catch (char const *what)
    {
        std::cout << what << std::endl;
        LOG_ERROR("process exit {}", what);
    }
    catch (sol::error &e)
    {
        std::cout << "sol error " << e.what() << std::endl;
        LOG_ERROR("sol error:{}", e.what());
    }

    return 0;
}

CApplication::CApplication() {
    m_status = 0;
    m_lastTick = 0;
    m_solLua.open_libraries();
}

CApplication::~CApplication() {
    m_status = 0;
}

bool CApplication::PreInit() {
    defLuaBaseConfig(m_solLua);

    return true;
}

void CApplication::OverShutDown() {
    m_iocpServer.Shutdown();
    spdlog::drop_all();
}

void CApplication::PreTick() {
    // 驱动时钟
    setSystemTick64();// 更新tick
    setSysTime();
    std::srand(getSysTime());
    g_RandGen.Reset(getSysTime());
    if (m_lastTick == 0)
    {
        m_lastTick = getSystemTick64();
    }
    uint64_t curTime = getSystemTick64();
    int64_t delta = curTime - m_lastTick;
    if (delta > 0)
    {
        m_timers.advance(delta);
        m_lastTick = curTime;
    }
    m_iocpServer.Update();
}

void CApplication::SetServerID(unsigned int svrid) {
    m_uiServerID = svrid;
}

uint32_t CApplication::GetServerID() {
    return m_uiServerID;
}

//状态
void CApplication::SetStatus(uint8_t status) {
    m_status = status;
}

uint8_t CApplication::GetStatus() {
    return m_status;
}

void CApplication::schedule(TimerEventInterface *event, uint64_t delta) {
    m_timers.schedule(event, delta);
}
void CApplication::schedule_in_range(TimerEventInterface *event, uint64_t start, uint64_t end) {
    m_timers.schedule_in_range(event, start, end);
}
//网络模块
IOCPServer& CApplication::GetIOCPServer() {
    return m_iocpServer;
}
//获得sol模块
sol::state &CApplication::GetSolLuaState() {
    return m_solLua;
}

asio::io_context &CApplication::GetAsioContext() {
	return m_ioContext;
}

