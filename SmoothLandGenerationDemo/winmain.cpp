#include <Windows.h>
#include "OpenGLAPI.h"
#include "Singletons.h"
#include "Timer.h"

#include <iostream>
#include <io.h>
#include <fcntl.h>

/***********************
********Winmain*********
***********************/
int WINAPI WinMain( HINSTANCE hInstance,
					HINSTANCE prevInstance,
					LPTSTR lpCmdLind,
					int nCmdShow)
{	
	ogl->InitializeProgram("Smooth Land Generation Demo");
	ogl->RegisterProgramClass(hInstance);

	//initialize the window
	if(!ogl->CreateProgramWindow("Smooth Land Generation Demo"))
		return false;
	
	//Creates the chunkManager instance to create the data
	chunkManager->Instance();
	Timer timer;
	double QPCFPS=1;


	MSG msg;
	ZeroMemory(&msg,sizeof(msg));

	//Main Loop
	while(msg.message!=WM_QUIT)
	{
		if(PeekMessage(&msg,NULL,0U,0U,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer.update();

			input->Update(1/QPCFPS);
			ogl->Render();

			timer.update();
			QPCFPS=timer.getFPS();

			if(timer.getFPS()>60)
				Sleep(20);
		}
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	bool result;
	POINT currentMousePos;

	switch(uiMsg)
	{
	case WM_PAINT:
		BeginPaint(hWnd, &ps);					
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
	case WM_QUIT:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		ogl->ResizeOpenGLViewport();
		break;
			
	case WM_RBUTTONDOWN:
		SetCapture(hWnd);
		input->KeyPress(true, KEY_MOUSERIGHT);
		break;

	case WM_RBUTTONUP:
		ReleaseCapture();
		input->KeyPress(false, KEY_MOUSERIGHT);
		break;

	case WM_KEYDOWN:
		int fwKeys;
		LPARAM keyData;
		fwKeys=(int)wParam;
		keyData=lParam;

		switch(fwKeys)
		{
		case 0x41:
			input->KeyPress(true, KEY_A);
			break;
		case 0x44:
			input->KeyPress(true, KEY_D);
			break;
		case 0x53:
			input->KeyPress(true, KEY_S);
			break;
		case 0x57:
			input->KeyPress(true, KEY_W);
			break;
		case VK_SPACE:
			input->KeyPress(true, KEY_SPACE);
			break;
		case VK_SHIFT:
			input->KeyPress(true, KEY_SHIFT);
			break;
		}
		break;
	
	case WM_KEYUP:
		fwKeys=(int)wParam;
		keyData=lParam;

		switch(fwKeys)
		{
		case 0x41:
			input->KeyPress(false, KEY_A);
			break;
		case 0x44:
			input->KeyPress(false, KEY_D);
			break;
		case 0x53:
			input->KeyPress(false, KEY_S);
			break;
		case 0x57:
			input->KeyPress(false, KEY_W);
			break;
		case VK_SPACE:
			input->KeyPress(false, KEY_SPACE);
			break;
		case VK_SHIFT:
			input->KeyPress(false, KEY_SHIFT);
			break;
		}
		break;
	case WM_MOUSEMOVE:
		result=GetCursorPos(&currentMousePos)!=0;

		if(result)
		{
			ScreenToClient(hWnd,&currentMousePos);
			input->MouseMove(currentMousePos.x,currentMousePos.y);
		}
		break;
	default:
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);

	}
	return 0;
}