#ifndef N3D_OBJECTCULLING_H
#define N3D_OBJECTCULLING_H

#include <vector>

#include "Matrix4.h"

namespace nest
{
	class CameraNode;

	class ContainerNode;

	class MeshNode;

	class OcTree;

	class ObjectCulling
	{
	public:

		/**
		 *	Test the visibility of container's meshes and store result in vector.
		 *	
		 *	@param result0 Stores those meshes with alphaSort off and passed specific culling function.
		 *	@param result1 Stores those meshes with alphaSort on and passed specific culling function.
		 *	@param result2 Stores those meshes who didn't pass the culling function.
		 */
		static void classify(
			ContainerNode *root, CameraNode *camera, 
			std::vector<MeshNode*> *result0, std::vector<MeshNode*> *result1, std::vector<MeshNode*> *result2
		);

		/**
		 *	Test the visibility of octree's meshes and store result in vector.
		 *	
		 *	@param result0 Stores those meshes with alphaSort off and passed specific culling function.
		 *	@param result1 Stores those meshes with alphaSort on and passed specific culling function.
		 *	@param result2 Stores those meshes who didn't pass the culling function.
		 */
		static void classify(
			OcTree *tree, CameraNode *camera, 
			std::vector<MeshNode*> *result0, std::vector<MeshNode*> *result1, std::vector<MeshNode*> *result2
		);

		Vector4 planes[6];

		/**
		 *	When your projection matrix is perspective, use this.
		 */
		void create(float fov, float ratio, float near, float far);

		/**
		 *	When your projection matrix is orthographic, use this.
		 */
		void create(float left, float right, float bottom, float top, float near, float far);

		/**
		 *	Test if taget point is in frustum.
		 *	
		 *	@param p Point in view space.
		 */
		bool classifyPoint(const Vector4 &p);

		/**
		 *	Test if taget bounding sphere is in frustum.
		 *	
		 *	@param center Bounding sphere's center in view space.
		 *	@param radius Bounding sphere's radius in view space.
		 */
		bool classifyBSphere(const Vector4 &center, float radius);

		/**
		 *	Test if taget axis aligned bounding box is in frustum.
		 *	
		 *	@param bound Axis aligned bounding box in view space.
		 */
		bool classifyAABB(const AABB &bound);

	private:

		void createPlane(Vector4 &plane, const Vector4 &v1, const Vector4 &v2, const Vector4 &v3);

	};
}

#endif