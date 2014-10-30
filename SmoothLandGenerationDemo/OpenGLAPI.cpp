#include "OpenGLAPI.h"
#include <gl\glew.h>
#include <gl\wglew.h>
#include "Singletons.h"
#include <glm\gtx\transform.hpp>
#include <glm\gtc\matrix_transform.hpp>

extern LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

using namespace std;

OpenGLAPI* OpenGLAPI::Instance()
{
	static OpenGLAPI instance;
	return &instance;
}

OpenGLAPI::OpenGLAPI(){}
OpenGLAPI::~OpenGLAPI(){}

/****************************
* 
****************************/
void OpenGLAPI::InitializeProgram(string progName)
{
	programName=progName;
}

/****************************
* RegisterProgramClass - Registers the dummy program
****************************/
void OpenGLAPI::RegisterProgramClass(HINSTANCE hInst)
{
	hInstance=hInst;
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;

	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	wcex.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hIconSm = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.hInstance = hInstance;

	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = programName.c_str();

	wcex.lpszMenuName = NULL;

	RegisterClassEx(&wcex);
}

/****************************
* RegisterMainProgramClass - Registers the main window
****************************/
void OpenGLAPI::RegisterMainProgramClass(HINSTANCE hInstance)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style =  CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0; wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_MENUBAR+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Smooth Land Gen Demo";

	RegisterClassEx(&wc);
}

/****************************
* CreateProgramWindow - Creates the program window
****************************/
bool OpenGLAPI::CreateProgramWindow(string title)
{
	hWnd=CreateWindowEx(0,programName.c_str(),title.c_str(),WS_OVERLAPPEDWINDOW| WS_MAXIMIZE | WS_CLIPCHILDREN,
		0,0,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,hInstance,NULL);

	if(!InitializeWindow(hInstance,3,3)) return false;

	ShowWindow(hWnd,SW_SHOW);
	
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return true;
}

/******************************
* InitializeWindow - Sets up the window for the program
******************************/
bool OpenGLAPI::InitializeWindow(HINSTANCE hInstance, int majorVersion, int minorVersion)
{	
	if(!InitializeGLEW(hInstance)) return false;

	hDC = GetDC(hWnd);

	int bError = NONE;
	PIXELFORMATDESCRIPTOR pfd;

	if(majorVersion <= 2)
	{
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
			pfd.nSize= sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion   = 1;
		pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;
 
		int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
		if (iPixelFormat == 0)return false;

		if(!SetPixelFormat(hDC, iPixelFormat, &pfd))return false;

		// Create the old style context (OpenGL 2.1 and before)
		hRC = wglCreateContext(hDC);
		if(hRC)
			wglMakeCurrent(hDC, hRC);
		else 
			bError = CONTEXT_CREATION_FAILED;
	}
	else if(WGLEW_ARB_create_context && WGLEW_ARB_pixel_format)
	{
		const int iPixelFormatAttribList[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0
		};
		int iContextAttribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, majorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, minorVersion,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0
		};

		int iPixelFormat, iNumFormats;
		wglChoosePixelFormatARB(hDC, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats);

		if(!SetPixelFormat(hDC, iPixelFormat, &pfd))return false;

		hRC = wglCreateContextAttribsARB(hDC, 0, iContextAttribs);
		// If everything went OK
		if(hRC)
		{
			wglMakeCurrent(hDC, hRC);
			if(!SetupShaders())
				bError=SHADER_ERROR;
		}
		else bError = CONTEXT_CREATION_FAILED;
	}
	else bError = NOT_SUPPORTED;
   
	if(bError)
	{
		// Generate error messages
		char sErrorMessage[255], sErrorTitle[255];

		switch(bError)
		{
		case CONTEXT_CREATION_FAILED:
			sprintf_s(sErrorMessage, "Context Creation has failed! Program Terminating...");
			sprintf_s(sErrorTitle, "Context Creation Failure");
			break;
		case SHADER_ERROR:
			sprintf_s(sErrorMessage, "Error loading shader files! Please ensure shader files are in the directory and are error free!");
			sprintf_s(sErrorTitle, "Shader Loading Failed");
			break;
		case NOT_SUPPORTED:
			sprintf_s(sErrorMessage, "OpenGL %d.%d is not supported! Please download latest GPU drivers!", majorVersion, minorVersion);
			sprintf_s(sErrorTitle, "OpenGL %d.%d Not Supported", majorVersion, minorVersion);
			break;
		}
		MessageBox(hWnd, sErrorMessage, sErrorTitle, MB_ICONINFORMATION);
		return false;
   }

   return true; 
	
}

