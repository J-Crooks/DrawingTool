#include <windows.h>
#include "DrawingToolUI.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int show)
{
#ifdef _DEBUG
	//_CrtSetBreakAlloc();
	_onexit(_CrtDumpMemoryLeaks);
#endif

	DrawingToolUI UI;

	UI.create(hInstance, 1050, 800, 50, true);
	UI.onClose();

	return 0;
}