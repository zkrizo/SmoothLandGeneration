#include "ShaderProgram.h"
#include <gl/glew.h>

ShaderProgram::ShaderProgram():linked(false){};

/****************************
* CreateProgram - Generates a program ID for the shader
****************************/
void ShaderProgram::CreateProgram()
{
	programID=glCreateProgram();
}

/****************************
* DeleteProgram - Destroys the program
****************************/
void ShaderProgram::DeleteProgram()
{
	if(!linked) return;
	
	linked=false;
	glDeleteProgram(programID);
}

/****************************
* UseProgram - Tells OpenGL to use this shader program
****************************/
void ShaderProgram::UseProgram()
{
	if(linked)
		glUseProgram(programID);
}

/****************************
* AddShader - Adds the passed shader to the program
****************************/
bool ShaderProgram::AddShader(const Shader& newShader)
{
	//Check to see if shader is loaded already
	if(!newShader.IsLoaded()) return false;
	
	glAttachShader(programID, newShader.GetShaderID());
	
	return true;
}

/****************************
* LinkProgram - Links the program
****************************/
bool ShaderProgram::LinkProgram()
{
	glLinkProgram(programID);
	
	int linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	linked=linkStatus==GL_TRUE;
	
	return linked;
}