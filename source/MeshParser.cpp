#include <set>
#include <stack>

#include "MeshParser.h"
#include "AnimationSet.h"
#include "Mesh.h"

namespace nest
{
	using namespace std;

	bool MeshParser::parse(const string &file, unsigned int flags)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(file, flags);
		error.clear();
		
		if(!scene)
		{
			error = importer.GetErrorString();
			return false;
		}

		int i, j;
		if(scene->HasMeshes())
		{
			j = scene->mNumMeshes;
			for(i = 0; i < j; i++)
				readMesh(scene, scene->mMeshes[i]);
		}
		if(scene->HasAnimations())
		{
			j = scene->mNumAnimations;
			for(i = 0; i < j; i++)
				readAnimation(scene, scene->mAnimations[i]);
		}

		return true;
	}

	void MeshParser::bindAniamtionForMesh(AnimationSet *anim, Mesh *mesh)
	{
		int i, j = anim->channels.size();
		AnimationChannel *channel = NULL;
		Joint *joint = NULL;
		for(i = 0; i < j; i++)
		{
			channel = anim->channels[i];
			joint = findJointFromRoot(aiString(channel->name), mesh->skin->root);
			if(joint != NULL) 
				channel->target = &joint->localMatrix;
		}
	}

	void MeshParser::readAnimation(const aiScene *scene, const aiAnimation *anim)
	{
		AnimationSet *animSet = new AnimationSet(
			anim->mName.C_Str(), 
			anim->mTicksPerSecond == 0 ? 25 : anim->mTicksPerSecond, 
			anim->mDuration, 
			false
		);
		animations.push_back(animSet);

		AnimationChannel *channel = NULL;
		QuatKeyFrame quat;
		Vec3KeyFrame vec;

		int i, j;
		aiNode *node = NULL;
		aiNodeAnim *nodeAnim = NULL;
		aiQuatKey *quatKey = NULL;
		aiVectorKey *vecKey = NULL;

		for(i = 0; i < anim->mNumChannels; i++)
		{
			nodeAnim = anim->mChannels[i];
			node = scene->mRootNode->FindNode(nodeAnim->mNodeName);
			if(node != NULL)
			{
				channel = new AnimationChannel();
				channel->name = nodeAnim->mNodeName.C_Str();
				for(j = 0; j < nodeAnim->mNumRotationKeys; j++)
				{
					quatKey = &nodeAnim->mRotationKeys[j];
					quat.t = quatKey->mTime;
					quat.x = quatKey->mValue.x;
					quat.y = quatKey->mValue.y;
					quat.z = quatKey->mValue.z;
					quat.w = quatKey->mValue.w;
					channel->rotationKeys.push_back(quat);
				}
				for(j = 0; j < nodeAnim->mNumPositionKeys; j++)
				{
					vecKey = &nodeAnim->mPositionKeys[j];
					vec.t = vecKey->mTime;
					vec.x = vecKey->mValue.x;
					vec.y = vecKey->mValue.y;
					vec.z = vecKey->mValue.z;
					channel->positionKeys.push_back(vec);
				}
				for(j = 0; j < nodeAnim->mNumScalingKeys; j++)
				{
					vecKey = &nodeAnim->mScalingKeys[j];
					vec.t = vecKey->mTime;
					vec.x = vecKey->mValue.x;
					vec.y = vecKey->mValue.y;
					vec.z = vecKey->mValue.z;
					channel->scalingKeys.push_back(vec);
				}
				animSet->channels.push_back(channel);
			}
		}
	}

	void MeshParser::readMesh(const aiScene *scene, const aiMesh *aimesh)
	{
		Mesh *mesh = new Mesh(
			new Geometry(), 
			aimesh->mNumBones > 0 ? new SkinInfo() : NULL
		);
		mesh->name = aimesh->mName.C_Str();
		meshes.push_back(mesh);

		int i, j, k, l;

		j = aimesh->mNumVertices;
		aiVector3D *aivector = NULL;

		mesh->geometry->vertexData = new GLfloat[j * 3];
		mesh->geometry->numVts = j;

		for(i = 0; i < j; i++)
		{
			aivector = &aimesh->mVertices[i];
			mesh->geometry->vertexData[i * 3] = aivector->x;
			mesh->geometry->vertexData[i * 3 + 1] = aivector->y;
			mesh->geometry->vertexData[i * 3 + 2] = aivector->z;
		}

		j = aimesh->mNumFaces;
		aiFace *face = NULL;

		mesh->geometry->indexData = new GLuint[j * 3];
		mesh->geometry->numTris = j;

		for(i = 0; i< j; i++)
		{
			face = &aimesh->mFaces[i];
			for(k = 0; k < face->mNumIndices; k++)
				mesh->geometry->indexData[i * 3 + k] = face->mIndices[k];
		}

		AABB::configure(mesh->geometry->bound, mesh->geometry->vertexData, mesh->geometry->numVts * 3);

		if(aimesh->mTextureCoords != NULL)
		{
			j = aimesh->mNumVertices;
			mesh->geometry->uvData = new GLfloat[j * 2];
			for(i = 0; i < j; i++)
			{
				aivector = &aimesh->mTextureCoords[0][i];
				mesh->geometry->uvData[i * 2] = aivector->x;
				mesh->geometry->uvData[i * 2 + 1] = 1 - aivector->y;
			}
		}

		if(aimesh->mNormals != NULL)
		{
			j = aimesh->mNumVertices;
			mesh->geometry->normalData = new GLfloat[j * 3];
			for(i = 0; i < j; i++)
			{
				aivector = &aimesh->mNormals[i];
				mesh->geometry->normalData[i * 3] = aivector->x;
				mesh->geometry->normalData[i * 3 + 1] = aivector->y;
				mesh->geometry->normalData[i * 3 + 2] = aivector->z;
			}
		}

		if(aimesh->mTangents != NULL)
		{
			j = aimesh->mNumVertices;
			mesh->geometry->tangentData = new GLfloat[j * 3];
			for(i = 0; i < j; i++)
			{
				aivector = &aimesh->mTangents[i];
				mesh->geometry->tangentData[i * 3] = aivector->x;
				mesh->geometry->tangentData[i * 3 + 1] = aivector->y;
				mesh->geometry->tangentData[i * 3 + 2] = aivector->z;
			}
		}
		// animation
		if(mesh->skin != NULL)
		{
			aiBone *bone = NULL;
			aiVertexWeight *weight = NULL;

			mesh->geometry->jointPerVertex = jointPerVertex;

			vector<bool> flags;
			j = aimesh->mNumVertices * jointPerVertex;
			mesh->geometry->indicesData = new GLuint[j];
			for(i = 0; i < j; i++)
			{
				flags.push_back(false);
				mesh->geometry->indicesData[i] = 0;
			}
			j = aimesh->mNumVertices * (jointPerVertex - 1);
			mesh->geometry->weightsData = new GLfloat[j];
			for(i = 0; i < j; i++)
				mesh->geometry->weightsData[i] = 0.0f;

			int m, n;
			Joint *joint0 = NULL;
			set<string> joints;

			// read the joins's offset matrices.
			j = aimesh->mNumBones;
			for(i = 0; i < j; i++)
			{
				bone = aimesh->mBones[i];
				joints.insert(bone->mName.C_Str());

				joint0 = new Joint(bone->mName.C_Str(), NULL);
				copyMatrix(&bone->mOffsetMatrix, &joint0->offsetMatrix);
				mesh->skin->joints.push_back(joint0);

				for(k = 0; k < bone->mNumWeights; k++)
				{
					weight = &bone->mWeights[k];
					for(l = 0; l < jointPerVertex; l++)
					{
						m = weight->mVertexId * jointPerVertex + l;
						n = weight->mVertexId * (jointPerVertex - 1) + l;
						if(!flags[m])
						{
							mesh->geometry->indicesData[m] = i;
							if(l < jointPerVertex - 1) 
								mesh->geometry->weightsData[n] = weight->mWeight;
							flags[m] = true;
							break;
						}
					}
				}
			}
			flags.clear();

			// build the skeleton tree for result mesh.
			set<string>::iterator it = joints.begin();

			aiNode *meshNode = scene->mRootNode->FindNode(aimesh->mName);

			// find the skeleton root.
			// aka meshNode or it's parent.
			aiNode *root = scene->mRootNode->FindNode(aiString(*it));
			if(root->mParent != NULL)
			{
				while(!(root == meshNode || root == meshNode->mParent))
				{
					root = root->mParent;
					joints.insert(root->mName.C_Str());
				}
			}

			Joint *joint1 = NULL, *joint2 = NULL;

			// find mesh's skeleton's root node.
			joint0 = findJointFromVector(root->mName, mesh->skin->joints);
			if(joint0 == NULL) 
			{
				joint0 = new Joint(root->mName.C_Str(), NULL);
				copyMatrix(&root->mTransformation, &joint0->localMatrix);
			}
			joint0->mesh = mesh;
			mesh->skin->root = joint0;

			stack<aiNode*> nodes;
			nodes.push(root);
			stack<Joint*> jointStack;
			jointStack.push(joint0);

			aiNode *node0 = NULL, *node1 = NULL;

			bool firstChild, flag = false;

			while(nodes.size() > 0)
			{
				node0 = nodes.top();
				nodes.pop();
				joint0 = jointStack.top();
				jointStack.pop();
				firstChild = false;
				for(i = 0; i < node0->mNumChildren; i++)
				{
					node1 = node0->mChildren[i];
					// we check if node0's first level childs is necessary for the skeleton.
					if(!flag)
					{
						it = joints.find(node1->mName.C_Str());
						// skeleton doesn't contain this node.
						// which means it's not necessary, then we ignore this node and it's childs.
						if(it == joints.end())
							continue;
					}
					joint1 = findJointFromVector(node1->mName, mesh->skin->joints);
					if(joint1 == NULL) joint1 = new Joint(node1->mName.C_Str(), NULL);
					copyMatrix(&node1->mTransformation, &joint1->localMatrix);
					if(!firstChild)
					{
						joint0->firstChild = joint2 = joint1;
						firstChild = true;
					}
					else 
					{
						joint2->sibling = joint1;
						joint2 = joint1;
					}
					if(node1->mNumChildren > 0)
					{
						nodes.push(node1);
						jointStack.push(joint1);
					}
				}
				flag = true;
			}
			// update geometry's bound.
			Matrix4 invertLocalMatrix = mesh->skin->root->localMatrix.clone();
			invertLocalMatrix.inverse();
			mesh->geometry->bound = invertLocalMatrix * mesh->geometry->bound;
		}
	}

	Joint *MeshParser::findJointFromRoot(aiString name, Joint *root)
	{
		if(aiString(root->name) == name)
			return root;

		Joint *joint0 = root, *joint1 = NULL;
		stack<Joint*> joints;
		joints.push(joint0);

		while(joints.size() > 0)
		{
			joint0 = joints.top();
			joints.pop();
			joint1 = joint0->firstChild;
			while(joint1 != NULL)
			{
				if(aiString(joint1->name) == name)
					return joint1;
				if(joint1->firstChild != NULL)
					joints.push(joint1);
				joint1 = joint1->sibling;
			}
		}

		return NULL;
	}

	Joint *MeshParser::findJointFromVector(aiString name, vector<Joint*> &joints)
	{
		int i, j = joints.size();
		Joint *result = NULL;

		for(i = 0; i < j; i++)
		{
			result = joints[i];
			if(aiString(result->name) == name)
				return result;
		}

		return NULL;
	}

	void *MeshParser::copyMatrix(aiMatrix4x4 *from, Matrix4 *to)
	{
		to->raw[0] = from->a1;	to->raw[4] = from->a2;	to->raw[8] = from->a3;	to->raw[12] = from->a4;
		to->raw[1] = from->b1;	to->raw[5] = from->b2;	to->raw[9] = from->b3;	to->raw[13] = from->b4;
		to->raw[2] = from->c1;	to->raw[6] = from->c2;	to->raw[10] = from->c3;	to->raw[14] = from->c4;
		to->raw[3] = from->d1;	to->raw[7] = from->d2;	to->raw[11] = from->d3;	to->raw[15] = from->d4;
	}

}