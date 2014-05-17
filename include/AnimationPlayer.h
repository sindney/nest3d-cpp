#ifndef N3D_ANIMATIONPLAYER_H
#define N3D_ANIMATIONPLAYER_H

#include "AnimationClip.h"
#include "EventDispatcher.h"

namespace nest
{
	class AnimationEvent : public Event
	{
	public:

		/**
		 *	Time in player's space.
		 *	<p>For exp: time = player->time * clip->ticesPerSecond * player->speed</p>
		 */
		float time;

		AnimationEvent(float time) : time(time), Event("AnimationEvent") {}
	};

	class AnimationPlayer : public EventDispatcher
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
		 *	<p>Plus, this triggers AnimationEvent.</p>
		 */
		void advanceTime(float dt);
	};
}

#endif