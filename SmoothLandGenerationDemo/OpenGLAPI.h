#ifndef OpenGLAPI_H
#define OpenGLAPI_H

#include "ShaderProgram.h"
#include "Camera.h"
#include <string>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\GL.h>
#include <glm\glm.hpp>

class OpenGLAPI
{
public:
	static OpenGLAPI* Instance();
	HWND hWnd;

	HINSTANCE GetInstance() {return hInstance;};

	void InitializeProgram(std::string progName);
	void RegisterProgramClass(HINSTANCE hInstance);
	void RegisterMainProgramClass(HINSTANCE hInstance);

	bool CreateProgramWindow(std::string title);
	bool InitializeWindow(HINSTANCE hInstance, int majorVersion, int minorVersion);
	bool InitializeGLEW(HINSTANCE hInstance);
	bool SetupShaders();

	void Shutdown();
	void SetupOpenGL();
	void ResizeOpenGLViewport();

	void Render();

	GLuint projectionID;
	GLuint cameraID;
	GLuint modelMatrixID;
	GLuint textureSamplerID;
	GLuint normalMatrixID;
	CameraController camera;
private:
	OpenGLAPI();
	~OpenGLAPI();
	OpenGLAPI(const OpenGLAPI&);
	OpenGLAPI operator=(const OpenGLAPI&);

	HINSTANCE hInstance;
	HDC hDC;
	HGLRC hRC;

	std::string programName;
	
	bool programActive;
	
	ShaderProgram shaders;
};

enum OpenGLFailures
{
	NONE=0,
	CONTEXT_CREATION_FAILED,
	SHADER_ERROR,
	NOT_SUPPORTED
};

#endif