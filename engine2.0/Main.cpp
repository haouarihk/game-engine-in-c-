#include <windows.h>
#include "window.cpp"

Window window = Window();

POINT Mouse;
int MouseX, MouseY;

#include "p5.cpp"
#include "Sketch.cpp"

// winmain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    window.init(hInstance, 640, 480, L"Hello World");
}
