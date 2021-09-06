#include <windows.h>
#include <iostream>
#include "Utils.cpp"

void earlySetup(){};
void setup();
void earlyUpdate(){};
void draw();

void CreateCanvas(float width, float height);

// make WndProc function
LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
        // send message to the window
        SendMessage(GetDlgItem(hWnd, 1), uMsg, wParam, lParam);
        break;

    // case WM_KEYDOWN:
    //     KeyDown(wParam);
    //     break;
    // case WM_KEYLAST:
    //     KeyDown(wParam);
    //     break;
    case WM_DESTROY:
        PostQuitMessage(WM_QUIT);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

class Window
{
public:
    // callback functions

    HINSTANCE hInstance;
    HWND hWnd;
    HDC hDC;
    int width, height;

    bool running = true;

    BITMAPINFO bitmap_info;
    void *buffer_memory;

    int buffer_width, buffer_height;
    int buffer_size;

    Window(){};
    void init(HINSTANCE hInstance, int width, int height, const wchar_t *title)
    {
        this->hInstance = hInstance;
        this->width = width;
        this->height = height;

        // create window class
        WNDCLASS wc;
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = L"WindowClass";

        // register window class
        RegisterClass(&wc);

        // create window
        hWnd = CreateWindow(L"WindowClass", title, WS_OVERLAPPEDWINDOW, 0, 0, width, height, NULL, NULL, hInstance, NULL);

        hDC = GetDC(hWnd);

        ShowWindow(hWnd, SW_SHOW);
        updateSize();

        // setup
        earlySetup();
        setup();

        // update
        while (running)
        {
            update();
            if (!handleMessages())
            {
                running = false;
            };
        }
    };
    bool handleMessages()
    {
        MSG msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return false;
            if (msg.message == WM_SIZE)
                this->updateSize();

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return true;
    }

    void updateSize()
    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        buffer_width = rect.right - rect.left;
        buffer_height = rect.bottom - rect.top;
        buffer_size = buffer_width * buffer_height;

        if (buffer_memory)
            VirtualFree(buffer_memory, 0, MEM_RELEASE);
        buffer_memory = VirtualAlloc(NULL, buffer_size * 4, MEM_COMMIT, PAGE_READWRITE);

