#include <algorithm>
#include <iterator>
#include <typeinfo>

#include "CameraNode.h"
#include "ContainerNode.h"
#include "MeshNode.h"
#include "ObjectCulling.h"
#include "OcNode.h"
#include "OcTree.h"

namespace nest
{
	using namespace std;

	void ObjectCulling::classify(
		ContainerNode *root, CameraNode *camera, 
		vector<MeshNode*> *result0, vector<MeshNode*> *result1, vector<MeshNode*> *result2
	)
	{
		// meshes pased culling test with alphaSort off
		bool mark0 = result0 != NULL;
		// meshes pased culling test with alphaSort on
		bool mark1 = result1 != NULL;
		// meshes didn't pased culling test
		bool mark2 = result2 != NULL;

		vector<ContainerNode*> containers;

		vector<ObjectNode*>::iterator it0;
		ContainerNode *current = root;

		MeshNode *meshNode;
		Matrix4 mat0;

		while(true)
		{
			if(current->visible)
			{
				for(it0 = current->objects.begin(); it0 != current->objects.end(); ++it0)
				{
					if(typeid(**it0) == typeid(ContainerNode))
					{
						containers.push_back(static_cast<ContainerNode*>(*it0));
					} 
					else 
					{
						meshNode = static_cast<MeshNode*>(*it0);
						if(meshNode->visible)
						{
							if(!meshNode->cliping || camera->culling.classifyAABB(camera->invertWorldMatrix * meshNode->bound))
							{
								if(!meshNode->alphaSort && mark0) result0->push_back(meshNode);
								if(meshNode->alphaSort && mark1)
								{
									mat0 = camera->invertWorldMatrix * meshNode->worldMatrix;
									meshNode->alphaKey = mat0.raw[12] * mat0.raw[12] + mat0.raw[13] * mat0.raw[13] + mat0.raw[14] * mat0.raw[14];
									result1->push_back(meshNode);
								}
							}
							else if(mark2)
							{
								result2->push_back(meshNode);
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
		// sort meshes with alphaSort on
		if(mark1) sort(result1->begin(), result1->end(), MeshNode(NULL));
	}

	void ObjectCulling::classify(
		OcTree *tree, CameraNode *camera, 
		vector<MeshNode*> *result0, vector<MeshNode*> *result1, vector<MeshNode*> *result2
	)
	{
		// meshes pased culling test with alphaSort off
		bool mark0 = result0 != NULL;
		// meshes pased culling test with alphaSort on
		bool mark1 = result1 != NULL;
		// meshes didn't pased culling test
		bool mark2 = result2 != NULL;

		vector<MeshNode*>::iterator it0;
		MeshNode *meshNode;
		AABB bound;
		Matrix4 mat0;

		vector<OcNode*> nodes;
		vector<OcNode*>::iterator it1;
		OcNode *node0 = tree->root;
		OcNode *node1 = NULL;

		vector<bool> marks;
		bool current;
		bool parent = false;

		while(true)
		{
			bound = camera->invertWorldMatrix * node0->bound;
			if(parent || camera->culling.classifyAABB(bound))
			{
				current = parent ? true : camera->culling.classifyPoint(bound.min) && camera->culling.classifyPoint(bound.max);
				if(node0->objects.size() != 0)
				{
					for(it0 = node0->objects.begin(); it0 != node0->objects.end(); ++it0)
					{
						meshNode = *it0;
						if(meshNode->visible)
						{
							if(!meshNode->cliping || current || camera->culling.classifyAABB(camera->invertWorldMatrix * meshNode->bound))
							{
								if(!meshNode->alphaSort && mark0) result0->push_back(meshNode);
								if(meshNode->alphaSort && mark1)
								{
									mat0 = camera->invertWorldMatrix * meshNode->worldMatrix;
									meshNode->alphaKey = mat0.raw[12] * mat0.raw[12] + mat0.raw[13] * mat0.raw[13] + mat0.raw[14] * mat0.raw[14];
									result1->push_back(meshNode);
								}
							}
							else if(mark2)
							{
								result2->push_back(meshNode);
							}
						}
					}
				}
				for(it1 = node0->childs.begin(); it1 != node0->childs.end(); ++it1)
				{
					node1 = *it1;
					if(node1 != NULL && (current || camera->culling.classifyAABB(camera->invertWorldMatrix * node1->bound)))
					{
						nodes.push_back(node1);
						marks.push_back(current);
					}
				}
			}
			if(nodes.size() != 0)
			{
				node0 = nodes.back();
				nodes.pop_back();
				parent = marks.back();
				marks.pop_back();
				continue;
			}
			break;
		}
		// sort meshes with alphaSort on
		if(mark1) sort(result1->begin(), result1->end(), MeshNode(NULL));
	}

	void ObjectCulling::create(float fov, float ratio, float near, float far)
	{
		float r = tan(fov * 0.5);
		float nH = near * r;
		float nW = nH * ratio;
		float fH = far * r;
		float fW = fH * ratio;

		Vector4 zv(0, 0, 1, 1);
		Vector4 xv(1, 0, 0, 1);
		Vector4 yv(0, 1, 0, 1);

		Vector4 vn = - (zv * near);
		Vector4 vf = - (zv * far);

		Vector4 nTL = vn + (yv * nH) - (xv * nW);
		Vector4 nTR = vn + (yv * nH) + (xv * nW);
		Vector4 nBL = vn - (yv * nH) - (xv * nW);
		Vector4 nBR = vn - (yv * nH) + (xv * nW);

		Vector4 fTL = vf + (yv * fH) - (xv * fW);
		Vector4 fTR = vf + (yv * fH) + (xv * fW);
		Vector4 fBL = vf - (yv * fH) - (xv * fW);
		Vector4 fBR = vf - (yv * fH) + (xv * fW);

		// top
		createPlane(planes[0], fTR, nTR, nTL);
		// bottom
		createPlane(planes[1], nBL, nBR, fBR);
		// left
		createPlane(planes[2], nTL, nBL, fBL);
		// right
		createPlane(planes[3], fBR, nBR, nTR);
		// near
		createPlane(planes[4], nTL, nTR, nBR);
		// far
		createPlane(planes[5], fTR, fTL, fBL);
	}

	void ObjectCulling::create(float left, float right, float bottom, float top, float near, float far)
	{
		Vector4 nTL(left, 	top, 	-near, 1);
		Vector4 nTR(right, 	top, 	-near, 1);
		Vector4 nBL(left, 	bottom, -near, 1);
		Vector4 nBR(right, 	bottom, -near, 1);

		Vector4 fTL(left, 	top, 	-far, 1);
		Vector4 fTR(right, 	top, 	-far, 1);
		Vector4 fBL(left, 	bottom, -far, 1);
		Vector4 fBR(right, 	bottom, -far, 1);

		// top
		createPlane(planes[0], fTR, nTR, nTL);
		// bottom
		createPlane(planes[1], nBL, nBR, fBR);
		// left
		createPlane(planes[2], nTL, nBL, fBL);
		// right
		createPlane(planes[3], fBR, nBR, nTR);
		// near
		createPlane(planes[4], nTL, nTR, nBR);
		// far
		createPlane(planes[5], fTR, fTL, fBL);
	}

	bool ObjectCulling::classifyPoint(const Vector4 &p)
	{
		int i;
		Vector4 *plane;
		for(i = 0; i < 6; i++)
		{
			plane = &planes[i];
			if(plane->x * p.x + plane->y * p.y + plane->z * p.z + plane->w < 0) return false;
		}
		return true;
	}

	bool ObjectCulling::classifyBSphere(const Vector4 &center, float radius)
	{
		int i;
		float d;
		Vector4 *plane;
		for(i = 0; i < 6; i++)
		{
			plane = &planes[i];
			d = plane->x * center.x + plane->y * center.y + plane->z * center.z + plane->w;
			if(d < 0 && radius < -d) return false;
		}
		return true;
	}

	bool ObjectCulling::classifyAABB(const AABB &bound)
	{
		Vector4 center = (bound.max + bound.min) / 2;
		Vector4 half = bound.max - center;
		half.absolute();

		int i;
		Vector4 *plane;
		Vector4 normal;
		for(i = 0; i < 6; i++)
		{
			plane = &planes[i];
			normal = (*plane);
			normal.absolute();
			if(center * (*plane) + half * normal < -plane->w) return false;
		}

		return true;
	}

	void ObjectCulling::createPlane(Vector4 &plane, const Vector4 &v1, const Vector4 &v2, const Vector4 &v3)
	{
		plane = Vector4::crossProduct(v2 - v1, v3 - v1);
		plane.normalize();
		plane.w = - (plane.x * v1.x + plane.y * v1.y + plane.z * v1.z);
	}
}