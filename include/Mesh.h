#ifndef N3D_MESH_H
#define N3D_MESH_H

#include <iterator>
#include <map>

#include "AnimationRig.h"
#include "Geometry.h"
#include "Joint.h"
#include "Shader.h"

namespace nest
{
	typedef struct
	{
		Shader *shader;

		/**
		 *	Delocation flag. True for auto delete.
		 */
		bool flag;

	} ShaderInfo;

	class SkinInfo
	{
	public:

		AnimationRig animRig;

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

		/**
		 *	Delocation flag. True for auto delete.
		 */
		bool geomFlag;

		SkinInfo *skin;

		/**
		 *	When MeshRender draws any MeshNode.
		 *	<p>He'd find correct shader for specific RenderTarget in this map.</p>
		 *	
		 *	@see Geometry
		 *	@see MeshRender
		 */
		std::map<std::string, ShaderInfo> shaderMap;

		Mesh(Geometry *geometry, SkinInfo *skin, bool geomFlag = true)
		 : geometry(geometry), skin(skin), faceCulling(true), face(GL_BACK), geomFlag(geomFlag) {}

		virtual ~Mesh();

		/**
		 *	Bind shader to this mesh. So we can use in in glsl.
		 *	
		 *	@param flag Delocation flag. True for auto delete.
		 *	
		 *	@return Returns false when there's a name conflict.
		 */
		bool bindShader(std::string name, Shader *shader, bool flag = true);

		bool bindShader(std::string name, ShaderInfo sInfo);

		bool unbindShader(std::string name, ShaderInfo *sInfo = NULL);
	};
}

#endif