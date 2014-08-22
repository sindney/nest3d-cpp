#include "AnimationUtils.h"
#include "AnimationRig.h"
#include "Mesh.h"

namespace nest
{
	using namespace std;

	void AnimationUtils::setupPoseData(vector<PoseData> &poses, AnimationClip *clip)
	{
		int i, j = clip->channels.size();
		AnimationChannel *channel = NULL;
		PoseData *pose = NULL;

		// resize poses vector to the size of clip->channels vector.
		poses.resize(j);

		// init poses data.
		for(i = 0; i < j; i++)
		{
			pose = &poses[i];
			channel = clip->channels[i];
			pose->name = channel->name;
			pose->position.zero();
			pose->rotation.identity();
			pose->scaling.zero();
		}
	}

	void AnimationUtils::setupRigData(AnimationRig *animRig, AnimationClip *clip)
	{
		int i, j = clip->channels.size();
		AnimationChannel *channel = NULL;
		RigData *rig = NULL;

		// resize rigs vector to the size of channels vector.
		animRig->rigs.resize(j);

		// init rigs data.
		for(i = 0; i < j; i++)
		{
			rig = &animRig->rigs[i];
			channel = clip->channels[i];
			rig->name = channel->name;
			rig->target = NULL;
			rig->oldP = rig->newP = NULL;
			rig->oldR = rig->newR = NULL;
			rig->oldS = rig->newS = NULL;
		}
	}

	void AnimationUtils::bindRigForMesh(AnimationRig *animRig, Mesh *mesh)
	{
		int i, j = animRig->rigs.size();
		RigData *rig = NULL;
		Joint *joint = NULL;
		for(i = 0; i < j; i++)
		{
			rig = &animRig->rigs[i];
			joint = Joint::findFromRoot(rig->name, mesh->skin->root);
			if(joint != NULL) 
			{
				// bind joint's RTS value.
				rig->oldP = &joint->oldP;
				rig->oldR = &joint->oldR;
				rig->oldS = &joint->oldS;
				rig->newP = &joint->newP;
				rig->newR = &joint->newR;
				rig->newS = &joint->newS;
				rig->target = &joint->localMatrix;
			}
		}
	}

	void AnimationUtils::addBlender(
		vector<PoseData> &result, vector<PoseData> &p0, vector<PoseData> &p1, 
		bool position, bool rotation, bool scaling
	)
	{
		int i;
		int size = result.size();

		PoseData *d0, *d1, *d2;

		for(i = 0; i < size; i++)
		{
			d0 = &result[i];
			d1 = &p0[i];
			d2 = &p1[i];
			if(position) d0->position = d1->position + d2->position;
			if(rotation) d0->rotation = d1->rotation * d2->rotation;
			if(scaling) d0->scaling = d1->scaling + d2->scaling;
		}
	}

	void AnimationUtils::subtractBlender(
		vector<PoseData> &result, vector<PoseData> &p0, vector<PoseData> &p1, 
		bool position, bool rotation, bool scaling
	)
	{
		int i;
		int size = result.size();

		PoseData *d0, *d1, *d2;

		for(i = 0; i < size; i++)
		{
			d0 = &result[i];
			d1 = &p0[i];
			d2 = &p1[i];
			if(position) d0->position = d1->position - d2->position;
			if(rotation) d0->rotation = d1->rotation * Quaternion::conjugate(d2->rotation);
			if(scaling) d0->scaling = d1->scaling - d2->scaling;
		}
	}

	void AnimationUtils::scaleBlender(
		vector<PoseData> &result, vector<PoseData> &p0, float scale, 
		bool position, bool rotation, bool scaling
	)
	{
		int i;
		int size = result.size();

		PoseData *d0, *d1;

		for(i = 0; i < size; i++)
		{
			d0 = &result[i];
			d1 = &p0[i];
			if(position) d0->position = d1->position * scale;
			if(rotation) d0->rotation = Quaternion::slerp(Quaternion(), d1->rotation, scale);
			if(scaling) d0->scaling = d1->scaling * scale;
		}
	}

	void AnimationUtils::crossDissolveBlender(
		vector<PoseData> &result, vector<PoseData> &p0, vector<PoseData> &p1, float dt, 
		bool position, bool rotation, bool scaling
	)
	{
		int i;
		int size = result.size();

		PoseData *d0, *d1, *d2;

		for(i = 0; i < size; i++)
		{
			d0 = &result[i];
			d1 = &p0[i];
			d2 = &p1[i];
			if(position) d0->position = d1->position + (d2->position - d1->position) * dt;
			if(rotation) d0->rotation = Quaternion::slerp(d1->rotation, d2->rotation, dt);
			if(scaling) d0->scaling = d1->scaling + (d2->scaling - d1->scaling) * dt;
		}
	}

	void AnimationUtils::combineBlender(
		vector<PoseData> &result, vector<PoseData> &p0, map<string, int> &table, 
		bool position, bool rotation, bool scaling
	)
	{
		vector<PoseData>::iterator i;
		map<string, int>::iterator j;

		PoseData *d0, *d1;

		for(i = p0.begin(); i != p0.end(); ++i)
		{
			d0 = static_cast<PoseData*>(&*i);
			j = table.find(d0->name);
			if(j != table.end())
			{
				d1 = &result[j->second];
				if(position) d1->position = d0->position;
				if(rotation) d1->rotation = d0->rotation;
				if(scaling) d1->scaling = d0->scaling;
			}
		}
	}

	void AnimationUtils::createPoseTable(vector<PoseData> &source, map<string, int> &table)
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