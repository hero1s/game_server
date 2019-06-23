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
            �ɹ����ع����ڴ�ı�ʶ�������ɹ�����-1��errno�������ԭ��
            EINVAL           ����sizeС��SHMMIN�����SHMMAX��
            EEXIST           Ԥ����key���µĹ����ڴ棬���Ѿ����ڡ�
            EIDRM            ����key���µĹ����ڴ��Ѿ�ɾ����
            ENOSPC        ������ϵͳ�������Ĺ����ڴ�����ֵ(SHMALL )��
            ENOENT        ����key��ָ�Ĺ����ڴ治���ڣ�����shmflgҲδ��IPC_CREATλ��
            EACCES        û��Ȩ�ޡ�
            ENOMEM       �����ڴ治�㡣

        */

        int iRet = SHM_ERROR;
        m_iShmID = shmget(iKey, iShmSize, IPC_CREAT | IPC_EXCL | 0666);
        if (m_iShmID < 0) {
            if (errno != EEXIST) {
                LOG_ERROR("Alloc share memory failed, {} ", strerror(errno));
                return SHM_ERROR;
            }
            //�����Ѿ�����,���� Attach
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

                    //���´���
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

        //����
        if ((m_pShmBuff = (char *) shmat(m_iShmID, NULL, 0)) == NULL) {

            LOG_ERROR("error, access share memory {} failed. ", m_iShmID);
            return SHM_ERROR;
        }

        m_iShmSize = iShmSize;
        m_iShmKey = iKey;

        //��ʼ�������ڴ�ͷ
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

        //�����ж�
        if((NULL == pOut) || (NULL == pnOutLen))
        {
            return -1;
        }

        if( m_pDataHead->iOffset <= 0 || m_pDataHead->iSize <= 0 )
        {
            return -1;
        }

        //ȡ�ס�β
        iBegin = m_pDataHead->iBegin;
        iEnd   = m_pDataHead->iEnd;


        //��
        if( iBegin == iEnd )
        {
            *pnOutLen = 0;
            return 0;
        }

        //ʣ�໺���С,С�ڰ������ֽ���,���󷵻�
        iLeftSize = GetLeftSize();
        if( iLeftSize < sizeof(short) )
        {
            //������β
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

        //�����ȱ���
        for(int i = 0; i < sizeof(short); i++ )
        {
            pTempDst[i] = pTempSrc[iBegin];
            iBegin = (iBegin+1) % m_pDataHead->iSize;
        }

        //���ݰ����ȷǷ�
        if(((*pnOutLen) > GetUsedSize())  || (*pnOutLen < 0))
        {
            *pnOutLen = 0;
            m_pDataHead->iBegin = 0;
            m_pDataHead->iEnd = 0;
            return -3;
        }

        pTempDst = pOut;

        //��С��β
        if(iBegin < iEnd )
        {
            memcpy((void *)pTempDst, (const void *)&pTempSrc[iBegin], (size_t)(*pnOutLen));
        }
        else
        {
            //�״���β�ҳ��ֶַΣ���ֶο���
            int iRightLeftSize = m_pDataHead->iSize - iBegin;
            if( iRightLeftSize < *pnOutLen)
            {

                memcpy((void *)pTempDst, (const void *)&pTempSrc[iBegin], iRightLeftSize);
                pTempDst += iRightLeftSize;
                memcpy((void *)pTempDst, (const void *)&pTempSrc[0], (size_t)(*pnOutLen - iRightLeftSize));
            }
                //����ֱ�ӿ���
            else
            {
                memcpy((void *)pTempDst, (const void *)&pTempSrc[iBegin], (size_t)(*pnOutLen));
            }
        }

        //���begin
        iBegin = (iBegin + (*pnOutLen)) % m_pDataHead->iSize;

        m_pDataHead->iBegin = iBegin;

        return iBegin;
    }

    int CShmBuffQueue::PutDataUnit(const char *pIn, short nInLen)
    {
        int iLeftSize = 0;
        int iBegin = -1;
        int iEnd = -1;

        //�����ж�
        if((NULL == pIn) || (nInLen <= 0))
        {
            return -1;
        }

        if( m_pDataHead->iOffset <= 0 || m_pDataHead->iSize <= 0 )
        {
            return -1;
        }


        //�����ж�������
        if(IsFull())
        {
            return -2;
        }

        //ȡ�ס�β
        iBegin = m_pDataHead->iBegin;
        iEnd   = m_pDataHead->iEnd;

        //�������쳣�жϴ���
        if( iBegin < 0 || iBegin >= m_pDataHead->iSize
            || iEnd < 0 || iEnd >= m_pDataHead->iSize )
        {

            //����
            m_pDataHead->iBegin = 0;
            m_pDataHead->iEnd = 0;


            return -3;
        }

        //ʣ�໺���СС������������,�����,���ش���
        iLeftSize = GetLeftSize();

        if((int)(nInLen + sizeof(short)) > iLeftSize)
        {
            return -2;
        }


        //������ָ��
        char *pbyCodeBuf = m_pBuff;

        char *pTempSrc = NULL;
        char *pTempDst = NULL;


        pTempDst = &pbyCodeBuf[0];
        pTempSrc = (char *)&nInLen;

        //���ĳ��ȱ���
        for(int i = 0; i < sizeof(nInLen); i++ )
        {
            pTempDst[iEnd] = pTempSrc[i];
            iEnd = (iEnd+1) % m_pDataHead->iSize;
        }

        //�״���β
        if( iBegin > iEnd )
        {
            memcpy((void *)&pbyCodeBuf[iEnd], (const void *)pIn, (size_t)nInLen);
        }
        else
        {
            //��С��β,�����������ұ�ʣ��ռ�,��Ҫ������ѭ���ŵ�buff���
            if((int)nInLen > (m_pDataHead->iSize - iEnd))
            {
                //�ұ�ʣ��buff
                int iRightLeftSize = m_pDataHead->iSize - iEnd;
                memcpy((void *)&pbyCodeBuf[iEnd], (const void *)&pIn[0], (size_t)iRightLeftSize );
                memcpy((void *)&pbyCodeBuf[0],(const void *)&pIn[iRightLeftSize], (size_t)(nInLen - iRightLeftSize));
            }
                //�ұ�ʣ��buff���ˣ�ֱ��put
            else
            {
                memcpy((void *)&pbyCodeBuf[iEnd], (const void *)&pIn[0], (size_t)nInLen);
            }
        }

        //����βƫ��
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

        //��β���
        if(iBegin == iEnd)
        {
            iRetSize = m_pDataHead->iSize;
        }
            //�״���β
        else if(iBegin > iEnd)
        {
            iRetSize = iBegin - iEnd;
        }
            //��С��β
        else
        {
            iRetSize = m_pDataHead->iSize - iEnd + iBegin;
        }

        //��󳤶ȼ�ȥԤ�����ֳ��ȣ���֤��β�������
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


