#ifndef N3D_MESHRENDER_H
#define N3D_MESHRENDER_H

#include <string>

namespace nest
{
	class Joint;

	class Matrix4;

	class Mesh;

	class MeshRender
	{
	public:

		/**
		 *	Use this to select shader in mesh's shader map.
		 */
		std::string flag;

		int numDraws, numTris, numVts;

		MeshRender() : numDraws(0), numTris(0), numVts(0) {}
		
		/**
		 *	This sets numDraws/Tris/Vts to 0.
		 */
		void reset(std::string flag)
		{
			this->flag = flag;
			numVts = numTris = numDraws = 0;
		}
		
		void draw(Mesh *mesh, Matrix4 *worldMatrix, Matrix4 *invertViewMatrix, Matrix4 *projectionMatrix);

	private:

		Matrix4 *invertViewMatrix, *projectionMatrix, *worldMatrix;

		/**
		 *	@see SkinInfo
		 */
		void draw(Joint *joint);
		
		void draw(Mesh *mesh, Matrix4 *combinedMatrix = NULL);
	};
}

#endif