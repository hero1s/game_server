
#pragma once

// 玩家基础数据
struct stPlayerBaseInfo
{
  string name;           // 姓名
  uint8_t  sex;          // 性别
  uint32_t offlinetime;  // 最后登录时间
  uint32_t clogin;       // 连续登陆天数
  uint8_t  weekLogin;    // 本周累计登陆
  uint8_t  bankrupt;     // 破产次数
  uint32_t loginIP;      // 登陆IP
  uint32_t allLoginDays; // 累计登陆天数
  int64_t  coin;         // 金币
  int32_t  vip;          // vip值
  double lon;            // 经纬度
  double lat;            // 经纬度

  stPlayerBaseInfo()
  {
	  clear();
  }

  void clear()
  {
	  name         = "";
	  sex          = 0;
	  offlinetime  = 0;
	  clogin       = 0;
	  weekLogin    = 0;
	  bankrupt     = 0;
	  loginIP      = 0;
	  allLoginDays = 0;
	  coin         = 0;
	  vip          = 0;
	  lon          = 0;
	  lat          = 0;
  }
};

// 数据类型
enum emACCDATA_TYPE
{
  emACCDATA_TYPE_BASE = 0,    // 基本信息
  emACCDATA_TYPE_MISS,        // 任务数据

  emACCDATA_TYPE_MAX,
};










