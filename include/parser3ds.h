#ifndef N3D_PARSER3DS_H
#define N3D_PARSER3DS_H

#include <vector>

#include "geometry.h"

namespace nest
{
	class parser3ds
	{
	public:

		std::vector<geometry*> result;

		~parser3ds();

		void parse(const char *location);
	};
}

#endif