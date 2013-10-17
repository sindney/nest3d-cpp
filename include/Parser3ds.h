#ifndef N3D_PARSER3DS_H
#define N3D_PARSER3DS_H

#include <vector>

#include "Geometry.h"

namespace nest
{
	class Parser3ds
	{
	public:

		std::vector<Geometry*> result;

		~Parser3ds();

		void parse(const char *location);
	};
}

#endif