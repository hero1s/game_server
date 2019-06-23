
#pragma once

#include "utility/basic_types.h"
#include "time/time_function.h"
#include <time.h>

/*************************************************************/
class CNumberStream {
protected:
    /*************************************************************/
#pragma pack(push, 1)
    union _U_NUMBER {
      struct {
        uint16_t _order;    //序号
        uint32_t _time;    //时间
        uint16_t _flag;    //标记ID[必须小于32767后15位]
      } _STREAM;

      uint64_t _stream;/*流水号*/
    };
#pragma pack(pop)
    /*************************************************************/
protected:
    _U_NUMBER m_uNumber;
    uint32_t    m_uMaxTime;            /*时间*/

public:
    CNumberStream(uint16_t _flag = 0)
    {
        m_uMaxTime = 0;
        m_uNumber._stream = 0;
        setFlag(_flag);
    }

    virtual~CNumberStream() { }

public:
    inline void setFlag(uint16_t _flag)
    {
        m_uNumber._STREAM._flag = _flag;
    }

public:
    inline uint64_t lastStreamNumber()
    {
        return m_uNumber._stream;
    }

    /*----->{ 产生事件流水号 }*/
    inline uint64_t buildStreamNumber(uint16_t _flag = 0)
    {


        /*设置标志*/
        if (_flag)
            setFlag(_flag);

        m_uNumber._STREAM._time = (uint32_t) getTime();
        /*设置时间改变标志*/
        if (m_uMaxTime>m_uNumber._STREAM._time)
        {
            m_uNumber._STREAM._flag |= 0x8000;
        }
        else
        {
            m_uMaxTime = m_uNumber._STREAM._time;
            m_uNumber._STREAM._flag &= 0x7FFF;
        }

        /*编号*/
        m_uNumber._STREAM._order++;

        return m_uNumber._stream;
    }
};



