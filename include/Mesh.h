#ifndef N3D_MESH_H
#define N3D_MESH_H

#include <vector>

#include "Geometry.h"
#include "Shader.h"
#include "SkinInfo.h"

namespace nest
{
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