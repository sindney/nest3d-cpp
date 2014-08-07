#include <iterator>

#include "AnimationRig.h"
#include "Mesh.h"
#include "Joint.h"

namespace nest
{
	using namespace std;

	void AnimationRig::applyAnimation(vector<PoseData> &poses)
	{
		int i, j = rigs.size();

		RigData *rig = NULL;
		PoseData *pose = NULL;

		for(i = 0; i < j; i++)
		{
			rig = &rigs[i];
			pose = &poses[i];
			*rig->oldP = *rig->newP;
			*rig->oldR = *rig->newR;
			*rig->oldS = *rig->newS;
			*rig->newP = pose->position;
			*rig->newR = pose->rotation;
			*rig->newS = pose->scaling;
		}
	}

	void AnimationRig::displayAnimation(float dt)
	{
		vector<RigData>::iterator i;
		RigData *rig = NULL;

		Matrix4 rtMatrix, sMatrix;
		Vector4 v0, v1;
		Quaternion q0, q1;

		for(i = rigs.begin(); i < rigs.end(); ++i)
		{
			rig = static_cast<RigData*>(&*i);
			// store rotation and position info in rtMatrix.
			rtMatrix.identity();
			rtMatrix.rotate(Quaternion::slerp(*rig->oldR, *rig->newR, dt));
			rtMatrix.translate(*rig->oldP + (*rig->newP - *rig->oldP) * dt);
			// store scaling info in sMatrix.
			sMatrix.identity();
			sMatrix.scale(*rig->oldS + (*rig->newS - *rig->oldS) * dt);
			*rig->target = rtMatrix * sMatrix;
		}
	}
}