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

	} KeyFrame;

	class AnimationSet
	{
	public:

		std::string name;

		/**
		 *	Target Matrix that this AnimationSet controls.
		 */
		Matrix4 *target;

		float ticksPerSecond;
		
		bool loop;

		std::vector<KeyFrame*> keyFrames;

		AnimationSet(std::string name, Matrix4 *target, float ticksPerSecond, bool loop)
		 : name(name), target(target), ticksPerSecond(ticksPerSecond), loop(loop) {}

		virtual ~AnimationSet()
		{
			KeyFrame *keyFrame = NULL;
			while(keyFrames.size() != 0)
			{
				keyFrame = keyFrames.back();
				keyFrames.pop_back();
				delete keyFrame;
			}
			target = NULL;
		}
	};
}

#endif