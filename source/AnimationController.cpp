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

		vector<AnimationSet*>::iterator k;
		for(k = sets.begin(); k != sets.end(); k++)
		{
			set = static_cast<AnimationSet*>(*k);
			set->target->identity();
		}

		Matrix4 matrix;
		Quaternion q0, q1, q2;

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
						ratio = (current - first->t) / (second->t - first->t);
						q0.x = first->x; q0.y = first->y; q0.z = first->z; q0.w = first->w;
						q1.x = second->x; q1.y = second->y; q1.z = second->z; q1.w = second->w;
						q0 = Quaternion::slerp(q2, q0, track->weight);
						q1 = Quaternion::slerp(q2, q1, track->weight);
						matrix.rotate(Quaternion::slerp(q0, q1, ratio));
						*set->target *= matrix;
						break;
					}
				}
			}
		}
	}
}