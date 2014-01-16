#ifndef N3D_AABB_H
#define N3D_AABB_H

#include "Vector4.h"

/**
 *	\mainpage 
 *	Nest3d is a cross platform opengl 3d engine in cpp.
 *	
 *	Copyright (C) 2013 - 2100 Nest3D Team.
 *	
 *	Permission is hereby granted, free of charge, to any person obtaining a copy of
 *	this software and associated documentation files (the "Software"), to deal in
 *	the Software without restriction, including without limitation the rights to
 *	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *	of the Software, and to permit persons to whom the Software is furnished to do
 *	so, subject to the following conditions:
 *	
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *	
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 *	
 *	http://sindney.com/project/nest3d-cpp
 *	
 *	http://github.com/sindney/nest3d-cpp
 */

namespace nest
{
	class AABB
	{
	public:

		Vector4 max, min;

		AABB &operator = (const AABB &a) 
		{
			max = a.max;
			min = a.min;
			return *this;
		}

		/**
		 *	Calculate target vertex array's bounding-box.
		 *	
		 *	@param bound Result bounding-box.
		 *	@param vertexData Vertex array pointer.
		 *	@param vertexDataSize Vertex array size.
		 */
		static void configure(AABB &bound, const float *vertexData, const int vertexDataSize);

	};
}

#endif