#ifndef N3D_MESHPARSER_H
#define N3D_MESHPARSER_H

#include <string>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace nest
{
	class AnimationClip;

	class AnimationRig;

	class Joint;

	class Mesh;

	class Matrix4;

	/**
	 *	If you get vector of animation clips from this parser.
	 *	<p>Then you'd create AnimRig and bind it to mesh's joint tree to control the mesh's joints.</p>
	 *	<p>So you can use AnimPlayer to play AnimClips, the result is represented as a pose vector.</p>
	 *	<p>You can also blend poses together to generate the final rig.</p>
	 */
	class MeshParser
	{
	public:

		std::vector<AnimationClip*> animations;

		std::vector<Mesh*> meshes;

		/**
		 *	Error detail stores here.
		 */
		std::string error;

		/**
		 *	0 - 4
		 */
		int jointPerVertex;

		MeshParser() : jointPerVertex(4) {}

		~MeshParser()
		{
			animations.clear();
			meshes.clear();
			error.clear();
		}

		/**
		 *	I suggest the following flags for model parsing.
		 *	<p>aiProcess_ValidateDataStructure, aiProcess_Triangulate</p>
		 *	<p>aiProcess_JoinIdenticalVertices, aiProcess_SortByPType</p>
		 *	<p>See Assimp::Importer::ReadFile(pFile, pFlags).</p>
		 *	
		 *	@param file model file path.
		 *	@param flags Options to control the decoding process.
		 *	
		 *	@return Returns false if there's something wrong here. Check MeshParser.error for details.
		 */
		bool parse(const std::string &file, unsigned int flags);

	private:

		void readAnimation(const aiScene *scene, const aiAnimation *anim);

		void readMesh(const aiScene *scene, const aiMesh *meshNode);

		Joint *findJointFromVector(aiString name, std::vector<Joint*> &joints);

		void copyMatrix(aiMatrix4x4 *from, Matrix4 *to);
	};
}

#endif