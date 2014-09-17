#ifndef N3D_IKSOLVER_H
#define N3D_IKSOLVER_H

#include <vector>

#include "AnimationClip.h"
#include "Joint.h"

namespace nest
{
	typedef struct
	{
		Matrix4 localMatrix, combinedMatrix;

		Quaternion *rotation;

		/**
		 *	The rotation's zero point.
		 */
		Quaternion initRotation;

		/**
		 *	The position's zero point.
		 */
		Vector4 initPosition;

		/**
		 *	Control params for ik calculation.
		 */
		float maxYaw, maxPitch, maxRoll;

		/**
		 *	Control params for ik calculation.
		 */
		float minYaw, minPitch, minRoll;

	} IKJoint;

	/**
	 *	Tips: fill joints vector first.
	 *	<p>Call createIKChain() to fill IKJoint && PoseData vector.</p>
	 *	<p>Call initializeIKChain() to init IKJoints' matrix by it's initial transforms.</p>
	 *	<p>Call calculateIKChain() to reach target position using joint chain.</p>
	 *	<p>The PoseData vector stores rotation info for animation blending.</p>
	 *	<p>You can also call applyResult() to apply IK result to joint chain directly.</p>
	 */
	class IKSolver
	{
	public:

		/**
		 *	The flag distant you'd call contact. Default value is 1.0.
		 */
		float minDist;

		/**
		 *	Default value is 8.
		 */
		int maxTries;

		/**
		 *	We would fill this vector with target joint chain.
		 */
		std::vector<Joint*> joints;

		/**
		 *	We store IKJoint chain here.
		 *	<p>You'd adjust IKJoint's DOF limitions and initial transforms.</p>
		 */
		std::vector<IKJoint> ikJoints;

		/**
		 *	The pointer in IKJoint points to PoseData's rotation data.
		 *	<p>So, PoseData vector stores the rotation poses for animation blending.</p>
		 */
		std::vector<PoseData> result;

		IKSolver() : minDist(1.0f), maxTries(8) {}

		~IKSolver();
		
		/**
		 *	Create IKJoint chain by joints vector.
		 */
		void createIKChain();

		/**
		 *	Initialize IKJoint chain's matrices by their initial transforms.
		 */
		void initializeIKChain();

		/**
		 *	Update IKJoint chain's transform from index.
		 */
		void updateIKChain(int index);

		/**
		 *	Reach target position using our joint chain.
		 *	<p>The result is stored in PoseData vector.</p>
		 */
		bool calculateIKChain(const Vector4 &target);

		/**
		 *	Apply ik result directly to joint chain.
		 */
		void applyResult();
	};
}

#endif