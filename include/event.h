// Refer to: http://www.liranuna.com/implementing-eventdispatcher-in-cpp/
#ifndef EVENT_H
#define EVENT_H

#include <string>

namespace nest
{	
	class eventdispatcher;
	class event
	{
	public:

		const std::string type;

		const eventdispatcher &target;

		event(const std::string &type, eventdispatcher &target) : type(type), target(target) {}
	};
}

#endif