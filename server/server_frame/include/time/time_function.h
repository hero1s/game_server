
#pragma once

#include "utility/basic_types.h"
#include <time.h>
#include <sys/time.h>

/*************************************************************/
//--- 获得系统启动毫秒
extern uint32_t getTickCount();

//--- 获得系统启动毫秒(64位)
extern uint64_t getTickCount64();

//--- 获得系统启动毫秒(64位)【需要手动更新】
extern uint64_t getSystemTick64();

//--- 设置系统启动毫秒
extern uint64_t setSystemTick64();

//--- 获得系统秒时间
extern uint64_t getSecond();

//--- 获得系统毫秒时间
extern uint64_t getMillisecond();

//--- 获得系统微秒时间
extern uint64_t getMicroseconds();

//--- 获得时钟时间(毫秒)
extern uint64_t getClockTime();

//--- 获得系统时间
extern uint64_t getTime();

//--- 获得系统时间
extern uint64_t setSysTime();

extern uint64_t getSysTime();

//--- 获得今日开始时间
extern uint64_t getDayBeginTime();

//--- 获得本地时间
extern int32_t getLocalTime(struct tm* _Tm, const uint64_t* _Time);

extern int32_t getLocalTime(struct tm* _Tm, uint64_t _Time);

//--- 获得时间相差天数
extern int32_t diffTimeDay(uint64_t _early, uint64_t _late);

//--- 获得时间相差周数
extern int32_t diffTimeWeek(uint64_t _early, uint64_t _late);

//--- 获得时间相差月数
extern int32_t diffTimeMonth(uint64_t _early, uint64_t _late);

//--- 时间格式化(YYYY-MM-DD HH:MM:SS)
extern const char* time_format(uint64_t _time);

//--- 时间日期(YYYYMMDD)
extern const char* date_format(uint64_t _time);

struct SYSTEMTIME {
  uint16_t wYear;
  uint16_t wMonth;
  uint16_t wDayOfWeek;
  uint16_t wDay;
  uint16_t wHour;
  uint16_t wMinute;
  uint16_t wSecond;
  uint16_t wMilliseconds;
};

extern void getLocalTime(SYSTEMTIME& systime);



