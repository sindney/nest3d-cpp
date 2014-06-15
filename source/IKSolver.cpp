#include <cmath>

#include "IKSolver.h"

namespace nest
{
	using namespace std;

	IKSolver::~IKSolver()
	{
		joints.clear();
		ikJoints.clear();
		result.clear();
	}

	void IKSolver::createIKChain()
	{
		int i, j = joints.size();
		float PI2 = 2 * 3.1415926f;
		ikJoints.resize(j);
		result.resize(j);

		IKJoint *joint0 = NULL;
		PoseData *pose = NULL;

		for(i = 0; i < j; i++)
		{
			joint0 = &ikJoints[i];
			pose = &result[i];
			pose->position.zero();
			pose->rotation.identity();
			pose->scaling.zero();
			joint0->rotation = &pose->rotation;
			joint0->yaw = joint0->pitch = joint0->roll = true;
			joint0->maxYaw = joint0->maxPitch = joint0->maxRoll = PI2;
			joint0->minYaw = joint0->minPitch = joint0->minRoll = 0.0f;
		}
	}

	void IKSolver::initializeIKChain()
	{
		int i, j = joints.size();

		IKJoint *joint0 = NULL;
		Joint *joint1 = NULL;

		for(i = 0; i < j; i++)
		{
			joint0 = &ikJoints[i];
			joint1 = joints[i];
			joint0->localMatrix = joint1->localMatrix;
			joint0->combinedMatrix = joint1->combinedMatrix;
		}
	}

	bool IKSolver::calculateIKChain(const Vector4 &target)
	{
		Matrix4 mat4;
		Quaternion quat;
		Vector4 curPos, curEnd, v0, v1;
		Vector4 axis, euler;

		int size = ikJoints.size();
		if(size < 2) return false;

		float PI2 = 2 * 3.1415926f;
		float angle;
		int tries = maxTries;

		int i = size - 2;
		IKJoint *curJoint = NULL, *endJoint = &ikJoints[size - 1];

		int j;
		IKJoint *j0 = NULL, *j1 = NULL;

		while(tries > 0)
		{
			curEnd.x = endJoint->combinedMatrix.raw[12];
			curEnd.y = endJoint->combinedMatrix.raw[13];
			curEnd.z = endJoint->combinedMatrix.raw[14];

			if(Vector4::distance(curEnd, target) > minDist)
			{
				curJoint = &ikJoints[i];
				curPos.x = curJoint->combinedMatrix.raw[12];
				curPos.y = curJoint->combinedMatrix.raw[13];
				curPos.z = curJoint->combinedMatrix.raw[14];

				v0 = curEnd - curPos;
				v1 = target - curPos;
				v0.normalize();
				v1.normalize();

				angle = v0 * v1;
				if(angle < 0.99999f)
				{
					angle = acos(angle);
					axis = Vector4::crossProduct(v0, v1);
					Matrix4::axisAngleToEuler(axis, angle, euler);
					// limit DOF values.
					// clamp them to 0 - 360 range first.
					// then clamp them to min - max range.
					if(curJoint->yaw)
					{
						while(euler.x > PI2)
							euler.x -= PI2;
						while(euler.x < 0.0f)
							euler.x += PI2;
						if(euler.x > curJoint->maxYaw)
							euler.x = curJoint->maxYaw;
						else if(euler.x < curJoint->minYaw)
							euler.x = curJoint->minYaw;
					}
					else 
						euler.x = 0.0f;
					if(curJoint->pitch)
					{
						while(euler.y > PI2)
							euler.y -= PI2;
						while(euler.y < 0.0f)
							euler.y += PI2;
						if(euler.y > curJoint->maxPitch)
							euler.y = curJoint->maxPitch;
						else if(euler.y < curJoint->minPitch)
							euler.y = curJoint->minPitch;
					}
					else 
						euler.y = 0.0f;
					if(curJoint->roll)
					{
						while(euler.z > PI2)
							euler.z -= PI2;
						while(euler.z < 0.0f)
							euler.z += PI2;
						if(euler.z > curJoint->maxPitch)
							euler.z = curJoint->maxPitch;
						else if(euler.z < curJoint->minPitch)
							euler.z = curJoint->minPitch;
					}
					else 
						euler.z = 0.0f;
					// store rotation info to matrix.
					mat4.identity();
					mat4.rotate(euler);
					quat.rotate(euler);
					*curJoint->rotation = (tries == maxTries ? quat : quat * *curJoint->rotation);
					curJoint->localMatrix *= mat4;
					curJoint->combinedMatrix *= mat4;
					// update joint chain
					j = i;
					j0 = &ikJoints[j++];
					while(j < size)
					{
						j1 = &ikJoints[j];
						j1->combinedMatrix = j0->combinedMatrix * j1->localMatrix;
						j0 = j1;
						j++;
					}
				}
				if(--i < 0) i = size - 2;
				tries--;
			}
			else 
				// target reached, we then finish the process.
				return true;
		}
		// target out of reach.
		return false;
	}

	void IKSolver::applyResult()
	{
		int i, j = joints.size();

		IKJoint *joint0 = NULL;
		Joint *joint1 = NULL;

		for(i = 0; i < j; i++)
		{
			joint0 = &ikJoints[i];
			joint1 = joints[i];
			joint1->localMatrix = joint0->localMatrix;
		}
		joint1 = joints[0];
		Joint::updateJoints(
			joint1, 
			joint1->parent != NULL ? &joint1->parent->combinedMatrix : NULL
		);
	}
}
