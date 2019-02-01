
#pragma once

#include "svrlib.h"
#include <vector>

using namespace std;

static const uint32_t SECONDS_IN_MIN      = 60;
static const uint32_t SECONDS_IN_30_MIN   = 1800;         // ��Сʱ
static const uint32_t SECONDS_IN_ONE_HOUR = 3600;         // һСʱ
static const uint32_t SECONDS_IN_ONE_DAY  = 24*3600;      // һ�������
static const uint32_t SECONDS_IN_ONE_WEEK = 7*86400;      // һ�ܵ�����
static const uint32_t SECONDS_IN_ONE_MON  = 30*86400;     // һ���µ�����
static const uint32_t SECONDS_IN_ONE_YEAR = 365*86400;    // һ�������

#define PRO_DENO_100w    1000000
#define PRO_DENO_10w     100000
#define PRO_DENO_10000   10000
#define PRO_DENO_100     100

#define ROUTE_MSG_ID     2000
#define PLAT_HEAD_LEN    5

#define SERVER_SOCKET_BUFF_SIZE 1024*1024*2

// ���ݿ��ʾ
enum DB_INDEX_TYPE
{
  DB_INDEX_TYPE_ACC    = 0,
  DB_INDEX_TYPE_CFG    = 1,
  DB_INDEX_TYPE_CENTER = 2,
  DB_INDEX_TYPE_LOG    = 3,

  DB_INDEX_TYPE_MAX = 4,
};
#define REDIS_INDEX_MAX  1
#define REDIS_POOL_SIZE  2
#define REDIS_TIME_OUT   5

// ������״̬
enum emSERVER_STATE
{
  emSERVER_STATE_NORMAL = 0,        // ����״̬
  emSERVER_STATE_REPAIR,            // ά��״̬

};
// ����������
enum emSERVER_TYPE
{
  emSERVER_TYPE_LOBBY  = 1, // ������
  emSERVER_TYPE_GAME   = 2, // ��Ϸ��
  emSERVER_TYPE_CENTER = 3, // ���ķ�

};
// ·������
enum emROUTE_TYPE
{
  emROUTE_TYPE_ALL_GAME  = 1,// ȫ����Ϸ��
  emROUTE_TYPE_ONE_GAME  = 2,// ָ����Ϸ��

};

































