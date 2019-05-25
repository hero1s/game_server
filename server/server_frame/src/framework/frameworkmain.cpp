
#include <iostream>
#include "framework.h"
#include "framework/application.h"
#include <malloc.h>
#include "sol/sol.hpp"
#include "helper/helper.h"
#include "lua/lua.h"
#include "spdlog/spdlog.h"
#include "utility/stringutility.h"
#include "utility/comm_macro.h"
#include "config/config.h"
#include "lua_service/lua_bind.h"
#include <memory>

using namespace svrlib;

int FrameworkMain(int argc, char *argv[]) {
    if (argc <= 0 || argv == NULL) {
        throw "The input argument for FrameworkMain is illegal!";
    }
    //mallopt(M_ARENA_MAX, 1);

    try {

        CFrameWork::Instance().InitializeEnvironment(argc, argv);

        CFrameWork::Instance().Run();
        LOG_DEBUG("application shutdown");

        CFrameWork::Instance().ShutDown();
    }
    catch (char const *what) {
        std::cout << what << std::endl;
        LOG_ERROR("process exit {}", what);
    }
    catch (sol::error &e) {
        std::cout << "sol error " << e.what() << std::endl;
        LOG_ERROR("sol error:{}", e.what());
    }
    return 0;
}

CApplication::CApplication() {
    m_status = 0;
    m_lastTick = 0;
    m_wheelTime = 0;
    m_wheelPrecision = 10;
    m_solLua.open_libraries();
    m_luaService = new svrlib::lua_service(&m_solLua);
}

CApplication::~CApplication() {
    m_status = 0;
    SAFE_DELETE(m_luaService);
}

bool CApplication::PreInit() {
    lua_bind bind(m_solLua);
    bind.export_lua_bind();
    
    return true;
}

bool CApplication::OverPreInit(){
    SOL_CALL_LUA(m_solLua["init_lua_service"](m_luaService));
    return true;
}

void CApplication::OverShutDown() {
    m_iocpServer.Shutdown();
    spdlog::drop_all();
}

uint64_t CApplication::PreTick() {
    // 驱动时钟
    setSystemTick64();// 更新tick
    setSysTime();
    std::srand(getSysTime());
    g_RandGen.Reset(getSysTime());
    if (m_lastTick == 0) {
        m_lastTick = getSystemTick64();
    }
    uint64_t curTime = getSystemTick64();
    int64_t delta = curTime - m_lastTick;
    if (delta > 0) {
        m_wheelTime += delta;
        m_timers.advance(m_wheelTime/m_wheelPrecision);
        m_wheelTime = m_wheelTime%m_wheelPrecision;
        m_lastTick = curTime;
    }
    m_iocpServer.Update();
    CheckNewDayEvent();

    return delta;
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
    m_timers.schedule(event, delta/m_wheelPrecision);
}

void CApplication::schedule_in_range(TimerEventInterface *event, uint64_t start, uint64_t end) {
    m_timers.schedule_in_range(event, start/m_wheelPrecision, end/m_wheelPrecision);
}

//网络模块
IOCPServer &CApplication::GetIOCPServer() {
    return m_iocpServer;
}

//获得sol模块
sol::state &CApplication::GetSolLuaState() {
    return m_solLua;
}

asio::io_context &CApplication::GetAsioContext() {
    return m_ioContext;
}
//获取lua_service
svrlib::lua_service* CApplication::GetLuaService(){
    return m_luaService;
}
//----检测日期变更---
void CApplication::CheckNewDayEvent(){
    static uint64_t uProcessTime = 0;
    uint64_t uTime = getSysTime();
    if (!uProcessTime)
        uProcessTime = uTime;
    if (uTime != uProcessTime) {
        bool bNewDay = (diffTimeDay(uProcessTime, uTime) != 0);
        if (bNewDay) {
            if (m_OnNewDay) {
                m_OnNewDay();
            }
            // 新的一天
            tm local_time;
            uint64_t uTime = getTime();
            getLocalTime(&local_time, uTime);
            // 跨周        0-6
            if (local_time.tm_wday == 0) {
                if (m_OnNewWeek) {
                    m_OnNewWeek();
                }
            }
            // 跨月        1-31
            if (local_time.tm_mday == 1) {
                if (m_OnNewMonth) {
                    m_OnNewMonth();
                }
            }
        }
        uProcessTime = uTime;
    }
}
void CApplication::InitWheelPrecision(uint32_t precision){
    m_wheelPrecision = precision > 1 ? precision : 1;
}
