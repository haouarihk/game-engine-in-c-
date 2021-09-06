#include <windows.h>

class Position
{
public:
    float x, y;
    Position(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    Position offset(float x, float y)
    {
        return Position(this->x + x, this->y + y);
    }

    float distance(Position other)
    {
        return sqrt(pow(this->x - other.x, 2) + pow(this->y - other.y, 2));
    }
};

class Transform
{
public:
    Position position = Position(0, 0);
    Vector2D velocity = Vector2D(0, 0);
    float rotation;
    int width, height;

    void update()
    {
        // friction
        velocity.x *= 0.7;
        velocity.y *= 0.7;

        position.x += velocity.x;
        position.y += velocity.y;
    };

    Transform(float x, float y, float width, float height)
    {
        this->position.x = x;
        this->position.y = y;
        this->width = width;
        this->height = (int)height;
    };

    void setPosition(float x, float y)
    {
        this->position.x = x;
        this->position.y = y;
    };

    void translate(int x, int y)
    {
        this->position.x += x;
        this->position.y += y;
    };

    bool containsPoint(int x, int y)
    {
        if (x >= this->position.x && x <= this->position.x + this->width && y >= this->position.y && y <= this->position.y + this->height)
        {
            return true;
        }
        return false;
    }

    bool contains(Transform other)
    {
        if (other.position.x >= this->position.x && other.position.x <= this->position.x + this->width && other.position.y >= this->position.y && other.position.y <= this->position.y + this->height)
        {
            return true;
        }
        return false;
    }

    float distanceFromSurfaceToSurface(Transform other)
    {
        // x axe
        float xsurface = this->position.x + this->height;
        float xothersurface = other.position.x + other.height;
        float xdistance = xsurface - xothersurface;

        // y axe
        float ysurface = this->position.y + this->width;
        float yothersurface = other.position.y + other.width;
        float ydistance = ysurface - yothersurface;

        return Vector2D(xdistance, ydistance).magnitude();
    }

    void move(Vector2D vector)
    {
        this->velocity += vector;
    }
    void move(Vector2D direction, float speed)
    {
        this->velocity.x += direction.normalize().x * speed;
        this->velocity.y += direction.normalize().y * speed;
    };

    void MoveTo(Position position, float speed = 1)
    {
        Vector2D direction = Vector2D((position.x - (width / 2)) - this->position.x, (position.y - (height / 2)) - this->position.y);
        this->velocity.x += direction.normalize().x * speed;
        this->velocity.y += direction.normalize().y * speed;
    }
};

const int BoxColider = 1;

class Colider
{
public:
    int type;
    int width, height;
    Colider(int type)
    {
        this->type = type;
    };

    Colider(int type, int width, int height)
    {
        this->type = type;
        this->width = width;
        this->height = height;
    }

    bool detectBoxBoxCollision(Transform tra1, Transform tra2, Colider colider1, Colider colider2)
    {
        if (tra1.position.x + colider1.width > tra2.position.x && tra1.position.x < tra2.position.x + colider2.width && tra1.position.y + colider1.height > tra2.position.y && tra1.position.y < tra2.position.y + colider2.height)
        {
            return true;
        }
        return false;
    };

    bool detectColition(Transform tra1, Transform tra2, Colider col1, Colider col2)
    {
        switch (col1.type)
        {
        case BoxColider:
            switch (col2.type)
            {
            case BoxColider:
                return detectBoxBoxCollision(tra1, tra2, col1, col2);
            default:
                MessageBox(NULL, L"Colider type not found 2", L"Error", MB_OK);
            };
        default:
            MessageBox(NULL, L"Colider type not found", L"Error", MB_OK);
        };
    };
};
class Object
{
public:
    RGBA color = RGBA(255, 255, 255, 255);
    bool staticObject = false;
    Transform transform = Transform(0, 0, 0, 0);
    Colider colider = Colider(1);

    void (*onClick)(int index){};

    Object(){};
    ~Object(){};

    Object(float x, float y, float width, float height, RGBA color)
    {
        colider = Colider(1, width, height);
        this->transform.position.x = x;
        this->transform.position.y = y;
        this->transform.width = width;
        this->transform.height = height;
        this->color = color;
    };

    Object(Transform tra, int color)
    {
        this->transform = tra;
        this->color = RGBA(color);
    };

    void update(Object objects[], int count, int index)
    {
        if (staticObject)
            return;

        // check for mouse click and mouse hover
        // if (p.x > transform.position.x && p.x < transform.position.x + transform.width && p.y > transform.position.y && p.y < transform.position.y + transform.height)
        // {
        //     if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
        //     {
        //         onClick(index);
        //     }
        // }

        // update transform
        this->transform.update();

        for (int i = 0; i < count; i++)
        {
            if (index != i)
                if (&objects[i].transform.velocity != 0 && this->transform.velocity != 0)
                    if (this->checkColitionWith(&objects[i]))
                    {
                        objects[i].transform.velocity += this->transform.velocity;
                        this->transform.position.x -= this->transform.velocity.x;
                        this->transform.position.y -= this->transform.velocity.y;
                    };
        };
    }

    bool checkColitionWith(Object *obj)
    {
        return colider.detectColition(this->transform, obj->transform, this->colider, obj->colider);
    };

    void draw(Window window)
    {
        window.drawBox(transform.position.x, transform.position.y, transform.width, transform.height, color);
    };
};
