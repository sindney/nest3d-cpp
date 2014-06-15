#include <stack>

#include "Joint.h"

namespace nest
{
	using namespace std;

	Joint::~Joint()
	{
		mesh = NULL;
		if(sibling != NULL) delete sibling;
		if(firstChild != NULL) delete firstChild;
	}

	void Joint::updateJoints(Joint *joint, Matrix4 *matrix)
	{
		joint->combinedMatrix = matrix != NULL ? (*matrix) * joint->localMatrix : joint->localMatrix;
		joint->finalMatrix = joint->combinedMatrix * joint->offsetMatrix;
		if(joint->sibling != NULL) 
			updateJoints(joint->sibling, matrix);
		if(joint->firstChild != NULL) 
			updateJoints(joint->firstChild, &joint->combinedMatrix);
	}

	Joint *Joint::findFromRoot(string name, Joint *root)
	{
		if(root->name.size() == name.size() && root->name.compare(0, name.size(), name) == 0)
			return root;

		Joint *joint0 = root, *joint1 = NULL;
		stack<Joint*> joints;
		joints.push(joint0);

		while(joints.size() > 0)
		{
			joint0 = joints.top();
			joints.pop();
			joint1 = joint0->firstChild;
			while(joint1 != NULL)
			{
				if(joint1->name.size() == name.size() && joint1->name.compare(0, name.size(), name) == 0)
					return joint1;
				if(joint1->firstChild != NULL)
					joints.push(joint1);
				joint1 = joint1->sibling;
			}
		}

		return NULL;
	}
}