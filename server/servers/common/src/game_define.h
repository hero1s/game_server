
#pragma once

#include "svrlib.h"
#include <vector>

using namespace std;

#define PRO_DENO_100w    1000000
#define PRO_DENO_10w     100000
#define PRO_DENO_10000   10000
#define PRO_DENO_100     100

// 数据库标示
enum DB_INDEX_TYPE
{
  DB_INDEX_TYPE_ACC    = 0,
  DB_INDEX_TYPE_CFG    = 1,
  DB_INDEX_TYPE_CENTER = 2,
  DB_INDEX_TYPE_LOG    = 3,

  DB_INDEX_TYPE_MAX = 4,
};

// 服务器状态
enum emSERVER_STATE
{
  emSERVER_STATE_NORMAL = 0,        // 正常状态
  emSERVER_STATE_REPAIR,            // 维护状态

};
// 服务器类型
enum emSERVER_TYPE
{
  emSERVER_TYPE_LOBBY  = 1, // 大厅服
  emSERVER_TYPE_GAME   = 2, // 游戏服
  emSERVER_TYPE_CENTER = 3, // 中心服
  emSERVER_TYPE_DBA    = 4, // DBA

};
// 路由类型
enum emROUTE_TYPE
{
  emROUTE_TYPE_ALL_SERVER  = 1,// 全部游戏服
  emROUTE_TYPE_ONE_SERVER  = 2,// 指定游戏服


};

































