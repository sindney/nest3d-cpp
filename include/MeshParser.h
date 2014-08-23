#ifndef N3D_MESHPARSER_H
#define N3D_MESHPARSER_H

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace nest
{
	class AnimationClip;

	class AnimationRig;

	class Joint;

	class Mesh;

	class Matrix4;

	/**
	 *	How to use AnimationClip to update Mesh's Joints' matrices: 
	 *	<p>First you'd use AnimationRig and bind it to Mesh's Joint tree to control the Mesh's Joints.</p>
	 *	<p>Then you can use AnimationPlayer to play AniamtionClips, the result is represented as a PoseData vector.</p>
	 *	<p>Finally you can use AnimationRig.applyAnimation(poses) to update joints's translation flags.</p>
	 *	<p>Call AnimationRig.displayAnimation(dt) to interpolate between translation flags then update Joint's matrices.</p>
	 *	<p>Call Joint::updateJoints(rootJoint) to update the joint tree's relation matrices.</p>
	 *	<p>Tip: you can also blend poses together to generate the final AnimationRig.</p>
	 *	
	 *	@see AnimationBlender
	 *	@see AnimationClip
	 *	@see AnimationPlayer
	 *	@see AnimationRig
	 *	@see AnimationUtils
	 */
	class MeshParser
	{
	public:

		std::vector<AnimationClip*> animClips;

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

		/**
		 *	Please note that this clears meshs&animClips vector.
		 *	<p>Make sure you'v made a copy or deleted those pointers.</p>
		 */
		~MeshParser()
		{
			animClips.clear();
			meshes.clear();
			error.clear();
		}

		/**
		 *	Call this to delete data in animClips&meshes.
		 */
		void delocateData();

		/**
		 *	I suggest the following flags for model parsing.
		 *	<p>aiProcess_ValidateDataStructure, aiProcess_Triangulate</p>
		 *	<p>aiProcess_JoinIdenticalVertices, aiProcess_SortByPType</p>
		 *	<p>See Assimp::Importer::ReadFile(pFile, pFlags).</p>
		 *	<p>Please note that this clears meshs&animClips vector.</p>
		 *	<p>Make sure you'v made a copy or deleted those pointers.</p>
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