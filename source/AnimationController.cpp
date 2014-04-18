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

		vector<AnimationChannel*>::iterator j;
		AnimationChannel *channel = NULL;

		vector<QuatKeyFrame>::iterator k;
		QuatKeyFrame *quatFirst = NULL, *quatSecond = NULL;

		std::vector<Vec3KeyFrame>::iterator l;
		Vec3KeyFrame *vec3First = NULL, *vec3Second = NULL;

		// identity all sets' channels' target matrices
		vector<AnimationSet*>::iterator m;
		for(m = sets.begin(); m != sets.end(); m++)
		{
			set = static_cast<AnimationSet*>(*m);
			for(j = set->channels.begin(); j != set->channels.end(); j++)
			{
				channel = static_cast<AnimationChannel*>(*j);
				channel->target->identity();
			}
		}

		Matrix4 matrix;
		Quaternion q0, q1, q2;
		Vector4 v0, v1;

		float current = 0.0f, ratio = 0.0f, size = 0.0f, weight = 0.0f;

		for(i = tracks.begin(); i != tracks.end(); i++)
		{
			track = static_cast<AnimationTrack*>(*i);
			set = track->set;
			weight = track->weight;
			if(track->enabled && time >= track->position)
			{
				// time in track's local space.
				current = (time - track->position) * set->ticksPerSecond * track->speed;
				size = set->duration;
				// advance iterator if time has passed the end of a no-loop track.
				if(!track->set->loop && current > size) continue;
				// calculate the right time.
				while(current > size) current -= size;
				// traverse the animationset.
				for(j = set->channels.begin(); j != set->channels.end(); j++)
				{
					channel = static_cast<AnimationChannel*>(*j);
					// position keyframes
					if(channel->positionKeys.size() == 1)
					{
						vec3First = &channel->positionKeys[0];
						v0.x = vec3First->x; v0.y = vec3First->y; v0.z = vec3First->z;
						v0 *= weight;
						matrix.identity();
						matrix.translate(v0);
						*channel->target *= matrix;
					}
					else 
					{
						l = channel->positionKeys.begin();
						while(true)
						{
							if(l == channel->positionKeys.end()) break;
							vec3First = &*l++;
							vec3Second = &*l;
							if(vec3First-> t <= current && vec3Second->t >= current)
							{
								ratio = (current - vec3First->t) / (vec3Second->t - vec3First->t);
								v0.x = vec3First->x; v0.y = vec3First->y; v0.z = vec3First->z;
								v1.x = vec3Second->x; v1.y = vec3Second->y; v1.z = vec3Second->z;
								v0 *= weight;
								v1 *= weight;
								v0 = v0 + (v1 - v0) * ratio;
								matrix.identity();
								matrix.translate(v0);
								*channel->target *= matrix;
								break;
							}
						}
					}
					// rotation keyframes
					if(channel->rotationKeys.size() == 1)
					{
						quatFirst = &channel->rotationKeys[0];
						q0.x = quatFirst->x; q0.y = quatFirst->y; q0.z = quatFirst->z; q0.w = quatFirst->w;
						q0 = Quaternion::slerp(q2, q0, weight);
						matrix.identity();
						matrix.rotate(q0);
						*channel->target *= matrix;
					}
					else 
					{
						k = channel->rotationKeys.begin();
						while(true)
						{
							if(k == channel->rotationKeys.end()) break;
							quatFirst = &*k++;
							quatSecond = &*k;
							if(quatFirst->t <= current && quatSecond->t >= current)
							{
								ratio = (current - quatFirst->t) / (quatSecond->t - quatFirst->t);
								q0.x = quatFirst->x; q0.y = quatFirst->y; q0.z = quatFirst->z; q0.w = quatFirst->w;
								q1.x = quatSecond->x; q1.y = quatSecond->y; q1.z = quatSecond->z; q1.w = quatSecond->w;
								q0 = Quaternion::slerp(q2, q0, weight);
								q1 = Quaternion::slerp(q2, q1, weight);
								matrix.identity();
								matrix.rotate(Quaternion::slerp(q0, q1, ratio));
								*channel->target *= matrix;
								break;
							}
						}
					}
					// scaling keyframes
					if(channel->scalingKeys.size() == 1)
					{
						vec3First = &channel->scalingKeys[0];
						v0.x = vec3First->x; v0.y = vec3First->y; v0.z = vec3First->z;
						v0 *= weight;
						matrix.identity();
						matrix.scale(v0);
						*channel->target *= matrix;
					}
					else 
					{
						l = channel->scalingKeys.begin();
						while(true)
						{
							if(l == channel->scalingKeys.end()) break;
							vec3First = &*l++;
							vec3Second = &*l;
							if(vec3First-> t <= current && vec3Second->t >= current)
							{
								ratio = (current - vec3First->t) / (vec3Second->t - vec3First->t);
								v0.x = vec3First->x; v0.y = vec3First->y; v0.z = vec3First->z;
								v1.x = vec3Second->x; v1.y = vec3Second->y; v1.z = vec3Second->z;
								v0 = v0 * weight;
								v1 = v1 * weight;
								v0 = v0 + (v1 - v0) * ratio;
								matrix.identity();
								matrix.scale(v0);
								*channel->target *= matrix;
								break;
							}
						}
					}
				}
			}
		}
	}
}