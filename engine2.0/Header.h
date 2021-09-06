#pragma once

class Position
{
public:
    float x, y;
    Position(float x, float y);
};

class Transform
{
public:
    Position position;
    float rotation;
    int width, height;

    Transform(int x, int y, int width, int height);

    void setPosition(float x, float y);

    void translate(int x, int y);
};

class Colider
{
public:
    int type;
    Colider(int type);

    bool detectColition(Object *obj, Object *obj2);
};

class Object
{
public:
    int color;
    bool staticObject;
    Transform transform;
    Colider colider;

    Object();
    ~Object();
    Object(int x, int y, int width, int height, int color);

    Object(Transform tra, int color);

    bool checkColitionWith(Object *obj);

    void draw();

    void draw(Window window);
};
