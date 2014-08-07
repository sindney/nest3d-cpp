#include <iterator>

#include "EventActor.h"

namespace nest
{
	using namespace std;

	EventDispatcher::~EventDispatcher()
	{
		ACTOR_LIST_MAP::iterator i = actorsMap.begin();
		ACTOR_LIST::iterator j;
		ACTOR_LIST *actors = NULL;
		EventActor *actor = NULL;
		while(i != actorsMap.end())
		{
			actors = static_cast<ACTOR_LIST*>(&i->second);
			j = actors->begin();
			while(j != actors->end())
			{
				actor = static_cast<EventActor*>(*j);
				if(actor->autoClean) delete actor;
				actors->erase(j);
				j++;
			}
			actorsMap.erase(i);
			i++;
		}
	}

	void EventDispatcher::dispatch(const Event *event)
	{
		ACTOR_LIST_MAP::iterator i = actorsMap.find(event->name);
		if(i != actorsMap.end())
		{
			ACTOR_LIST *actors = static_cast<ACTOR_LIST*>(&i->second);
			ACTOR_LIST::iterator j = actors->begin();
			EventActor *actor = NULL;
			while(j != actors->end())
			{
				actor = static_cast<EventActor*>(*j);
				actor->respond(event, this);
				if(actor->actOnce)
				{
					if(actor->autoClean) delete actor;
					actors->erase(j);
				}
				j++;
			}
			if(actors->size() == 0)
				actorsMap.erase(i);
		}
	}

	void EventDispatcher::addEventActor(const string &event, EventActor *actor)
	{
		ACTOR_LIST_MAP::iterator i = actorsMap.find(event);
		if(i == actorsMap.end()) 
		{
			ACTOR_LIST actors;
			actors.push_back(actor);
			actorsMap.insert(ACTOR_LIST_MAP::value_type(event, actors));
		}
		else
			i->second.push_back(actor);
	}

	bool EventDispatcher::hasEventActor(const string &event)
	{
		ACTOR_LIST_MAP::iterator i = actorsMap.find(event);
		return i != actorsMap.end();
	}

	bool EventDispatcher::removeEventActor(const string &event, EventActor *actor)
	{
		bool result = false;
		ACTOR_LIST_MAP::iterator i = actorsMap.find(event);
		if(i != actorsMap.end())
		{
			ACTOR_LIST *actors = static_cast<ACTOR_LIST*>(&i->second);
			ACTOR_LIST::iterator j;
			for(j = actors->begin(); j != actors->end(); ++j)
			{
				if(actor == static_cast<EventActor*>(*j))
				{
					actors->erase(j);
					result = true;
					break;
				}
			}
			if(actors->size() == 0) actorsMap.erase(i);
		}
		return result;
	}
}