#ifndef N3D_ANIMATIONRIG_H
#define N3D_ANIMATIONRIG_H

#include "AnimationClip.h"

namespace nest
{
	typedef struct 
	{
		std::string name;

		Matrix4 *target;

		Vector4 *oldP, *newP;

		Quaternion *oldR, *newR;

		Vector4 *oldS, *newS;

	} RigData;

	class AnimationRig
	{
	public:

		std::string name;

		std::vector<RigData> rigs;

		/**
		 *	Apply animation data to rig vector.
		 *	<p>You'd setupRigData() first, then applyAnimation().</p>
		 */
		void applyAnimation(std::vector<PoseData> &poses);

		/**
		 *	Blend old,new transforms and store them to target matrix for rendering.
		 *	<p>You'd applyAnimation() first, then play it.</p>
		 */
		void displayAnimation(float dt);
	};
}

#endif