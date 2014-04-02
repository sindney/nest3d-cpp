#include "Joint.h"
#include "Mesh.h"

namespace nest
{
	Joint::~Joint()
	{
		mesh = NULL;
		if(sibling != NULL) delete sibling;
		if(firstChild != NULL) delete firstChild;
	}

	void Joint::updateJoints(Joint *joint, Matrix4 *parent)
	{
		joint->combinedMatrix = parent != NULL ? (*parent) * joint->localMatrix : joint->localMatrix;
		joint->finalMatrix = joint->combinedMatrix * joint->offsetMatrix;
		if(joint->sibling != NULL) 
			updateJoints(joint->sibling, parent);
		if(joint->firstChild != NULL) 
			updateJoints(joint->firstChild, &joint->combinedMatrix);
	}
}