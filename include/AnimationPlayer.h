#ifndef N3D_ANIMATIONPLAYER_H
#define N3D_ANIMATIONPLAYER_H

#include "AnimationClip.h"

namespace nest
{
	class AnimationPlayer
	{
	public:

		float speed, time;

		AnimationClip *clip;

		std::vector<PoseData> poses;

		AnimationPlayer(AnimationClip *clip, float speed, float time)
		 : clip(clip), speed(speed), time(time) {}

		/**
		 *	Advance time to calculate new pose.
		 *	<p>You'd setupPoseData() first, then advanceTime.</p>
		 */
		void advanceTime(float dt);
	};
}

#endif