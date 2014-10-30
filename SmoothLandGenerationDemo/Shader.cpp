#include "Shader.h"
#include <vector>
#include <gl\glew.h>

Shader::Shader():loaded(false) {}

/****************************
* LoadShader - Loads and compiles the shader
****************************/
bool Shader::LoadShader(const std::string& filename, const int& settype)
{
	FILE* file;
	fopen_s(&file,filename.c_str(),"rt");
	
	if(!file) return false;
	
	std::vector<std::string> lines;
	char line[255];
	while(fgets(line,255,file))
		lines.push_back(line);
		
	fclose(file);
	
	const char** program =  new const char*[lines.size()];
	for(unsigned int i=0;i<lines.size();++i)
	{
		program[i]=lines[i].c_str();
	}
	
	shaderID=glCreateShader(settype);
	
	glShaderSource(shaderID,lines.size(),program,NULL);
	glCompileShader(shaderID);
	
	delete [] program;
	
	int compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	
	if(compileStatus==GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
 
		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);
 
		//We don't need the shader anymore.
		glDeleteShader(shaderID);
		
		return false;
	}
	
	type=settype;
	loaded=true;
	
	return true;
}

/****************************
* DeleteShader - Deletes the shader
****************************/
void Shader::DeleteShader()
{
	if(!IsLoaded()) return;
	
	loaded=false;
	glDeleteShader(shaderID);
}
