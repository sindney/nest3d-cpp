#include <algorithm>
#include <iterator>
#include <list>
#include <typeinfo>

#include "ContainerRender.h"

namespace nest
{
	using namespace std;

	void ContainerRender::calculate(vector<Mesh*> *result0, vector<Mesh*> *result1, vector<Mesh*> *result2)
	{
		bool mark0 = result0 != NULL;
		bool mark1 = result1 != NULL;
		bool mark2 = result2 != NULL;

		numMeshes = numTris = numVts = 0;

		list<Container3d*> containers;

		vector<Object3d*>::iterator i;
		Container3d *current = root;

		Mesh *mesh;
		ORDraw *draw;

		while(true)
		{
			if(current->visible)
			{
				for(i = current->objects.begin(); i != current->objects.end(); i++)
				{
					if(typeid(**i) == typeid(Container3d))
					{
						containers.push_back(static_cast<Container3d*>(*i));
					} 
					else 
					{
						mesh = static_cast<Mesh*>(*i);
						if(mesh->visible)
						{
							if(!mesh->cliping || camera->culling->classifyAABB(camera->invertWorldMatrix * mesh->bound))
							{
								if(!mesh->alphaTest)
								{
									draw = mesh->draw ? mesh->draw : this->draw;
									draw->calculate(mesh, &camera->invertWorldMatrix, &camera->projectionMatrix);
									if(mark0) result0->push_back(mesh);
									numMeshes++;
									numTris += mesh->geometry->numTris;
									numVts += mesh->geometry->numVts;
								}
								else if(mark1)
								{
									mesh->alphaKey = mesh->worldMatrix.raw[12] * mesh->worldMatrix.raw[12] + 
													mesh->worldMatrix.raw[13] * mesh->worldMatrix.raw[13] + 
													mesh->worldMatrix.raw[14] * mesh->worldMatrix.raw[14];
									result1->push_back(mesh);
								}
							}
							else if(mark2)
							{
								result2->push_back(mesh);
							}
						}
					}
				}
			}
			if(containers.size() != 0)
			{
				current = containers.back();
				containers.pop_back();
				continue;
			}
			break;
		}
	}
}