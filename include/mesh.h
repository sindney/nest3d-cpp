#ifndef N3D_MESH_H
#define N3D_MESH_H

#include "geometry.h"
#include "object3d.h"
#include "shader3d.h"

namespace nest
{
	class ocnode;
	
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

		ocnode *node;

		mesh(geometry *geom, shader3d *shader);

		~mesh();

		void recompose();
	};
}

#endif