        // create bitmap
        bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
        bitmap_info.bmiHeader.biWidth = buffer_width;
        bitmap_info.bmiHeader.biHeight = -buffer_height;
        bitmap_info.bmiHeader.biPlanes = 1;
        bitmap_info.bmiHeader.biBitCount = 32;
    }

    void resizeWindow(float width, float height)
    {
        SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
    }

    void clear()
    {
        // clear window
    }
    void update()
    {

        earlyUpdate();

        // update window
        StretchDIBits(hDC, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer_memory, &bitmap_info, DIB_RGB_COLORS, SRCCOPY);

        draw();
    }

    void drawBackground(RGBA color)
    {
        unsigned int *pixels = (unsigned int *)buffer_memory;
        for (int i = 0; i < buffer_size; i++)
        {
            pixels[i] = color.toInt();
        }
    }

    void drawBackground(unsigned int color)
    {
        unsigned int *pixels = (unsigned int *)buffer_memory;
        for (int i = 0; i < buffer_size; i++)
        {
            pixels[i] = color;
        }
    }

    void drawRect(float x, float y, float w, float h, bool isStroke, bool isFill, float strokeWeight, RGBA strokeColor, RGBA fillColor)
    {
        unsigned int *pixels = (unsigned int *)buffer_memory;

        // draw the outline
        if (isStroke)
            for (int j = -strokeWeight / 2; j < strokeWeight / 2; j++)
            {
                for (int i = -strokeWeight / 2; i < w + strokeWeight / 2; i++)
                {
                    if (x + i < buffer_width && x + i >= 0 && y + j < buffer_height && y + j >= 0)
                        pixels[(int)(x + i) + (int)((y + j) * buffer_width)] = strokeColor.toInt();

                    if (x + i < buffer_width && x + i >= 0 && y + h + j < buffer_height && y + h + j >= 0)
                        pixels[(int)(x + i) + (int)((y + h + j) * buffer_width)] = strokeColor.toInt();
                }

                for (int i = 0; i < h; i++)
                {
                    if (x + j + i < buffer_width && x + j + i >= 0 && y + i < buffer_height && y + i >= 0)
                        pixels[(int)(x + j) + (int)((y + i) * buffer_width)] = strokeColor.toInt();

                    if (x + w + j < buffer_width && x + w + j >= 0 && y + i < buffer_height && y + i >= 0)
                        pixels[(int)(x + j + w) + (int)((y + i) * buffer_width)] = strokeColor.toInt();
                }
            }

        // fill
        if (isFill)
            for (int i = 0; i < w; i++)
            {
                for (int j = 0; j < h; j++)
                {
                    // if in the bound
                    if (x + i >= 0 && x + i < buffer_width && y + j >= 0 && y + j < buffer_height)
                    {
                        float diffx = x + 1 - (int)x;
                        float diffy = y + 1 - (int)y;
                        pixels[(((int)x) + i + (((int)y) + j) * buffer_width)] = fillColor.ChangeOpacity((diffx) * (diffy)*fillColor.a).toInt();
                    };
                };
            }
    }

    void drawLine(float x1, float y1, float x2, float y2, RGBA strokeColor)
    {
        unsigned int *pixels = (unsigned int *)buffer_memory;
        // draw line
        float x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
        dx = x2 - x1;
        dy = y2 - y1;
        dx1 = fabs(dx);
        dy1 = fabs(dy);
        px = 2 * dy1 - dx1;
        py = 2 * dx1 - dy1;
        if (dy1 <= dx1)
        {
            x = x2;
            y = y2;
            xe = x1;
            if (dx >= 0)
            {
                x = x1;
                y = y1;
                xe = x2;
            }

            for (i = 0; x < xe; i++)
            {
                x++;
                if (px < 0)
                    px += 2 * dy1;
                else
                {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                        y += 1;
                    else
                        y -= 1;
                    px += 2 * (dy1 - dx1);
                }
                if (x >= 0 && x < buffer_width && y >= 0 && y < buffer_height)
                    pixels[((int)x + (int)y * buffer_width)] = strokeColor.toInt();
            }
        }
        else
        {
            x = x2;
            y = y2;
            ye = y1;
            if (dy >= 0)
            {
                x = x1;
                y = y1;
                ye = y2;
            }

            for (i = 0; y < ye; i++)
            {
                y++;
                if (py <= 0)
                    py += 2 * dx1;
                else
                {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                        x++;
                    else
                        x--;
                    py += 2 * (dx1 - dy1);
                }
                if (x >= 0 && x < buffer_width && y >= 0 && y < buffer_height)
                    pixels[((int)x + (int)y * buffer_width)] = strokeColor.toInt();
            }
        }
    }

    void drawCircle(float x, float y, float r, RGBA rgba)
    {
        unsigned int *pixels = (unsigned int *)buffer_memory;
        int x0 = x - r;
        int y0 = y - r;
        int x1 = x + r;
        int y1 = y + r;
        int d = 1 - r;
        int xie = 0;
        int yie = r;
        int i = 0;
        while (x < y)
        {
            if (d < 0)
                d += 2 * xie + 3;
            else
            {
                d += 2 * (xie - yie) + 5;
                yie--;
            }
            xie++;
            if (x0 <= xie && xie <= x1 && y0 <= yie && yie <= y1)
                pixels[((int)xie + (int)yie * buffer_width)] = rgba.toInt();
            if (x0 <= xie && xie <= x1 && y0 <= yie + 1 && yie + 1 <= y1)
                pixels[((int)xie + (int)(yie + 1) * buffer_width)] = rgba.toInt();
            if (x0 <= xie && xie <= x1 && y0 <= yie - 1 && yie - 1 <= y1)
                pixels[((int)xie + (int)(yie - 1) * buffer_width)] = rgba.toInt();
            if (x0 <= xie + 1 && xie + 1 <= x1 && y0 <= yie && yie <= y1)
                pixels[((int)(xie + 1) + (int)yie * buffer_width)] = rgba.toInt();
            if (x0 <= xie + 1 && xie + 1 <= x1 && y0 <= yie + 1 && yie + 1 <= y1)
                pixels[((int)(xie + 1) + (int)(yie + 1) * buffer_width)] = rgba.toInt();
            if (x0 <= xie + 1 && xie + 1 <= x1 && y0 <= yie - 1 && yie - 1 <= y1)
                pixels[((int)(xie + 1) + (int)(yie - 1) * buffer_width)] = rgba.toInt();
            if (x0 <= xie - 1 && xie - 1 <= x1 && y0 <= yie && yie <= y1)
                pixels[((int)(xie - 1) + (int)yie * buffer_width)] = rgba.toInt();
            if (x0 <= xie - 1 && xie - 1 <= x1 && y0 <= yie + 1 && yie + 1 <= y1)
                pixels[((int)(xie - 1) + (int)(yie + 1) * buffer_width)] = rgba.toInt();
            if (x0 <= xie - 1 && xie - 1 <= x1 && y0 <= yie - 1 && yie - 1 <= y1)
                pixels[((int)(xie - 1) + (int)(yie - 1) * buffer_width)] = rgba.toInt();
        }
    }

    void drawCircle2(float x, float y, float r, RGBA rgba)
    {
    }
};
