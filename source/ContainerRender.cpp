#include <algorithm>
#include <iterator>
#include <typeinfo>
#include <vector>

#include "ContainerRender.h"

namespace nest
{
	using namespace std;

	void ContainerRender::calculate() 
	{
		passed0.clear();
		passed1.clear();
		rejected.clear();
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
						if(!mesh->cliping || mesh->visible && camera->culling->classifyAABB(camera->invertWorldMatrix * mesh->bound))
						{
							if(mesh->alphaTest)
							{
								mesh->alphaKey = mesh->worldMatrix.raw[12] * mesh->worldMatrix.raw[12] + 
													mesh->worldMatrix.raw[13] * mesh->worldMatrix.raw[13] + 
													mesh->worldMatrix.raw[14] * mesh->worldMatrix.raw[14];
								passed1.push_back(mesh);
							}
							else 
							{
								draw = mesh->draw ? mesh->draw : this->draw;
								draw->calculate(mesh, &camera->invertWorldMatrix, &camera->projectionMatrix);
								passed0.push_back(mesh);
								numMeshes++;
								numTris += mesh->numTris();
								numVts += mesh->numVts();
							}
						}
						else if(recordRejectedMeshes)
						{
							rejected.push_back(mesh);
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