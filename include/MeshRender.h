#ifndef N3D_MESHRENDER_H
#define N3D_MESHRENDER_H

#include <string>

namespace nest
{
	class Joint;

	class Matrix4;

	class Mesh;

	class MeshNode;

	class MeshRender
	{
	public:

		/**
		 *	Use this to select shader in mesh's shader map.
		 */
		std::string flag;

		int numDraws, numTris, numVts;

		MeshRender() : numDraws(0), numTris(0), numVts(0) {}

		virtual ~MeshRender() 
		{
			invertViewMatrix = NULL;
			projectionMatrix = NULL;
			worldMatrix = NULL;
		}

		/**
		 *	@param node MeshNode to draw.
		 *	@param invertViewMatrix Camera's invert view matrix.
		 *	@param projectionMatrix Camera's projection matrix.
		 */
		virtual void draw(MeshNode *node, Matrix4 *invertViewMatrix, Matrix4 *projectionMatrix);

	protected:

		Matrix4 *invertViewMatrix, *projectionMatrix, *worldMatrix;

		/**
		 *	Draw mesh with skinInfo.
		 *	@see SkinInfo
		 */
		virtual void draw(Joint *joint);

		/**
		 *	Draw static mesh.
		 */
		virtual void draw(Mesh *mesh, Matrix4 *combinedMatrix = NULL);
	};
}

#endif