#ifndef N3D_ANIMATIONUTILS_H
#define N3D_ANIMATIONUTILS_H

#include <map>

#include "AnimationClip.h"

namespace nest
{
	class AnimationRig;

	class Mesh;

	class AnimationUtils
	{
	public:

		/**
		 *	Setup pose vector to store animation pose.
		 */
		static void setupPoseData(std::vector<PoseData> &poses, AnimationClip *clip);

		/**
		 *	Setup rig vector to store animation info.
		 */
		static void setupRigData(AnimationRig *animRig, AnimationClip *clip);

		/**
		 *	Bind rigData to mesh's joint tree.
		 */
		static void bindRigForMesh(AnimationRig *animRig, Mesh *mesh);

		/**
		 *	p0 + p1; q0 * q1;
		 */
		static void addBlender(
			std::vector<PoseData> &result, std::vector<PoseData> &p0, std::vector<PoseData> &p1, 
			bool position = true, bool rotation = true, bool scaling = true
		);

		/**
		 *	p0 - p1; q0 * -q1;
		 */
		static void subtractBlender(
			std::vector<PoseData> &result, std::vector<PoseData> &p0, std::vector<PoseData> &p1, 
			bool position = true, bool rotation = true, bool scaling = true
		);

		/**
		 *	p0 * s; slerp(0001, q0, s);
		 */
		static void scaleBlender(
			std::vector<PoseData> &result, std::vector<PoseData> &p0, float scale, 
			bool position = true, bool rotation = true, bool scaling = true
		);

		/**
		 *	p0 + (p1 - p0) * t; slerp(q0, q1, t);
		 */
		static void crossDissolveBlender(
			std::vector<PoseData> &result, std::vector<PoseData> &p0, std::vector<PoseData> &p1, float dt, 
			bool position = true, bool rotation = true, bool scaling = true
		);

		/**
		 *	Say result poses store the full body pose data.
		 *	<p>p0 stores part of it, you call combineBlender to update those parts in full body poses.</p>
		 *	
		 *	@param table The map to look up during blending process.
		 *	
		 *	@see createPoseTable
		 */
		static void combineBlender(
			std::vector<PoseData> &result, std::vector<PoseData> &p0, std::map<std::string, int> &table, 
			bool position = true, bool rotation = true, bool scaling = true
		);

		/**
		 *	Init a map to store pose name, index data.
		 *	<p>So we can use combineBlender.</p>
		 */
		static void createPoseTable(std::vector<PoseData> &source, std::map<std::string, int> &table);
	};
}

#endif