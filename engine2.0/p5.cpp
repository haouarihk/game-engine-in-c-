#include <windows.h>

void CreateCanvas(float width, float height)
{
    window.buffer_width = width;
    window.buffer_height = height;
    window.updateSize();
    window.resizeWindow(window.buffer_width, window.buffer_height);
}

//-- background

/* background color with rgba, between 0 and 255 */
void background(float r, float g, float b, float a = 1.0f)
{
    window.drawBackground(RGBA(r, g, b, a));
}

/* color variant between white and black, between 0 and 255*/
void background(float color)
{
    background(color, color, color, 1.0f);
}

/* color using RGBA built in tool*/
void background(RGBA color)
{
    background(color.r, color.g, color.b, color.a);
}
//--

// fill
bool isFill = false;
RGBA fillColor = RGBA(0, 0, 0, 1);

/* color variant between white and black, between 0 to 255 */
void fill(float color)
{
    fillColor = RGBA(color, color, color, 1);
};

/*  color with rgba, between 0 to 255 */
void fill(float r, float g, float b, float a = 1.0f)
{
    fillColor = RGBA(r, g, b, 1);
};

/* color using RGBA built in tool */
void fill(RGBA color)
{
    fillColor = color;
};

/* no filling drawings*/
void noFill()
{
    isFill = false;
}

// stroke
float isStroke = true;
RGBA strokeColor = RGBA(0, 0, 0, 1);
float _strokeWeight = 1;

/* color variant between white and black, between 0 to 255 */
void stroke(float color)
{
    strokeColor = RGBA(color, color, color, 1);
};

/*  color with rgba, between 0 to 255 */
void stroke(float r, float g, float b, float a = 1.0f)
{
    strokeColor = RGBA(r, g, b, 1);
};

/* color using RGBA built in tool */
void stroke(RGBA color)
{
    strokeColor = color;
};
//

void noStroke()
{
    isStroke = false;
}

/* weight of the outlines */
void strokeWeight(float thiccness)
{
    _strokeWeight = thiccness;
}

// draw rect
void rect(float x, float y, float w, float h)
{
    window.drawRect(x, y, w, h, isStroke, isFill, _strokeWeight, strokeColor, fillColor);
}

void rect(float x, float y, float w)
{
    rect(x, y, w, w);
}

// [ ] rounded corners
// still more coming ...

// draw circle
void circle(float x, float y, float r)
{
    window.drawCircle(x, y, r, fillColor);
}
//