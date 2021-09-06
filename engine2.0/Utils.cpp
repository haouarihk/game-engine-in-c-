
#include <math.h>
class RGBA
{
public:
    int r, g, b;
    float a = 1;
    RGBA(int color)
    {
        r = (int)((color >> 24) & 0xFF) / 255.0f;
        g = (int)((color >> 16) & 0xFF) / 255.0f;
        b = (int)((color >> 8) & 0xFF) / 255.0f;
        a = (int)(color & 0xFF) / 255.0f;
    };
    RGBA(int r, int g, int b, float a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    };
    RGBA(int r, int g, int b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = 1.0f;
    };

    RGBA ChangeOpacity(float opacity)
    {
        return RGBA(r, g, b, opacity);
    };

    int toInt()
    {
        return RGB(r, g, b);
    };
};

class Vector2D
{
public:
    float x, y;
    Vector2D()
    {
        x = 0;
        y = 0;
    };
    Vector2D(float x, float y)
    {
        this->x = x;
        this->y = y;
    };
    Vector2D(const Vector2D &other)
    {
        this->x = other.x;
        this->y = other.y;
    };
    Vector2D &operator=(const Vector2D &other)
    {
        this->x = other.x;
        this->y = other.y;
        return *this;
    };
    Vector2D operator+(const Vector2D &other)
    {
        return Vector2D(this->x + other.x, this->y + other.y);
    };
    Vector2D operator-(const Vector2D &other)
    {
        return Vector2D(this->x - other.x, this->y - other.y);
    };

    Vector2D operator-(const float &other)
    {
        return Vector2D(this->x - other, this->y - other);
    }

    Vector2D operator*(const Vector2D &other)
    {
        return Vector2D(this->x * other.x, this->y * other.y);
    };
    Vector2D operator/(const Vector2D &other)
    {
        return Vector2D(this->x / other.x, this->y / other.y);
    };
    Vector2D operator*(const float &other)
    {
        return Vector2D(this->x * other, this->y * other);
    }

    Vector2D operator+=(const Vector2D &other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }
    Vector2D operator-=(const Vector2D &other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }

    bool operator!=(const float &other)
    {
        return (this->x != other || this->y != other);
    }

    // magnitude
    float magnitude()
    {
        return sqrt(x * x + y * y);
    };
    // normalize
    Vector2D normalize()
    {
        float mag = magnitude();
        return Vector2D(x / mag, y / mag);
    };
    // inverse
    Vector2D inverse()
    {
        return Vector2D(-x, -y);
    };

    // change magnitude
    Vector2D changeMagnitude(float magnitude)
    {
        return normalize() * magnitude;
    };
};

namespace Vector2DUtils
{
    Vector2D Up()
    {
        return Vector2D(0, 1);
    };
    Vector2D Down()
    {
        return Vector2D(0, -1);
    };
    Vector2D Left()
    {
        return Vector2D(-1, 0);
    };
    Vector2D Right()
    {
        return Vector2D(1, 0);
    };
    Vector2D Zero()
    {
        return Vector2D(0, 0);
    };
}