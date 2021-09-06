Object *selected;

void KeyDown(int key)
{
    if (key == VK_ESCAPE)
    {
        PostQuitMessage(0);
    }
    // if arrow key

    if (key == VK_LEFT || key == 0x41)
    {
        selected->transform.translate(-10, 0);
    }
    else if (key == VK_RIGHT || key == 0x44)
    {
        selected->transform.translate(10, 0);
    }

    if (key == VK_UP || key == 0x57)
    {
        selected->transform.translate(0, 10);
    }
    else if (key == VK_DOWN || key == 0x53)
    {
        selected->transform.translate(0, -10);
    }
};

void setup()
{
    RGBA groundColor = RGBA(0, 0, 0);

    selected = engine.createObject(200, 200, 100, 100, RGBA(255, 255, 255, .5));

    engine.createObject(200, 400, 100, 100, RGBA(255, 0, 0));
    engine.createObject(600, 200, 100, 100, RGBA(0, 0, 255));

    // ground
    engine.createObject(0, 0, 1024, 100, groundColor)->staticObject = true;
    engine.createObject(0, 590, 1024, 100, groundColor)->staticObject = true;
    engine.createObject(-90, 0, 100, 640, groundColor)->staticObject = true;
    engine.createObject(1000, 0, 100, 640, groundColor)->staticObject = true;
    for (int i = 0; i < engine.objectCount; i++)
    {
        if (!engine.objects[i].staticObject)
        {
        }
    }
};

void update()
{

    // on mouse release
    if (GetAsyncKeyState(VK_LBUTTON) == 0)
    {
        selected = NULL;
    }

    for (int i = 0; i < engine.objectCount; i++)
    {

        // change selected object
        if (selected == NULL && GetAsyncKeyState(VK_LBUTTON))
        {
            if (engine.objects[i].transform.containsPoint(MouseX, MouseY))
            {
                selected = &engine.objects[i];
            }
        }

        // simulate gravity
        if (!engine.objects[i].staticObject)
            engine.objects[i].transform.move(Vector2DUtils::Down(), 1);
    }

    if (selected != NULL)
        if (Position(MouseX, MouseY).distance(selected->transform.position) > 10.0)
        {
            // draw a line between mouse and selected object
            engine.drawLine(Position(MouseX, MouseY), selected->transform.position.offset(selected->transform.width / 2, selected->transform.height / 2), selected->color);

            selected->transform.MoveTo(Position(MouseX, MouseY), 5);
        }
};