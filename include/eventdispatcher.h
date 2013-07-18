// Refer to: http://www.liranuna.com/implementing-eventdispatcher-in-cpp/
#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <list>
#include <map>
#include <string>

#include "event.h"

namespace nest
{
	typedef void (*callback)(const event &);

	using namespace std;

	class eventdispatcher
	{
	private:

		map< const string, map< int, list<callback> > > handlers;

	public:
		
		virtual ~eventdispatcher()
		{
			handlers.clear();
		}

		void addEventListener(const string &type, callback listener, int priority = 0)
		{
			handlers[type][priority].push_back(listener);
		}

		bool hasEventListener(const string &type) const
		{
			return (handlers.find(type) != handlers.end());
		}

		virtual void dispatchEvent(const event &evt)
		{
			if(!hasEventListener(evt.type)) return;

			map< int, list<callback> > &all = handlers[evt.type];
			for(map< int, list<callback> >::reverse_iterator i = all.rbegin(); i != all.rend(); ++i) {
				list<callback> &functions = i->second;
				for(list<callback>::iterator f = functions.begin(); f != functions.end(); ++f) (*f)(evt);
			}
		}

		void removeEventListener(const string &type, callback listener)
		{
			if(!hasEventListener(type)) return;

			map< int, list<callback> > &all = handlers[type];
			map< int, list<callback> >::iterator i = all.begin();

			while(i != all.end())
			{
				i->second.remove(listener);
				if (i->second.empty())
				{
					all.erase(i++);
				} 
				else 
				{
					++i;
				}
			}

			if(all.empty()) handlers.erase(type);
		}
	};
}

#endif