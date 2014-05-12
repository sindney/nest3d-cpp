#include <iterator>

#include "AnimationPlayer.h"

namespace nest
{
	using namespace std;

	void AnimationPlayer::advanceTime(float dt)
	{
		time += dt;

		int i, j = poses.size();
		PoseData *pose = NULL;
		AnimationChannel *channel = NULL;

		vector<QuatKeyFrame>::iterator k;
		QuatKeyFrame *quatFirst = NULL, *quatSecond = NULL;

		vector<Vec3KeyFrame>::iterator l;
		Vec3KeyFrame *vec3First = NULL, *vec3Second = NULL;

		Quaternion q0, q1;
		Vector4 v0, v1;

		float current = 0.0f, ratio = 0.0f, size = 0.0f;

		current = time * clip->ticksPerSecond * speed;
		size = clip->duration;
		if(clip->loop || current <= size) 
		{
			// calculate the right time.
			while(current > size) current -= size;
			// traverse the clip.
			for(i = 0; i < j; i++)
			{
				pose = &poses[i];
				channel = clip->channels[i];
				// position keyframes
				if(channel->positionKeys.size() == 1)
				{
					vec3First = &channel->positionKeys[channel->positionKeys.size() - 1];
					pose->position.x = vec3First->x;
					pose->position.y = vec3First->y;
					pose->position.z = vec3First->z;
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
							v0 = v0 + (v1 - v0) * ratio;
							pose->position = v0;
							break;
						}
					}
				}
				// rotation keyframes
				if(channel->rotationKeys.size() == 1)
				{
					quatFirst = &channel->rotationKeys[channel->rotationKeys.size() - 1];
					pose->rotation.x = quatFirst->x;
					pose->rotation.y = quatFirst->y;
					pose->rotation.z = quatFirst->z;
					pose->rotation.w = quatFirst->w;
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
							pose->rotation = Quaternion::slerp(q0, q1, ratio);
							break;
						}
					}
				}
				// scaling keyframes
				if(channel->scalingKeys.size() == 1)
				{
					vec3First = &channel->scalingKeys[channel->scalingKeys.size() - 1];
					pose->scaling.x = vec3First->x;
					pose->scaling.y = vec3First->y;
					pose->scaling.z = vec3First->z;
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
							v0 = v0 + (v1 - v0) * ratio;
							pose->scaling = v0;
							break;
						}
					}
				}
			}
		}
	}
}