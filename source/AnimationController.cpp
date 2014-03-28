#include <iterator>
#include <typeinfo>

#include "AnimationController.h"
#include "AnimationSet.h"
#include "AnimationTrack.h"

namespace nest
{
	AnimationController::~AnimationController()
	{
		AnimationTrack *track = NULL;
		while(tracks.size() != 0)
		{
			track = tracks.back();
			tracks.pop_back();
			delete track;
		}
		AnimationSet *set = NULL;
		while(sets.size() != 0)
		{
			set = sets.back();
			sets.pop_back();
			delete set;
		}
	}

	void AnimationController::advanceTime(float delta)
	{
		using namespace std;

		time += delta;

		vector<AnimationTrack*>::iterator i;
		AnimationTrack *track = NULL;
		AnimationSet *set = NULL;

		vector<KeyFrame*>::iterator j;
		KeyFrame *first = NULL, *second = NULL;

		float current = 0.0f, ratio = 0.0f, size = 0.0f;

		for(i = tracks.begin(); i != tracks.end(); i++)
		{
			track = static_cast<AnimationTrack*>(*i);
			set = track->set;
			if(track->enabled && time >= track->position)
			{
				// time in track's local space.
				current = (time - track->position) * set->ticksPerSecond * track->speed;
				size = set->keyFrames.back()->t;
				// advance iterator if time has passed the end of a no-loop track.
				if(!track->set->loop && current > size) continue;
				// calculate the right time.
				while(current > size) current -= size;
				// traverse the animationset's keyframes.
				j = set->keyFrames.begin();
				while(true)
				{
					if(j == set->keyFrames.end()) break;
					first = *j++;
					second = *j;
					if(first->t <= current && second->t >= current)
					{
						ratio = track->weight * (current - first->t) / (second->t - first->t);
						Quaternion q0(first->x, first->y, first->z, first->w);
						Quaternion q1(second->x, second->y, second->z, second->w);
						set->target->rotate(Quaternion::slerp(q0, q1, ratio));
						break;
					}
				}
			}
		}
	}
}