#ifndef N3D_MESH_H
#define N3D_MESH_H

#include <vector>

#include "Geometry.h"
#include "Object3d.h"
#include "Shader.h"

namespace nest
{
	class ORDraw;

	class OcNode;
	
	class Mesh : public Object3d 
	{
	public:

		bool alphaTest;

		float alphaKey;

		bool cliping;

		bool visible;

		bool faceCulling;

		GLenum face;

		OcNode *node;

		ORDraw *draw;

		/**
		 *	Note one Geometry can be used in several Meshes.
		 *	<p>So you need to take care of it's delocation.</p>
		 */
		Geometry *geometry;

		/**
		 *	Note one Shader can be used in several Meshes.
		 *	<p>So you need to take care of it's delocation.</p>
		 */
		Shader *shader;

		AABB bound;

		Mesh(ORDraw *draw, Geometry *geometry, Shader *shader);

		~Mesh();

		void recompose();
	};
}

#endif