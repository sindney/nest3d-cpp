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
		float PI = 3.1415926f;
		ikJoints.resize(j);
		result.resize(j);

		IKJoint *ikJoint = NULL;
		Joint *joint = NULL;
		PoseData *pose = NULL;

		for(i = 0; i < j; i++)
		{
			ikJoint = &ikJoints[i];
			joint = joints[i];
			pose = &result[i];
			pose->name = joint->name;
			pose->position.zero();
			pose->rotation.identity();
			pose->scaling.zero();
			ikJoint->rotation = &pose->rotation;
			ikJoint->maxYaw = ikJoint->maxPitch = ikJoint->maxRoll = PI;
			ikJoint->minYaw = ikJoint->minPitch = ikJoint->minRoll = -PI;
		}
	}

	void IKSolver::initializeIKChain()
	{
		int i, j = ikJoints.size();

		IKJoint *ikJoint = NULL;

		for(i = 0; i < j; i++)
		{
			ikJoint = &ikJoints[i];
			ikJoint->localMatrix.identity();
			ikJoint->localMatrix.translate(ikJoint->initPosition);
			ikJoint->localMatrix.rotate(ikJoint->initRotation);
			*ikJoint->rotation = ikJoint->initRotation;
		}

		updateIKChain(0);
	}

	void IKSolver::updateIKChain(int index)
	{
		int size = ikJoints.size();
		if(index >= 0 && index < size)
		{
			IKJoint *ikJoint = NULL;
			Matrix4 *mat4 = NULL;
			if(index == 0)
			{
				Joint *joint = joints[0]->parent;
				if(joint != NULL)
					mat4 = &joint->combinedMatrix;
			}
			else 
			{
				ikJoint = &ikJoints[index - 1];
				mat4 = &ikJoint->combinedMatrix;
			}
			int i = index;
			while(i < size)
			{
				ikJoint = &ikJoints[i];
				if(mat4 == NULL)
					ikJoint->combinedMatrix = ikJoint->localMatrix;
				else 
					ikJoint->combinedMatrix = *mat4 * ikJoint->localMatrix;
				mat4 = &ikJoint->combinedMatrix;
				i++;
			}
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
		float radian;
		int tries = maxTries;

		int i = size - 2;
		IKJoint *curJoint = NULL, *endJoint = &ikJoints[size - 1];

		int j;
		IKJoint *j0 = NULL, *j1 = NULL;
		Joint *parentJoint = NULL;

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

				radian = v0 * v1;
				if(radian < 0.99999f)
				{
					axis = Vector4::crossProduct(v0, v1);
					radian = acos(radian);
					quat.rotate(axis, radian);
					*curJoint->rotation = quat * *curJoint->rotation;
					// Quaternion to euler
					Quaternion::quatToAxisRadian(*curJoint->rotation, axis, radian);
					Matrix4::axisRadianToEuler(axis, radian, euler);
					// limit rotation DOF.
					if(euler.x > curJoint->maxYaw)
						euler.x = curJoint->maxYaw;
					else if(euler.x < curJoint->minYaw)
						euler.x = curJoint->minYaw;
					if(euler.y > curJoint->maxPitch)
						euler.y = curJoint->maxPitch;
					else if(euler.y < curJoint->minPitch)
						euler.y = curJoint->minPitch;
					if(euler.z > curJoint->maxPitch)
						euler.z = curJoint->maxPitch;
					else if(euler.z < curJoint->minPitch)
						euler.z = curJoint->minPitch;
					// Euler to quaternion
					curJoint->rotation->rotate(euler);
					curJoint->localMatrix.identity();
					curJoint->localMatrix.translate(curJoint->initPosition);
					curJoint->localMatrix.rotate(euler);
					// update joint chain
					updateIKChain(i);
				}
				if(--i < 0) 
				{
					i = size - 2;
					tries--;
				}
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

		IKJoint *ikJoint = NULL;
		Joint *joint = NULL;

		for(i = 0; i < j; i++)
		{
			ikJoint = &ikJoints[i];
			joint = joints[i];
			joint->localMatrix = ikJoint->localMatrix;
		}
		joint = joints[0];
		Joint::updateJoints(
			joint, 
			joint->parent != NULL ? &joint->parent->combinedMatrix : NULL
		);
	}
}
