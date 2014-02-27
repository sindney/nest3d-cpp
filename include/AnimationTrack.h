#ifndef N3D_ANIMATIONTRACK_H
#define N3D_ANIMATIONTRACK_H

#include "AnimationSet.h"

namespace nest
{	
	class AnimationTrack
	{
	public:

		bool enabled;

		float position;

		float weight, speed;

		AnimationSet *set;

		AnimationTrack(bool enabled, float position, float weight, float speed)
		 : enabled(enabled), position(position), weight(weight), speed(speed), set(NULL) {}

		virtual ~AnimationTrack()
		{
			set = NULL;
		}
	};
}

#endif