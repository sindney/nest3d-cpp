#ifndef N3D_ANIMATIONCONTROLLER_H
#define N3D_ANIMATIONCONTROLLER_H

#include <vector>

namespace nest
{
	class AnimationSet;

	class AnimationTrack;

	/**
	 *	Doesn't support animation blending currentlly.
	 */
	class AnimationController
	{
	public:

		std::vector<AnimationTrack*> tracks;

		std::vector<AnimationSet*> sets;

		float time;

		AnimationController() : time(0) {}

		virtual ~AnimationController();

		virtual void advanceTime(float delta);
	};
}

#endif