
#pragma once

#include "svrlib.h"

// 玩家基本信息
struct stBaseInfo {
    string name;                    // 昵称
    uint32_t sex;                   // 性别
    uint32_t coin;                  // 金币
    uint32_t vip;                   // vip经验值
    uint32_t clogin;                // 连续登陆天数
    uint32_t weeklogin;             // 本周累计登陆天数
    uint32_t login_ip;              // 登陆IP
    uint32_t all_login_days;        // 累计登陆天数
    uint32_t offline_time;          // 下线时间

    stBaseInfo() {
        name = "";
        sex = 0;
        coin = 0;
        vip = 0;
        clogin = 0;
        weeklogin = 0;
        login_ip = 0;
        all_login_days = 0;
        offline_time = 0;
    }
};

// 数据类型
enum emACCDATA_TYPE {
    emACCDATA_TYPE_BASE = 0,    // 基本信息
    emACCDATA_TYPE_MISS,        // 任务数据

    emACCDATA_TYPE_MAX,
};










