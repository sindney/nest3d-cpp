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
		 *	Flags for ik calculation.
		 */
		bool yaw, pitch, roll;

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
	 *	<p>Call createIKChain() to fill ikJoints vector.</p>
	 *	<p>Call initializeIKChain() to init IKJoints' matrix by joints'.
	 *	<p>Call calculateIKChain(target) to reach target position using joint chain.</p>
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
		 *	First, we fill this vector with target joint chain.
		 */
		std::vector<Joint*> joints;

		/**
		 *	Then, we call IKSolver.createIKChain() to create IKJoint vector.
		 *	<p>We can also adjust IKJoints' rotation limitions.</p>
		 *	<p>After those we call IKSolver.calculateIKChain() to reach target position with our joint chain.</p>
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
		 *	Initialize IKJoint chain's matrices by joints vector.
		 */
		void initializeIKChain();

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