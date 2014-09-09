#include "Mesh.h"

namespace nest
{
	using namespace std;

	Mesh::~Mesh()
	{
		if(geometry != NULL && geomFlag) delete geometry;
		if(skin != NULL) delete skin;
		map<string, ShaderInfo>::iterator it;
		ShaderInfo sInfo;
		for(it = shaderMap.begin(); it != shaderMap.end(); ++it)
		{
			sInfo = it->second;
			if(sInfo.flag) delete sInfo.shader;
		}
	}

	bool Mesh::bindShader(string name, Shader *shader, bool flag)
	{
		ShaderInfo sInfo = {shader, flag};
		return bindShader(name, sInfo);
	}

	bool Mesh::bindShader(string name, ShaderInfo sInfo)
	{
		pair<map<string, ShaderInfo>::iterator, bool> it;
		it = shaderMap.insert(map<string, ShaderInfo>::value_type(name, sInfo));
		return it.second;
	}

	bool Mesh::unbindShader(string name, ShaderInfo *sInfo)
	{
		map<string, ShaderInfo>::iterator it;
		it = shaderMap.find(name);
		if(it != shaderMap.end())
		{
			if(sInfo != NULL) *sInfo = it->second;
			shaderMap.erase(it);
			return true;
		}
		return false;
	}
}