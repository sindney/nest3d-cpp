#include "Mesh.h"
#include "OcNode.h"
#include "OcTree.h"

namespace nest
{
	Mesh::Mesh(ORDraw *draw, Geometry *geometry, Shader *shader)
	{
		this->alphaTest = false;
		this->alphaKey = 0.0;
		this->cliping = true;
		this->visible = true;
		this->faceCulling = true;
		this->face = GL_BACK;
		this->node = NULL;
		this->draw = draw;
		this->geometry = geometry;
		this->shader = shader;
	}

	Mesh::~Mesh()
	{
		if(node != NULL) node->belonging->removeChild(this);
		node = NULL;
		draw = NULL;
		geometry = NULL;
		shader = NULL;
	}

	void Mesh::recompose()
	{
		Object3d::recompose();
		bound = worldMatrix * geometry->bound;
		if(node != NULL) node->belonging->transformChild(this);
	}
}