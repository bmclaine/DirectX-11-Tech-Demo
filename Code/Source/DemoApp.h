#ifndef _DEMO_APP_H_
#define _DEMO_APP_H_

#include "Graphics.h"

class DemoApp
{
private:
	HINSTANCE instance;
	HWND window;

	Graphics* pGraphics;
	DRX* pDirectX;

public:
	DemoApp();
	DemoApp(const DemoApp&);
	~DemoApp();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool Update();
	void InitializeWindow();
	void ShutdownWindow();

};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
static DemoApp* pAppHandle = NULL;

#endif