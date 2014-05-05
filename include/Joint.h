#ifndef N3D_JOINT_H
#define N3D_JOINT_H

#include <string>

#include "Matrix4.h"

namespace nest
{
	class Mesh;
	
	class Joint
	{
	public:

		std::string name;

		Vector4 oldP, newP;

		Quaternion oldR, newR;

		Vector4 oldS, newS;

		/**
		 *	Local transform matrix. Store bone's transform info.
		 */
		Matrix4 localMatrix;

		/**
		 *	Combined transform matrix. Aka parent.combined * local.
		 */
		Matrix4 combinedMatrix;

		/**
		 *	Offset matrix. Transform vertices into bone space.
		 */
		Matrix4 offsetMatrix;

		/**
		 * Final matrix. Aka combinedMatrix * offsetMatix.
		 */
		Matrix4 finalMatrix;

		Mesh *mesh;

		Joint *firstChild, *sibling;

		Joint(std::string name, Mesh *mesh) : name(name), mesh(mesh), firstChild(NULL), sibling(NULL) {}

		~Joint();

		/**
		 *	Update joint tree's matrices.
		 */
		static void updateJoints(Joint *joint, Matrix4 *parent);
	};
}

#endif