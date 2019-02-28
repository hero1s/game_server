//
// Created by Administrator on 2018/8/14.
//

#pragma once

#include "modern/locked_queue.h"

#include <stdlib.h>
#include <memory>
#include <mutex>
#include <queue>
#include <list>

namespace Network
{
class CMessage
{
public:
	CMessage(const void* pMsg, uint16_t wSize)
	{
		m_pMsg  = new uint8_t[wSize];
		m_wSize = wSize;
		memcpy(m_pMsg,pMsg,wSize);
	}
	~CMessage()
	{
		delete[](m_pMsg);
		m_pMsg  = NULL;
		m_wSize = 0;
	}
	uint8_t* Data()
	{
		return m_pMsg;
	}
	uint16_t Length()
	{
		return m_wSize;
	}
private:
	CMessage()
	{
	}
protected:
	uint8_t* m_pMsg;    // 消息指针
	uint16_t m_wSize;   // 消息长度
};

};

