#include <iterator>

#include "EventActor.h"

namespace nest
{
	using namespace std;

	EventDispatcher::~EventDispatcher()
	{
		map<string, EventActor*>::iterator i;
		for(i = actors.begin(); i != actors.end(); i++)
		{
			if(i->second->autoClean) delete i->second;
			i->second = NULL;
			actors.erase(i);
		}
	}

	void EventDispatcher::dispatch(const string *event)
	{
		map<string, EventActor*>::iterator i = actors.find(*event);
		if(i != actors.end())
		{
			EventActor *actor = static_cast<EventActor*>(i->second);
			actor->respond(event, this);
			if(actor->actOnce) 
			{
				if(actor->autoClean) delete actor;
				i->second = NULL;
				actors.erase(i);
			}
		}
	}

	bool EventDispatcher::addEventActor(const string *event, EventActor *actor)
	{
		if(!hasEventActor(event))
		{
			actors.insert(map<string, EventActor*>::value_type(*event, actor));
			return true;
		}
		return false;
	}

	bool EventDispatcher::hasEventActor(const string *event)
	{
		map<string, EventActor*>::iterator i = actors.find(*event);
		return i != actors.end();
	}

	EventActor *EventDispatcher::removeEventActor(const string *event)
	{
		EventActor *result = NULL;
		map<string, EventActor*>::iterator i = actors.find(*event);
		if(i != actors.end())
		{
			result = i->second;
			actors.erase(i);
		}
		return result;
	}
}