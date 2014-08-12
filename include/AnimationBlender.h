#ifndef N3D_ANIMATIONBLENDER_H
#define N3D_ANIMATIONBLENDER_H

#include <map>

#include "AnimationClip.h"
#include "AnimationRig.h"

namespace nest
{
	using namespace std;
	
	class AnimationBlender
	{
	public:

		/**
		 *	p0 + p1; q0 * q1;
		 */
		static void addBlender(
			vector<PoseData> &result, vector<PoseData> &p0, vector<PoseData> &p1, 
			bool position = true, bool rotation = true, bool scaling = true
		);

		/**
		 *	p0 - p1; q0 * -q1;
		 */
		static void subtractBlender(
			vector<PoseData> &result, vector<PoseData> &p0, vector<PoseData> &p1, 
			bool position = true, bool rotation = true, bool scaling = true
		);

		/**
		 *	p0 * s; slerp(0001, q0, s);
		 */
		static void scaleBlender(
			vector<PoseData> &result, vector<PoseData> &p0, float scale, 
			bool position = true, bool rotation = true, bool scaling = true
		);

		/**
		 *	p0 + (p1 - p0) * t; slerp(q0, q1, t);
		 */
		static void crossDissolveBlender(
			vector<PoseData> &result, vector<PoseData> &p0, vector<PoseData> &p1, float dt, 
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
			vector<PoseData> &result, vector<PoseData> &p0, map<string, int> &table, 
			bool position = true, bool rotation = true, bool scaling = true
		);

		/**
		 *	Init a map to store pose name, index data.
		 *	<p>So we can use combineBlender.</p>
		 */
		static void createPoseTable(vector<PoseData> &source, map<string, int> &table);
	};
}

#endif