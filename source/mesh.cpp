#include "mesh.h"
#include "ocnode.h"
#include "octree.h"

namespace nest
{
	mesh::mesh(geometry *geom, shader3d *shader) 
	{
		this->alphaTest = false;
		this->alphaKey = 0.0f;
		this->castShadows = false;
		this->cliping = true;
		this->visible = true;
		this->faceCulling = true;
		this->face = GL_BACK;
		this->geom = geom;
		this->shader = shader;
		this->node = NULL;
	}

	mesh::~mesh()
	{
		geom = NULL;
		shader = NULL;
		if(node != NULL) node->belonging->removeChild(this);
	}

	void mesh::recompose()
	{
		object3d::recompose();
		aabb::transform(worldMatrix, geom->bound, bound);
		if(node != NULL) node->belonging->transformChild(this);
	}
}