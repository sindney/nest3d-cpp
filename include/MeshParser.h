#ifndef N3D_MESHPARSER_H
#define N3D_MESHPARSER_H

#include <string>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace nest
{
	using namespace std;

	class AnimationSet;

	class Joint;

	class Mesh;

	class Matrix4;

	class MeshParser
	{
	public:

		vector<AnimationSet*> animations;

		vector<Mesh*> meshes;

		/**
		 *	Error detail stores here.
		 */
		string error;

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
		bool parse(const string &file, unsigned int flags);

		void bindAniamtionForMesh(AnimationSet *anim, Mesh *mesh);

	private:

		void readAnimation(const aiScene *scene, const aiAnimation *anim);

		void readMesh(const aiScene *scene, const aiMesh *meshNode);

		Joint *findJointFromRoot(aiString name, Joint *root);

		Joint *findJointFromVector(aiString name, vector<Joint*> &joints);

		void *copyMatrix(aiMatrix4x4 *from, Matrix4 *to);
	};
}

#endif