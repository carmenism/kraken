#ifndef BUTTON_H_
#define BUTTON_H_

class Color;

#include "ShadowedRectangle.h"
#include <string>

class Button: public ShadowedRectangle
{
public:
    Button(std::string label);
    virtual ~Button();
    void draw();
    
    bool mouseMoved(float x, float y);
    bool mousePressed(float x, float y);
    bool mouseReleased(float x, float y);
private:
    std::string label;
    Color *labelColor, *labelHoveredColor, *labelActiveColor;
    bool hovered;
    bool active;
};

#endif /*BUTTON_H_*/