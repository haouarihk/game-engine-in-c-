#include <windows.h>

POINT Mouse;
int MouseX, MouseY;

void KeyDown(int key);

#include "Engine.cpp"
Engine engine = Engine();

#include "Game.cpp"

// winmain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    engine._update = &update;
    engine._setup = &setup;
    // gray color
    engine.backgroundColor = 0x808080;
    engine.setup(hInstance, 1024, 640, L"Hello World");
}
