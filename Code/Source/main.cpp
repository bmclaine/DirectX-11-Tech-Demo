#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include "DemoApp.h"

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(-1);

	DemoApp* drxDemoApp;
	drxDemoApp = new DemoApp;

	if (drxDemoApp == NULL) return 0;

	if(drxDemoApp->Initialize())
		drxDemoApp->Run();

	drxDemoApp->Shutdown();
	delete drxDemoApp;
	drxDemoApp = NULL;
	
	return 0;
}