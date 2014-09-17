#ifndef N3D_MESHPARSER_H
#define N3D_MESHPARSER_H

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

namespace nest
{
	typedef struct
	{
		AnimationClip *animClip;

		/**
		 *	Delocation flag. True for auto delete.
		 */
		bool flag;

	} AnimationInfo;

	typedef struct
	{
		Mesh *mesh;

		/**
		 *	Delocation flag. True for auto delete.
		 */
		bool flag;

	} MeshInfo;

	/**
	 *	How to use AnimationClip to update Mesh's Joints' matrices: 
	 *	<p>First you'd use AnimationRig and bind it to Mesh's Joint tree to control the Mesh's Joints.</p>
	 *	<p>Then you can use AnimationPlayer to play AniamtionClips, the result is represented as a PoseData vector.</p>
	 *	<p>Finally you can use AnimationRig.applyAnimation(poses) to update joints's translation flags.</p>
	 *	<p>Call AnimationRig.displayAnimation(dt) to interpolate between translation flags then update Joint's matrices.</p>
	 *	<p>Call Joint::updateJoints(rootJoint) to update the joint tree's relation matrices.</p>
	 *	<p>Tip: you can also blend poses together to generate the final AnimationRig.</p>
	 *	
	 *	@see AnimationClip
	 *	@see AnimationPlayer
	 *	@see AnimationRig
	 *	@see AnimationUtils
	 */
	class MeshParser
	{
	public:

		std::vector<AnimationInfo> animClips;

		std::vector<MeshInfo> meshes;

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
			delocateData();
		}

		/**
		 *	Call this to delete data in animClips&meshes with flag == true.
		 */
		void delocateData();

		/**
		 *	Please note this first calls delocateData() to delete pointers in animClips&meshes.
		 *	<p>And clears error string.</p>
		 *	<p>I suggest the following flags for model parsing.</p>
		 *	<p>aiProcess_ValidateDataStructure, aiProcess_Triangulate</p>
		 *	<p>aiProcess_JoinIdenticalVertices, aiProcess_SortByPType</p>
		 *	<p>See Assimp::Importer::ReadFile(pFile, pFlags).</p>
		 *	
		 *	@param file Model file path.
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