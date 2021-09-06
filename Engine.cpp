#include <windows.h>
#include "Transform.cpp"
#include "Window.cpp"
#include "Objects.cpp"

class Engine
{
public:
    void (*_earlySetup)(){

    };
    void (*_setup)(){

    };
    void (*_earlyUpdate)(){

    };
    void (*_update)(){

    };

    Window window = Window();
    // list of objects
    Object objects[10] = {};
    int objectCount = 0;
    int width, height;
    int backgroundColor = 0xffffffff;
    bool running = true;

    Engine(){};
    ~Engine(){};
    void setup(HINSTANCE hInstance, int width, int height, const wchar_t *title)
    {
        window.setup(hInstance, width, height, title);
        this->setup();
        while (running)
        {
            this->update();
            window.update();

            if (!window.handleMessages())
            {
                running = false;
            };
        };
    };

    void setup()
    {
        //_earlySetup();
        _setup();
    }

    void update()
    {
        GetCursorPos(&Mouse);
        ScreenToClient(GetActiveWindow(), &Mouse);
        MouseX = Mouse.x;
        MouseY = Mouse.y = 600 - Mouse.y;

        window.drawBackground(backgroundColor);
        //_earlyUpdate();
        updateObjects();
        _update();

        //draw all objects in engine
        this->drawAll();
    };

    void updateObjects()
    {
        for (int i = 0; i < objectCount; i++)
        {
            objects[i].update(objects, objectCount, i);
        };
    };

    Object *createObject(float x, float y, float width, float height, RGBA color)
    {
        Object obj = Object(x, y, width, height, color);
        return &(objects[objectCount++] = obj);
    };

    void drawLine(Position a1, Position a2, RGBA color)
    {
        this->window.drawLine(a1.x, a1.y, a2.x, a2.y, color);
    }

    void drawAll()
    {
        for (int i = 0; i < _countof(objects); i++)
        {
            objects[i].draw(window);
        }
    }
};
