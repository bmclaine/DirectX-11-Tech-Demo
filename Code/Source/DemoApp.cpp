#include "DemoApp.h"

DemoApp::DemoApp()
{
	pGraphics = NULL;
}

DemoApp::DemoApp(const DemoApp&)
{

}

DemoApp::~DemoApp()
{

}

bool DemoApp::Initialize()
{
	InitializeWindow();

	pGraphics = new Graphics;
	if (pGraphics == NULL) return false;

	if (!pGraphics->Initialize(window)) return false;

	return true;
}

void DemoApp::Shutdown()
{
	if (pGraphics)
	{
		pGraphics->Shutdown();
		delete pGraphics;
		pGraphics = NULL;
	}

	ShutdownWindow();
}

void DemoApp::Run()
{
	MSG msg;
	bool exit = false;

	ZeroMemory(&msg, sizeof(MSG));

	while (exit == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}



		if (msg.message == WM_QUIT) exit = true;
		else
		{
			if (!Update()) exit = true;
			//else
			//{
				//frame stuff here
			//}
		}
	}

	return;
}

bool DemoApp::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE)) return false;

	if (!pGraphics->Update()) return false;

	return true;
}

LRESULT CALLBACK DemoApp::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{

	// If we're handling messages later for whatever reason..

	//switch (umsg)
	//{
	//default:
	//	break;
	//}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void DemoApp::InitializeWindow()
{
	WNDCLASSEX wndClass;

	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = L"DirectX Demo App";
	wndClass.hInstance = instance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);

	RegisterClassEx(&wndClass);

	RECT window_size = { 0, 0, (int)BACKBUFFER_WIDTH, (int)BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(L"DirectX Demo App", L"GP2 DirectX11 Demo", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
		NULL, NULL, instance, this);

	ShowWindow(window, SW_SHOW);
}

void DemoApp::ShutdownWindow()
{
	DestroyWindow(window);
	window = NULL;

	UnregisterClass(L"DirectX Demp App", instance);
	instance = NULL;

	pAppHandle = NULL;

	return;
}

#include "DRX.h"
DRX* gD3d = NULL;
#include "Graphics.h"
Graphics* gGraphics = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_SIZE:
	{
		if (gD3d)
			gD3d->ResizeWindow();
		if (gGraphics)
			gGraphics->Resize();

		return pAppHandle->MessageHandler(hwnd, umsg, wparam, lparam);
	}
	break;
	default:
	{
		return pAppHandle->MessageHandler(hwnd, umsg, wparam, lparam);
	}
	}
}