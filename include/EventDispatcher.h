#ifndef N3D_EVENTDISPATCHER_H
#define N3D_EVENTDISPATCHER_H

#include <list>
#include <map>
#include <string>

namespace nest
{
	using namespace std;

	#define ACTOR_LIST list<EventActor*>

	#define ACTOR_LIST_MAP map<string, ACTOR_LIST>

	class EventActor;

	class EventDispatcher
	{
	public:

		ACTOR_LIST_MAP actorsMap;

		EventDispatcher() {}

		virtual ~EventDispatcher();
		
		virtual void dispatch(const string *event);

		void addEventActor(const string *event, EventActor *actor);

		bool hasEventActor(const string *event);
		
		bool removeEventActor(const string *event, EventActor *actor);
	};
}

#endif