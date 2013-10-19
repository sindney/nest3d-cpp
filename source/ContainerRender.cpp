#include <algorithm>
#include <iterator>
#include <typeinfo>

#include "ContainerRender.h"

namespace nest
{
	using namespace std;

	void ContainerRender::calculate(int id, vector<Mesh*> *passed0, vector<Mesh*> *passed1, vector<Mesh*> *rejected)
	{
		bool recordPassed0 = passed0 != NULL;
		bool recordPassed1 = passed1 != NULL;
		bool recordRejected = rejected != NULL;

		numMeshes = numTris = numVts = 0;

		vector<Container3d*> containers;
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
									draw->calculate(mesh, &camera->invertWorldMatrix, &camera->projectionMatrix, id);
									if(recordPassed0) passed0->push_back(mesh);
									numMeshes++;
									numTris += mesh->numTris();
									numVts += mesh->numVts();
								}
								else if(recordPassed1)
								{
									mesh->alphaKey = mesh->worldMatrix.raw[12] * mesh->worldMatrix.raw[12] + 
														mesh->worldMatrix.raw[13] * mesh->worldMatrix.raw[13] + 
														mesh->worldMatrix.raw[14] * mesh->worldMatrix.raw[14];
									passed1->push_back(mesh);
								}
							}
							else if(recordRejected)
							{
								rejected->push_back(mesh);
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