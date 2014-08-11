#ifndef N3D_EVENTDISPATCHER_H
#define N3D_EVENTDISPATCHER_H

#include <list>
#include <map>
#include <string>

namespace nest
{
#ifndef ACTOR_LIST
	#define ACTOR_LIST std::list<EventActor*>
#endif

#ifndef ACTOR_LIST_MAP
	#define ACTOR_LIST_MAP std::map<std::string, ACTOR_LIST>
#endif

	class EventActor;

	/**
	 *	Extend this to create ur own events.
	 */
	class Event
	{
	public:

		const std::string name;

		Event(std::string name) : name(name) {}
	};

	class EventDispatcher
	{
	public:

		ACTOR_LIST_MAP actorsMap;

		EventDispatcher() {}

		virtual ~EventDispatcher();
		
		/**
		 *	Broadcast event, events are ided by their names.
		 */
		virtual void dispatch(const Event *event);

		void addEventActor(const std::string &event, EventActor *actor);

		bool hasEventActor(const std::string &event);
		
		bool removeEventActor(const std::string &event, EventActor *actor);
	};
}

#endif