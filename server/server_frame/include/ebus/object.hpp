#pragma once

namespace ebus {
/**
 * \brief Root class of the type hierarchy
 *
 * All events and event handlers derive from this class
 */
	class Object {
	public:
		/**
         * \brief Default empty constructor
         */
		Object() { }

		/**
         * Empty virtual destructor
         */
		virtual ~Object() { }

		/**
         * Default empty copy constructor
         * @param other The instance to copy from
         */
		Object(const Object& other) { }
	};
};
