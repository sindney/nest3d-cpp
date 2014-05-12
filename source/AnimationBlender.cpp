#include <iterator>

#include "AnimationBlender.h"

namespace nest
{
	using namespace std;

	void AnimationBlender::addBlender(vector<PoseData> &result, vector<PoseData> &p0, vector<PoseData> &p1)
	{
		int i;
		int size = result.size();

		PoseData *d0, *d1, *d2;

		for(i = 0; i < size; i++)
		{
			d0 = &result[i];
			d1 = &p0[i];
			d2 = &p1[i];
			d0->position = d1->position + d2->position;
			d0->rotation = d1->rotation * d2->rotation;
			d0->scaling = d1->scaling + d2->scaling;
		}
	}

	void AnimationBlender::subtractBlender(vector<PoseData> &result, vector<PoseData> &p0, vector<PoseData> &p1)
	{
		int i;
		int size = result.size();

		PoseData *d0, *d1, *d2;

		for(i = 0; i < size; i++)
		{
			d0 = &result[i];
			d1 = &p0[i];
			d2 = &p1[i];
			d0->position = d1->position - d2->position;
			d0->rotation = d1->rotation * Quaternion::conjugate(d2->rotation);
			d0->scaling = d1->scaling - d2->scaling;
		}
	}

	void AnimationBlender::scaleBlender(vector<PoseData> &result, vector<PoseData> &p0, float scale)
	{
		int i;
		int size = result.size();

		PoseData *d0, *d1;

		for(i = 0; i < size; i++)
		{
			d0 = &result[i];
			d1 = &p0[i];
			d0->position = d1->position * scale;
			d0->rotation = Quaternion::slerp(Quaternion(), d1->rotation, scale);
			d0->scaling = d1->scaling * scale;
		}
	}

	void AnimationBlender::crossDissolveBlender(vector<PoseData> &result, vector<PoseData> &p0, vector<PoseData> &p1, float dt)
	{
		int i;
		int size = result.size();

		PoseData *d0, *d1, *d2;

		for(i = 0; i < size; i++)
		{
			d0 = &result[i];
			d1 = &p0[i];
			d2 = &p1[i];
			d0->position = d1->position + (d2->position - d1->position) * dt;
			d0->rotation = Quaternion::slerp(d1->rotation, d2->rotation, dt);
			d0->scaling = d1->scaling + (d2->scaling - d1->scaling) * dt;
		}
	}

	void AnimationBlender::combineBlender(vector<PoseData> &result, vector<PoseData> &p0, map<string, int> &table)
	{
		vector<PoseData>::iterator i;
		map<string, int>::iterator j;
		PoseData *d0, *d1;
		for(i = p0.begin(); i != p0.end(); i++)
		{
			d0 = static_cast<PoseData*>(&*i);
			j = table.find(d0->name);
			if(j != table.end())
			{
				d1 = &result[j->second];
				d1->position = d0->position;
				d1->rotation = d0->rotation;
				d1->scaling = d0->scaling;
			}
		}
	}

	void AnimationBlender::createPoseTable(vector<PoseData> &source, map<string, int> &table)
	{
		table.clear();
		int i, j = source.size();
		PoseData *d0 = NULL;
		for(i = 0; i < j; i++)
		{
			d0 = &source[i];
			table.insert(map<string, int>::value_type(d0->name, i));
		}
	}

}