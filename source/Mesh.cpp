#include "Mesh.h"
#include "OcNode.h"
#include "OcTree.h"

namespace nest
{
	Mesh::Mesh(MeshRender *render)
	{
		this->alphaTest = false;
		this->alphaKey = 0.0;
		this->cliping = true;
		this->visible = true;
		this->faceCulling = true;
		this->face = GL_BACK;
		this->node = NULL;
		this->tree = NULL;
		this->render = render;
	}

	Mesh::~Mesh()
	{
		if(node != NULL && tree != NULL) tree->removeChild(this);
		node = NULL;
		tree = NULL;
		render = NULL;
	}
}