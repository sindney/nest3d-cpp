#ifndef N3D_MESH_H
#define N3D_MESH_H

#include <iterator>
#include <map>

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

		std::string name;

		bool faceCulling;

		GLenum face;

		Geometry *geometry;

		SkinInfo *skin;

		/**
		 *	When MeshRender draws any MeshNode.
		 *	<p>He'd find correct shader for specific RenderTarget in this map.</p>
		 *	
		 *	@see Geometry
		 *	@see MeshRender
		 */
		std::map<std::string, Shader*> shaderMap;

		Mesh(Geometry *geometry, SkinInfo *skin)
		 : geometry(geometry), skin(skin), faceCulling(true), face(GL_BACK) {}

		virtual ~Mesh()
		{
			if(geometry != NULL) delete geometry;
			if(skin != NULL) delete skin;
			std::map<std::string, Shader*>::iterator it;
			for(it = shaderMap.begin(); it != shaderMap.end(); ++it)
				delete it->second;
		}
	};
}

#endif