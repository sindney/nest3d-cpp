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

		/**
		 *	Use this to setup child's new and old PRS value.
		 */
		virtual void advanceTime(float delta);

		/**
		 *	Use this to calculate child's interpolated localMatirx.
		 *	<p>Call Joint::updateJoints(joint, parent) to update the joint tree.</p>
		 */
		virtual void display(float dt);
	};
}

#endif