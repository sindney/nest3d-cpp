#ifndef N3D_EVENTDISPATCHER_H
#define N3D_EVENTDISPATCHER_H

#include <map>
#include <string>

namespace nest
{
	using namespace std;

	class EventActor;

	class EventDispatcher
	{
	public:

		map<string, EventActor*> actors;

		EventDispatcher() {}

		virtual ~EventDispatcher();
		
		virtual void dispatch(const string *event);

		bool addEventActor(const string *event, EventActor *actor);

		bool hasEventActor(const string *event);

		EventActor *removeEventActor(const string *event);
	};
}

#endif