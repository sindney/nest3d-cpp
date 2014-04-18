#ifndef N3D_ANIMATIONSET_H
#define N3D_ANIMATIONSET_H

#include <string>
#include <vector>

#include "Matrix4.h"

namespace nest
{
	typedef struct
	{
		float t, x, y, z, w;

	} QuatKeyFrame;

	typedef struct
	{
		float t, x, y, z;

	} Vec3KeyFrame;

	typedef struct
	{
		std::string name;

		/**
		 *	Target Matrix that this channel controls.
		 */
		Matrix4 *target;

		std::vector<QuatKeyFrame> rotationKeys;

		std::vector<Vec3KeyFrame> positionKeys, scalingKeys;

	} AnimationChannel;

	class AnimationSet
	{
	public:

		std::string name;

		float ticksPerSecond, duration;
		
		bool loop;

		std::vector<AnimationChannel*> channels;

		AnimationSet(std::string name, float ticksPerSecond, float duration, bool loop)
		 : name(name), ticksPerSecond(ticksPerSecond), duration(duration), loop(loop) {}

		virtual ~AnimationSet()
		{
			AnimationChannel *channel = NULL;
			while(channels.size() != 0)
			{
				channel = channels.back();
				channels.pop_back();
				delete channel;
			}
		}
	};
}

#endif