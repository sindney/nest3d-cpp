#include "Mesh.h"
#include "MeshNode.h"
#include "OcNode.h"
#include "OcTree.h"

namespace nest
{
	MeshNode::~MeshNode()
	{
		if(node != NULL && tree != NULL) tree->removeChild(this);
		node = NULL;
		tree = NULL;
		mesh = NULL;
	}

	void MeshNode::recompose()
	{
		// Update transform matriices.
		ObjectNode::recompose();
		// Translate Bounding box from mesh space to world space.
		bound = worldMatrix * mesh->geometry->bound;
		// Translate node in octree.
		if(node != NULL && tree != NULL) tree->transformChild(this);
	}
}