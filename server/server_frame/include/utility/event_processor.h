
#pragma once

#include "basic_types.h"
#include <map>

class EventProcessor;

// Note. All times are in milliseconds here.

class BasicEvent {
    friend class EventProcessor;

    enum class AbortState : uint8_t {
        STATE_RUNNING,
        STATE_ABORT_SCHEDULED,
        STATE_ABORTED
    };

public:
    BasicEvent()
            : m_abortState(AbortState::STATE_RUNNING), m_addTime(0), m_execTime(0) {}

    virtual ~BasicEvent() {}                           // override destructor to perform some actions on event removal

    // this method executes when the event is triggered
    // return false if event does not want to be deleted
    // e_time is execution time, p_time is update interval
    virtual bool Execute(uint64_t /*e_time*/, uint32_t /*p_time*/) { return true; }

    virtual bool IsDeletable() const { return true; }   // this event can be safely deleted

    virtual void Abort(uint64_t /*e_time*/) {}           // this method executes when the event is aborted

    // Aborts the event at the next update tick
    void ScheduleAbort();

private:
    void SetAborted();

    bool IsRunning() const { return (m_abortState == AbortState::STATE_RUNNING); }

    bool IsAbortScheduled() const { return (m_abortState == AbortState::STATE_ABORT_SCHEDULED); }

    bool IsAborted() const { return (m_abortState == AbortState::STATE_ABORTED); }

    AbortState m_abortState;                            // set by externals when the event is aborted, aborted events don't execute

    // these can be used for time offset control
    uint64_t m_addTime;                                   // time when the event was added to queue, filled by event handler
    uint64_t m_execTime;                                  // planned time of next execution, filled by event handler
};

class EventProcessor {
public:
    EventProcessor() : m_time(0) {}

    ~EventProcessor();

    void Update(uint32_t p_time);

    void KillAllEvents(bool force);

    void AddEvent(BasicEvent *Event, uint64_t e_time, bool set_addtime = true);

    uint64_t CalculateTime(uint64_t t_offset) const;

protected:
    uint64_t m_time;
    std::multimap<uint64_t, BasicEvent *> m_events;
};

