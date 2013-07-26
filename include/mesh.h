#ifndef MESH_H
#define MESH_H

#include "geometry.h"
#include "object3d.h"
#include "shader3d.h"

namespace nest
{
	class mesh : public object3d 
	{
	public:

		bool alphaTest;

		float alphaKey;

		bool castShadows;

		bool cliping;

		bool visible;

		bool faceCulling;

		GLenum face;
		
		aabb bound;

		geometry *geom;

		shader3d *shader;

		mesh(geometry *geom, shader3d *shader) 
		 : alphaTest(false), alphaKey(0.0f), castShadows(false), cliping(true), visible(true), 
		 	faceCulling(true), face(GL_BACK), geom(geom), shader(shader) {}

		~mesh()
		{
			geom = NULL;
			shader = NULL;
		}

		void recompose()
		{
			object3d::recompose();
			geometry::transformAABB(worldMatrix, geom->bound, bound);
		}
	};
}

#endif