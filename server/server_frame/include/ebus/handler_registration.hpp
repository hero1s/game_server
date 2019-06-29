#pragma once

#include "object.hpp"

namespace ebus {
/**
 * \brief Interface that that allows event handlers to be removed from the EventBus
 */
    class HandlerRegistration : public Object {
    public:
        virtual ~HandlerRegistration() { }

        virtual void removeHandler() = 0;
    };

};
