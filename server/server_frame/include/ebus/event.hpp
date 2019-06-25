

#pragma once

#include "object.hpp"

#include <typeindex>
#include <typeinfo>
#include <vector>
#include <stdexcept>

/**
 * \brief The base event class, all events inherit from this class
 */
namespace ebus {

	class Event : public Object {
	public:
		/**
         * \brief Default constructor
         *
         * @param typeIndex The type ID of the inherited class
         * @param sender The sender of the event
         */
		Event(Object& sender)
				:
				sender(sender),
				canceled(false)
		{
		}

		/**
         * \brief Empty virtual destructor
         */
		virtual ~Event() { }

		/**
         * \brief Gets the source object for this event
         *
         * @return The event sender
         */
		Object& getSender()
		{
			return sender;
		}

		/**
         * \brief Gets whether the event has been canceled
         *
         * @return true if the event is canceled
         */
		bool getCanceled()
		{
			return canceled;
		}

		/**
         * \brief Sets the canceled status for the event
         *
         * @param canceled Whether the even is canceled or not
         */
		void setCanceled(bool canceled)
		{
			this->canceled = canceled;
		}

	private:
		Object& sender;
		bool canceled;

	};
};

