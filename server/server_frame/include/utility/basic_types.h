
/***************************************************************
* 定义各种基本类型
***************************************************************/
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifndef NULL
#define NULL	0
#endif//NULL
//------------------------------------------------------
//------------------------------- min/max Operate...
//最小值
#ifndef MIN
#define MIN(a, b)    (a > b ? b: a)
#endif//MIN
//最大值
#ifndef MAX
#define MAX(a, b)    (a > b ? a: b)
#endif//MAX
//范围值
#ifndef RANGE
#define RANGE(a, mina, maxa)  (a = MAX(mina,MIN(a,maxa)))
#endif//

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)                { if ( p) { delete ( p); ( p) = NULL; } }
#endif

//#############################################################
//############################## 三角形数据
//#############################################################
#ifndef M_PI
#define M_PI        3.1415926535897932384626433
#endif//M_PI

#ifndef M_2PI
#define M_2PI        (M_PI * 2.0)
#endif//M_2PI

#ifndef M_PI_F
#define M_PI_F        3.1415926535897932384626433f
#endif//M_PI_F

#ifndef M_2PI_F
#define M_2PI_F    (M_PI_F * 2.0f)
#endif//M_2PI_F

//#############################################################
//############################## 常量
//#############################################################
#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif





