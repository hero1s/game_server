//
// Created by toney on 2019/4/29.
//
#include "memory/shm_buff_queue.h"
#include "utility/comm_macro.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>

namespace svrlib::shm_queue {

    CShm::CShm() {
        m_iShmID = -1;
        m_iShmKey = -1;
        m_iShmSize = 0;
        m_pShmBuff = NULL;
    }

    CShm::CShm(size_t iShmSize, key_t iKey) {
        Init(iShmSize, iKey);
    }

    CShm::~CShm() {
        Detach();
    }

    int CShm::Init(size_t iShmSize, key_t iKey) {
        /*
            成功返回共享内存的标识符；不成功返回-1，errno储存错误原因。
            EINVAL           参数size小于SHMMIN或大于SHMMAX。
            EEXIST           预建立key所致的共享内存，但已经存在。
            EIDRM            参数key所致的共享内存已经删除。
            ENOSPC        超过了系统允许建立的共享内存的最大值(SHMALL )。
            ENOENT        参数key所指的共享内存不存在，参数shmflg也未设IPC_CREAT位。
            EACCES        没有权限。
            ENOMEM       核心内存不足。

        */

        int iRet = SHM_ERROR;
        m_iShmID = shmget(iKey, iShmSize, IPC_CREAT | IPC_EXCL | 0666);
        if (m_iShmID < 0) {
            if (errno != EEXIST) {
                LOG_ERROR("Alloc share memory failed, {} ", strerror(errno));
                return SHM_ERROR;
            }
            //可能已经存在,尝试 Attach
            if ((m_iShmID = shmget(iKey, iShmSize, 0666)) < 0) {

                LOG_ERROR("Attach to share memory {} failed, {}. Now try to touch it ", iKey, strerror(errno));

                m_iShmID = shmget(iKey, 0, 0666);
                if (m_iShmID < 0) {
                    LOG_ERROR("error, touch shm failed, {}. ", strerror(errno));
                    return SHM_ERROR;;
                } else {
                    LOG_ERROR("remove the exist share memory {} ", m_iShmID);
                    if (shmctl(m_iShmID, IPC_RMID, NULL)) {
                        LOG_ERROR("error, remove share memory failed, {} ", strerror(errno));
                        return SHM_ERROR;;
                    }

                    //重新创建
                    m_iShmID = shmget(iKey, iShmSize, IPC_CREAT | IPC_EXCL | 0666);
                    if (m_iShmID < 0) {
                        LOG_ERROR("error, create memory failed, {} ", strerror(errno));
                        return SHM_ERROR;
                    }
                    iRet = CREATE_SUCC;
                }
            } else {
                LOG_DEBUG("attach to share memory {} succeed. ", iKey);
                iRet = ATTACH_SUCC;
            }

        } else {
            LOG_DEBUG("create share memory block, key = {}, id = {}, size = {} ", iKey, m_iShmID, iShmSize);
            iRet = CREATE_SUCC;
        }

        //访问
        if ((m_pShmBuff = (char *) shmat(m_iShmID, NULL, 0)) == NULL) {

            LOG_ERROR("error, access share memory {} failed. ", m_iShmID);
            return SHM_ERROR;
        }

        m_iShmSize = iShmSize;
        m_iShmKey = iKey;

        //初始化共享内存头
        DataHead stDataHead;
        if (iRet == CREATE_SUCC) {

            stDataHead.iBegin = 0;
            stDataHead.iEnd = 0;
            stDataHead.iOffset = sizeof(DataHead);
            stDataHead.iSize = iShmSize - sizeof(DataHead);
            memcpy(m_pShmBuff, (char *) &stDataHead, sizeof(DataHead));
        } else {
            memcpy((char *) &stDataHead, m_pShmBuff, sizeof(DataHead));
            LOG_DEBUG("DataHead:\n\tiSize:{}\n\tiBegin:{}\n\tiEnd:{}\n\tiOffset:{}\n", stDataHead.iSize, stDataHead.iBegin,
                   stDataHead.iEnd, stDataHead.iOffset);

        }
        return iRet;
    }

    int CShm::Detach() {
        int iRet = 0;
        if (m_pShmBuff != NULL) {
            iRet = shmdt(m_pShmBuff);

            m_pShmBuff = NULL;
        }

        return iRet;
    }

    int CShm::Delete() {
        int iRet = 0;
        if (m_pShmBuff != NULL) {
            iRet = shmctl(m_iShmID, IPC_RMID, 0);

            m_pShmBuff = NULL;
        }

        return iRet;
    }


    CShmBuffQueue::CShmBuffQueue(char *pShmBuff)
    {

        m_pDataHead = (DataHead*)pShmBuff;
        m_pBuff = pShmBuff + sizeof(DataHead);
    }
    CShmBuffQueue::~CShmBuffQueue()
    {
        m_pBuff = NULL;
        m_pDataHead = NULL;
    }
    void CShmBuffQueue::PrintHead()
    {
        LOG_DEBUG("DataHead:\n\tiSize:{}\n\tiBegin:{}\n\tiEnd:{}\n\tiOffset:{}\n", m_pDataHead->iSize, m_pDataHead->iBegin,m_pDataHead->iEnd, m_pDataHead->iOffset);
    }

    int CShmBuffQueue::GetDataUnit(char *pOut, short *pnOutLen)
    {
        int iLeftSize = 0;

        int iBegin = -1;
        int iEnd = -1;

        //参数判断
        if((NULL == pOut) || (NULL == pnOutLen))
        {
            return -1;
        }

        if( m_pDataHead->iOffset <= 0 || m_pDataHead->iSize <= 0 )
        {
            return -1;
        }

        //取首、尾
        iBegin = m_pDataHead->iBegin;
        iEnd   = m_pDataHead->iEnd;


        //空
        if( iBegin == iEnd )
        {
            *pnOutLen = 0;
            return 0;
        }

        //剩余缓冲大小,小于包长度字节数,错误返回
        iLeftSize = GetLeftSize();
        if( iLeftSize < sizeof(short) )
        {
            //重置首尾
            *pnOutLen = 0;
            m_pDataHead->iBegin = 0;
            m_pDataHead->iEnd = 0;
            return -3;
        }

        char *pbyCodeBuf = m_pBuff;
        char *pTempSrc = NULL;
        char *pTempDst = NULL;

        pTempDst = (char *)pnOutLen;
        pTempSrc = (char *)&pbyCodeBuf[0];

        //包长度编码
        for(int i = 0; i < sizeof(short); i++ )
        {
            pTempDst[i] = pTempSrc[iBegin];
            iBegin = (iBegin+1) % m_pDataHead->iSize;
        }

        //数据包长度非法
        if(((*pnOutLen) > GetUsedSize())  || (*pnOutLen < 0))
        {
            *pnOutLen = 0;
            m_pDataHead->iBegin = 0;
            m_pDataHead->iEnd = 0;
            return -3;
        }

        pTempDst = pOut;

        //首小于尾
        if(iBegin < iEnd )
        {
            memcpy((void *)pTempDst, (const void *)&pTempSrc[iBegin], (size_t)(*pnOutLen));
        }
        else
        {
            //首大于尾且出现分段，则分段拷贝
            int iRightLeftSize = m_pDataHead->iSize - iBegin;
            if( iRightLeftSize < *pnOutLen)
            {

                memcpy((void *)pTempDst, (const void *)&pTempSrc[iBegin], iRightLeftSize);
                pTempDst += iRightLeftSize;
                memcpy((void *)pTempDst, (const void *)&pTempSrc[0], (size_t)(*pnOutLen - iRightLeftSize));
            }
                //否则，直接拷贝
            else
            {
                memcpy((void *)pTempDst, (const void *)&pTempSrc[iBegin], (size_t)(*pnOutLen));
            }
        }

        //变更begin
        iBegin = (iBegin + (*pnOutLen)) % m_pDataHead->iSize;

        m_pDataHead->iBegin = iBegin;

        return iBegin;
    }

    int CShmBuffQueue::PutDataUnit(const char *pIn, short nInLen)
    {
        int iLeftSize = 0;
        int iBegin = -1;
        int iEnd = -1;

        //参数判断
        if((NULL == pIn) || (nInLen <= 0))
        {
            return -1;
        }

        if( m_pDataHead->iOffset <= 0 || m_pDataHead->iSize <= 0 )
        {
            return -1;
        }


        //首先判断是已满
        if(IsFull())
        {
            return -2;
        }

        //取首、尾
        iBegin = m_pDataHead->iBegin;
        iEnd   = m_pDataHead->iEnd;

        //缓冲区异常判断处理
        if( iBegin < 0 || iBegin >= m_pDataHead->iSize
            || iEnd < 0 || iEnd >= m_pDataHead->iSize )
        {

            //重置
            m_pDataHead->iBegin = 0;
            m_pDataHead->iEnd = 0;


            return -3;
        }

        //剩余缓冲大小小于新来的数据,溢出了,返回错误
        iLeftSize = GetLeftSize();

        if((int)(nInLen + sizeof(short)) > iLeftSize)
        {
            return -2;
        }


        //数据首指针
        char *pbyCodeBuf = m_pBuff;

        char *pTempSrc = NULL;
        char *pTempDst = NULL;


        pTempDst = &pbyCodeBuf[0];
        pTempSrc = (char *)&nInLen;

        //包的长度编码
        for(int i = 0; i < sizeof(nInLen); i++ )
        {
            pTempDst[iEnd] = pTempSrc[i];
            iEnd = (iEnd+1) % m_pDataHead->iSize;
        }

        //首大于尾
        if( iBegin > iEnd )
        {
            memcpy((void *)&pbyCodeBuf[iEnd], (const void *)pIn, (size_t)nInLen);
        }
        else
        {
            //首小于尾,本包长大于右边剩余空间,需要分两段循环放到buff存放
            if((int)nInLen > (m_pDataHead->iSize - iEnd))
            {
                //右边剩余buff
                int iRightLeftSize = m_pDataHead->iSize - iEnd;
                memcpy((void *)&pbyCodeBuf[iEnd], (const void *)&pIn[0], (size_t)iRightLeftSize );
                memcpy((void *)&pbyCodeBuf[0],(const void *)&pIn[iRightLeftSize], (size_t)(nInLen - iRightLeftSize));
            }
                //右边剩余buff够了，直接put
            else
            {
                memcpy((void *)&pbyCodeBuf[iEnd], (const void *)&pIn[0], (size_t)nInLen);
            }
        }

        //更新尾偏移
        iEnd = (iEnd + nInLen) % m_pDataHead->iSize;
        m_pDataHead->iEnd = iEnd;

        return iEnd;
    }

    bool CShmBuffQueue::IsFull()
    {
        int iLeftSize = 0;
        iLeftSize = GetLeftSize();

        if( iLeftSize > 0 )
        {
            return false;
        }
        else
        {
            return false;
        }
    }
    int CShmBuffQueue::GetLeftSize()
    {
        int iRetSize = 0;
        int iBegin = -1;
        int iEnd = -1;

        iBegin = m_pDataHead->iBegin;
        iEnd = m_pDataHead->iEnd;

        //首尾相等
        if(iBegin == iEnd)
        {
            iRetSize = m_pDataHead->iSize;
        }
            //首大于尾
        else if(iBegin > iEnd)
        {
            iRetSize = iBegin - iEnd;
        }
            //首小于尾
        else
        {
            iRetSize = m_pDataHead->iSize - iEnd + iBegin;
        }

        //最大长度减去预留部分长度，保证首尾不会相接
        iRetSize -= BUFF_RESERVE_LENGTH;

        return iRetSize;
    }

    int CShmBuffQueue::GetUsedSize()
    {
        int iLeftSize = GetLeftSize();
        if(iLeftSize > 0)
        {
            return m_pDataHead->iSize - iLeftSize;
        }
        else
        {
            return m_pDataHead->iSize;
        }
    }













};


