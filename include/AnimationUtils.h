#ifndef N3D_ANIMATIONUTILS_H
#define N3D_ANIMATIONUTILS_H

#include "AnimationClip.h"

namespace nest
{
	class Mesh;

	class AnimationRig;

	class AnimationUtils
	{
	public:

		/**
		 *	Setup pose vector to store animation pose.
		 */
		static void setupPoseData(std::vector<PoseData> &poses, AnimationClip *clip);

		/**
		 *	Setup rig vector to store animation info.
		 */
		static void setupRigData(AnimationRig *animRig, AnimationClip *clip);

		/**
		 *	Bind rigData to mesh's joint tree.
		 */
		static void bindRigForMesh(AnimationRig *animRig, Mesh *mesh);
	};
}

#endif