
#pragma once

#include "svrlib.h"
#include <vector>

using namespace std;

#define PRO_DENO_100w    1000000
#define PRO_DENO_10w     100000
#define PRO_DENO_10000   10000
#define PRO_DENO_100     100

// ���ݿ��ʾ
enum DB_INDEX_TYPE
{
  DB_INDEX_TYPE_ACC    = 0,
  DB_INDEX_TYPE_CFG    = 1,
  DB_INDEX_TYPE_CENTER = 2,
  DB_INDEX_TYPE_LOG    = 3,

  DB_INDEX_TYPE_MAX = 4,
};

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
  emSERVER_TYPE_DBA    = 4, // DBA

};
// ·������
enum emROUTE_TYPE
{
  emROUTE_TYPE_ALL_GAME  = 1,// ȫ����Ϸ��
  emROUTE_TYPE_ONE_GAME  = 2,// ָ����Ϸ��

};

































