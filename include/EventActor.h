#ifndef N3D_EVENTACTOR_H
#define N3D_EVENTACTOR_H

#include "EventDispatcher.h"

namespace nest
{
	class EventActor
	{
	public:

		bool actOnce, autoClean;

		/**
		 *	@param actOnce If set to true, then this actor is erased from EventDispatcher after triggered.
		 *	@param autoClean If set to true, then this actor will be deleted after it's erased from EventDispatcher.
		 */
		EventActor(bool actOnce = false, bool autoClean = true)
		 : actOnce(actOnce), autoClean(autoClean) {}

		virtual ~EventActor() {}

		/**
		 *	@param event Target event to respond.
		 *	@param dispatcher The EventDispatcher that triggers target event.
		 */
		virtual void respond(const Event *event, EventDispatcher *dispatcher) = 0;
	};
}

#endif