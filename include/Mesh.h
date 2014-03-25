#ifndef N3D_MESH_H
#define N3D_MESH_H

#include <vector>

#include "Geometry.h"
#include "Joint.h"
#include "Shader.h"

namespace nest
{
	class SkinInfo
	{
	public:

		std::vector<Joint*> joints;

		Joint *root;

		SkinInfo() : root(NULL) {}

		virtual ~SkinInfo()
		{
			joints.clear();
			if(root != NULL) delete root;
		}
	};

	class Mesh
	{
	public:

		bool faceCulling;

		GLenum face;

		Geometry *geometry;

		SkinInfo *skin;

		/**
		 *	When MeshRender draws any MeshNode.
		 *	<p>He'd find correct shader for specific RenderTarget in this vector by render's int flag.</p>
		 *	
		 *	@see MeshRender
		 */
		std::vector<Shader*> shaders;

		Mesh(Geometry *geometry, SkinInfo *skin)
		 : geometry(geometry), skin(skin), faceCulling(true), face(GL_BACK) {}

		virtual ~Mesh()
		{
			if(geometry != NULL) delete geometry;
			if(skin != NULL) delete skin;
			Shader *shader = NULL;
			while(shaders.size() != 0)
			{
				shader = shaders.back();
				shaders.pop_back();
				delete shader;
			}
		}
	};
}

#endif