#ifndef N3D_EVENTDISPATCHER_H
#define N3D_EVENTDISPATCHER_H

#include <list>
#include <map>
#include <string>

namespace nest
{
	using namespace std;

#ifndef ACTOR_LIST
	#define ACTOR_LIST list<EventActor*>
#endif

#ifndef ACTOR_LIST_MAP
	#define ACTOR_LIST_MAP map<string, ACTOR_LIST>
#endif

	class EventActor;

	/**
	 *	Extend this to create ur own events.
	 */
	class Event
	{
	public:

		const string name;

		Event(string name) : name(name) {}
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

		void addEventActor(const string &event, EventActor *actor);

		bool hasEventActor(const string &event);
		
		bool removeEventActor(const string &event, EventActor *actor);
	};
}

#endif