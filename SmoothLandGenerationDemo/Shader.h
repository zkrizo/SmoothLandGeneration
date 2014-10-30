#ifndef Shader_H
#define Shader_H

#include <string>

class Shader
{
public:
	bool LoadShader(const std::string& file, const int& type);
	void DeleteShader();
	
	bool IsLoaded() const{return loaded;};
	unsigned int GetShaderID() const{return shaderID;};
	
	Shader();
private:
	unsigned int shaderID;
	int type;
	bool loaded;
};

#endif