
#pragma once

#include "svrlib.h"

// ��һ�����Ϣ
struct stBaseInfo {
    string name;                    // �ǳ�
    uint32_t sex;                   // �Ա�
    uint32_t coin;                  // ���
    uint32_t vip;                   // vip����ֵ
    uint32_t clogin;                // ������½����
    uint32_t weeklogin;             // �����ۼƵ�½����
    uint32_t login_ip;              // ��½IP
    uint32_t all_login_days;        // �ۼƵ�½����
    uint32_t offline_time;          // ����ʱ��

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

// ��������
enum emACCDATA_TYPE {
    emACCDATA_TYPE_BASE = 0,    // ������Ϣ
    emACCDATA_TYPE_MISS,        // ��������

    emACCDATA_TYPE_MAX,
};










