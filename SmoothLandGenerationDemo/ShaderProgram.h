#ifndef ShaderProgram_H
#define ShaderProgram_H

#include "Shader.h"

class ShaderProgram
{
public:
	void CreateProgram();
	void DeleteProgram();
	void UseProgram();
	
	bool AddShader(const Shader&);
	bool LinkProgram();
	
	unsigned int GetProgramID() const{return programID;};
	
	ShaderProgram();
private:
	unsigned int programID;
	bool linked;
};

#endif