#ifndef N3D_ANIMATIONCLIP_H
#define N3D_ANIMATIONCLIP_H

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

		Vector4 position, scaling;

		Quaternion rotation;

	} PoseData;

	typedef struct
	{
		std::string name;

		std::vector<QuatKeyFrame> rotationKeys;

		std::vector<Vec3KeyFrame> positionKeys, scalingKeys;

	} AnimationChannel;

	class AnimationClip
	{
	public:

		std::string name;

		std::vector<AnimationChannel*> channels;

		float ticksPerSecond, duration;

		bool loop;

		AnimationClip(std::string name, float ticksPerSecond, float duration, bool loop)
		 : name(name), ticksPerSecond(ticksPerSecond), duration(duration), loop(loop) {}

		virtual ~AnimationClip()
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