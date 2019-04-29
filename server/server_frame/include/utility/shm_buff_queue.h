//
// Created by toney on 2019/4/29.
//

#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>

namespace svrlib::shm_queue {

    struct DataHead {
        int iSize;
        int iBegin;
        int iEnd;
        int iOffset;
    };

    struct DataUnit {
        int iLen;
        char *pData;
    };

    enum SHM_RET {
        SHM_ERROR = -1,
        CREATE_SUCC = 0,
        ATTACH_SUCC = 1,
    };

    class CShm {
    public:
        CShm();
        CShm(size_t iShmSize, key_t iKey);
        ~CShm();
        int Init(size_t iShmSize, key_t iKey);
        char *GetShmBuff() { return m_pShmBuff; }
        int Detach();
        int Delete();
    protected:
        size_t m_iShmSize;
        key_t m_iShmKey;
        char *m_pShmBuff;
        int m_iShmID;
    };

#define BUFF_RESERVE_LENGTH 8

    class CShmBuffQueue
    {

    public:
        CShmBuffQueue(char *pShmBuff);
        ~CShmBuffQueue();

        //从buff   取数据, 只改变begin偏移
        int GetDataUnit(char *pOut, short *psOutLen);
        //向buff 加数据, 只改变end偏移
        int PutDataUnit(const char *pIn, short sInLen);
        void PrintHead();

    private:
        CShmBuffQueue(){}
        int GetLeftSize();
        int GetUsedSize();
        bool IsFull();
        DataHead *m_pDataHead;
        char *m_pBuff;
    };



};