/****************************
* InitializeGLEW - Initializes GLEW 
****************************/
bool OpenGLAPI::InitializeGLEW(HINSTANCE hInstance)
{
	RegisterMainProgramClass(hInstance);

	//Creates fake window to obtain OpenGL contexts from GLEW
	HWND fakeWnd= CreateWindow(NULL,"FAKE",WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
      0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
      NULL, hInstance, NULL); 

	hDC = GetDC(fakeWnd);

	//setup fake pixel format
	
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
			pfd.nSize= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;
 
	int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (iPixelFormat == 0)return false;

	if(!SetPixelFormat(hDC, iPixelFormat, &pfd))return false; 

	//Create fake context
	HGLRC hRCFake = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRCFake);

	bool bResult = true;

	if(glewInit() != GLEW_OK)
	{
		bResult = false;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRCFake);
	DestroyWindow(fakeWnd);

	return bResult;	
}

/****************************
* Shutdown - Handles deconstruction of OpenGL items
****************************/
void OpenGLAPI::Shutdown()
{
	shaders.DeleteProgram();
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd,hDC);

	DestroyWindow(hWnd);
	UnregisterClass(programName.c_str(),hInstance);
}

/****************************
* SetupShaders - Loads, compiles, and links the shader files
****************************/
bool OpenGLAPI::SetupShaders()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Shader vertexShader;
	Shader fragmentShader;
	
	shaders.CreateProgram();

	vertexShader.LoadShader("Vertex.sh", GL_VERTEX_SHADER);
	fragmentShader.LoadShader("Fragment.sh", GL_FRAGMENT_SHADER);


	if(!shaders.AddShader(vertexShader)) return false;
	if(!shaders.AddShader(fragmentShader)) return false;

	if(!shaders.LinkProgram())	return false;

	vertexShader.DeleteShader();
	fragmentShader.DeleteShader();

	shaders.UseProgram();

	SetupOpenGL();
	return true;
}

/****************************
* Render - Renders the scene to the screen
****************************/
void OpenGLAPI::Render()
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	
	chunkManager->Render();

	SwapBuffers(hDC);
}

/****************************
* ResizesOpenGLViewport - Resizes the viewport
****************************/
void OpenGLAPI::ResizeOpenGLViewport()
{
	if(hWnd == NULL)return;
	RECT rRect; GetClientRect(hWnd, &rRect);
	glViewport(0, 0, rRect.right, rRect.bottom);
}

/****************************
* SetupOpenGL - Sets up the opengl uniforms and features
****************************/
void OpenGLAPI::SetupOpenGL()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glClearDepth(1.0);

	cameraID=glGetUniformLocation(shaders.GetProgramID(),"Camera");
	projectionID=glGetUniformLocation(shaders.GetProgramID(),"Projection");
	textureSamplerID=glGetUniformLocation(shaders.GetProgramID(),"textureSampler");
	normalMatrixID=glGetUniformLocation(shaders.GetProgramID(),"normalMatrix");
	modelMatrixID=glGetUniformLocation(shaders.GetProgramID(),"modelMatrix");
	
	GLuint sunlight=glGetUniformLocation(shaders.GetProgramID(),"sun.lightColor");
	glUniform3fv(sunlight,1,(GLfloat*)&glm::vec3(1.0,1.0,1.0));
	sunlight=glGetUniformLocation(shaders.GetProgramID(),"sun.lightDirection");
	glUniform3fv(sunlight,1,(GLfloat*)&(glm::normalize(glm::vec3(0.2,-1.0,0.2))));
	sunlight=glGetUniformLocation(shaders.GetProgramID(),"sun.lightIntensity");
	glUniform1f(sunlight,0.05f);
}
