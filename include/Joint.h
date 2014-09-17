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

		Matrix4 localMatrix;

		/**
		 *	Parent.combined * local.
		 */
		Matrix4 combinedMatrix;

		/**
		 *	Transform vertices into bone space.
		 */
		Matrix4 offsetMatrix;

		/**
		 * CombinedMatrix * offsetMatix.
		 */
		Matrix4 finalMatrix;

		Mesh *mesh;

		Joint *firstChild, *sibling, *parent;

		Joint(std::string name, Mesh *mesh)
		 : name(name), mesh(mesh), firstChild(NULL), sibling(NULL), parent(NULL) {}

		~Joint();

		/**
		 *	Update joint tree's matrices.
		 */
		static void updateJoints(Joint *joint, Matrix4 *matrix);

		/**
		 *	Search target joint from root by it's name.
		 */
		static Joint *findFromRoot(std::string name, Joint *root);
	};
}

#endif