#pragma once

#include "object.hpp"

#include <typeinfo>
#include <type_traits>

namespace ebus {
// Forward declare the Event class
    class Event;

/**
 * \brief Base class of all classes that listen for events
 *
 * For a class to be an event listener, it needs to inherit from EventHandler
 * with the specific event type as the template parameter. A class can inherit from
 * multiple EventHandler base classes each using a different template parameter.
 */
    template<class T>
    class EventHandler {
    public:

        /**
         * \brief Default constructor that enforces the template type
         */
        EventHandler()
        {
            // An error here indicates you're trying to implement EventHandler with a type that is not derived from Event
            static_assert(std::is_base_of<Event, T>::value, "EventHandler<T>: T must be a class derived from Event");
        }

        /**
         * \brief Empty virtual destructor
         */
        virtual ~EventHandler() { }

        /**
         * \brief Pure virtual method for implementing the body of the listener
         *
         * @param The event instance
         */
        virtual void onEvent(T&) = 0;

        /**
         * \brief Dispatches a generic event to the appropriate listener method
         *
         * This method is called by the EventBus and dispatches to the correct method by
         * dynamic casting the event parameter to the template type for this handler.
         *
         * @param e The event to dispatch
         */
        void dispatch(Event& e)
        {
            onEvent(dynamic_cast<T&>(e));
        }
    };
};
