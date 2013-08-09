#include <iterator>
#include <vector>

#include "mesh.h"
#include "ocnode.h"

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
		if(node != NULL)
		{
			vector<mesh*>::iterator i;
			for(i = node->objects.begin(); i != node->objects.end(); i++)
			{
				if(*i == this)
				{
					node->objects.erase(i);
					break;
				}
			}
			if(node->objects.size() == 0)
			{
				if(node->parent)
				{
					node->parent->childs[node->id] = NULL;
					delete node;
				}
			}
			node = NULL;
		}
	}

	void mesh::recompose()
	{
		object3d::recompose();
		aabb::transform(worldMatrix, geom->bound, bound);
	}
}