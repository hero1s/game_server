//
// Created by Administrator on 2018/10/9.
//

#pragma once

#include <mutex>

class ReadWriteLock
{
	ReadWriteLock()
			:m_readCount(0)
	{
	}

public:
	void ReadLock()
	{
		std::lock_guard<std::mutex> lockGuard(m_readMtx);

		if (m_readCount == 0)
		{
			m_writeMtx.lock();
		}

		m_readCount++;
	}

	void ReadUnLock()
	{
		std::lock_guard<std::mutex> lockGuard(m_readMtx);

		m_readCount--;
		if (m_readCount == 0)
		{
			m_writeMtx.unlock();
		}
	}

	void WriteLock()
	{
		m_writeMtx.lock();
	}

	void WriteUnLock()
	{
		m_writeMtx.unlock();
	}

private:
	ReadWriteLock& operator=(const ReadWriteLock&) = delete;
	ReadWriteLock(const ReadWriteLock&) = delete;

private:
	std::mutex m_readMtx;
	std::mutex m_writeMtx;
	int        m_readCount;
};


