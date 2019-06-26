#pragma once

#include <list>
#include <typeinfo>
#include <unordered_map>
#include <map>
#include <typeindex>
#include <vector>
#include <stdexcept>

namespace ebus {
    //事件及派发基类
    class Object {
    public:
        Object() { }

        virtual ~Object() { }

        Object(const Object& other) { }
    };

    //事件基类
    class Event : public Object {
    public:
        Event(Object& sender)
                :
                sender(sender),
                canceled(false)
        {
        }

        virtual ~Event() { }

        Object& getSender()
        {
            return sender;
        }

        bool getCanceled()
        {
            return canceled;
        }

        void setCanceled(bool canceled)
        {
            this->canceled = canceled;
        }

    private:
        Object& sender;    //事件触发对象
        bool canceled;    //是否终止事件
    };

    //事件处理handler
    template<class T>
    class EventHandler {
    public:
        EventHandler()
        {
            static_assert(std::is_base_of<Event, T>::value, "EventHandler<T>: T must be a class derived from Event");
        }

        virtual ~EventHandler() { }

        //实现事件处理接口
        virtual void onEvent(T&) = 0;

        void dispatch(Event& e)
        {
            onEvent(dynamic_cast<T&>(e));
        }
    };

    // 异步事件BUS
    class EventBus : public Object {
    public:
        EventBus() { }

        virtual ~EventBus() { }

        static EventBus* const GetInstance()
        {
            if (instance==nullptr) {
                instance = new EventBus();
            }
            return instance;
        }

        //注册事件handler到ebus
        template<class T>
        static bool const AddHandler(EventHandler<T>& handler, Object* sender)
        {
            EventBus* instance = GetInstance();
            std::type_index typeIndex = typeid(T);
            Registrations* registrations = instance->handlers[typeIndex];
            if (registrations==nullptr) {
                registrations = new Registrations();
                auto regMap = new RegMap();
                instance->handlers[typeIndex] = registrations;
                instance->registrations[typeIndex] = regMap;
            }
            EvRegKey key(static_cast<void*>(&handler), sender);
            auto pCur = findEventRegistration(typeIndex, key);
            if (pCur) {
                return false;//重复注册
            }
            EventRegistration* registration = new EventRegistration(static_cast<void*>(&handler), registrations,
                    sender);
            registrations->push_back(registration);
            instance->registrations[typeIndex]->insert(RegMap::value_type(key, registration));
            return true;
        }

        template<class T>
        static bool const RemoveHandler(EventHandler<T>& handler, Object* sender)
        {
            EventBus* instance = GetInstance();
            std::type_index typeIndex = typeid(T);
            Registrations* registrations = instance->handlers[typeIndex];
            if (registrations==nullptr) {
                registrations = new Registrations();
                auto regMap = new RegMap();
                instance->handlers[typeIndex] = registrations;
                instance->registrations[typeIndex] = regMap;
            }
            EvRegKey key(static_cast<void*>(&handler), sender);
            auto pCur = findEventRegistration(typeIndex, key);
            if (pCur) {
                return false;//不存在
            }
            else {
                pCur->removeHandler();
                instance->registrations[typeIndex]->erase(key);
                delete pCur;
            }
            return true;
        }

        //派发事件
        static void FireEvent(Event& e)
        {
            EventBus* instance = GetInstance();
            Registrations* registrations = instance->handlers[typeid(e)];
            if (registrations==nullptr) {
                return;
            }
            for (auto& reg : *registrations) {
                if ((reg->getSender()==nullptr) || (reg->getSender()==&e.getSender())) {
                    static_cast<EventHandler<Event>*>(reg->getHandler())->dispatch(e);
                }
            }
        }

    private:
        static EventBus* instance;

        //检索索引
        struct EvRegKey {
            uint64_t h;
            uint64_t s;

            EvRegKey(void* const handler, void* const sender)
            {
                this->h = (uint64_t) handler;
                this->s = (uint64_t) sender;
            }

            bool operator==(const EvRegKey& erk) const
            {
                return h==erk.h && s==erk.s;
            }

            bool operator<(const EvRegKey& erk) const
            {
                if (h<erk.h)
                    return true;
                else if (h==erk.h)
                    return s<erk.s;
                return false;
            }
        };

        class EventRegistration {
        public:
            typedef std::list<EventRegistration*> Registrations;

            EventRegistration(void* const handler, Registrations* const registrations, Object* const sender)
                    :handler(handler),
                     registrations(registrations),
                     sender(sender),
                     registered(true) { }

            virtual ~EventRegistration() { }

            void* const getHandler()
            {
                return handler;
            }

            Object* const getSender()
            {
                return sender;
            }

            void removeHandler()
            {
                if (registered) {
                    registrations->remove(this);
                    registered = false;
                }
            }

        private:
            void* const handler;
            Registrations* const registrations;
            Object* const sender;
            bool registered;
        };

        static EventRegistration* findEventRegistration(std::type_index& index, EvRegKey& key)
        {
            EventBus* instance = GetInstance();
            auto it = instance->registrations[index]->find(key);
            if (it==instance->registrations[index]->end())return nullptr;
            return it->second;
        }

        using Registrations = std::list<EventRegistration*>;
        using TypeMap = std::unordered_map<std::type_index, std::list<EventRegistration*>*>;
        using RegMap = std::map<EvRegKey, EventRegistration*>;
        using RegMaps = std::unordered_map<std::type_index, std::map<EvRegKey, EventRegistration*>*>;

        TypeMap handlers;
        RegMaps registrations;
    };


};
