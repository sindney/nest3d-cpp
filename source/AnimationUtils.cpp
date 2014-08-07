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
}