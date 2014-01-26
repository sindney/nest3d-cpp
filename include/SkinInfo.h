#ifndef N3D_SKININFO_H
#define N3D_SKININFO_H

#include <string>

#include "Joint.h"

namespace nest
{
	class SkinInfo
	{
	public:

		std::vector<Joint*> joints;

		Joint *root;

		bool software;

		SkinInfo() : root(NULL), software(true) {}

		~SkinInfo()
		{
			if(root != NULL) delete root;
		}
	};
}

#endif