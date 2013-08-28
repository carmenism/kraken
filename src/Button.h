#ifndef BUTTON_H_
#define BUTTON_H_

class Color;

#include "MouseEventItem.h"
#include "ShadowedRectangle.h"
#include <string>

class Button: public MouseEventItem, public ShadowedRectangle
{
public:
    Button(std::string label);
    ~Button();
    void draw();
    
    virtual bool mouseMoved(float x, float y);
    virtual bool mousePressed(float x, float y);
    virtual bool mouseReleased(float x, float y);
protected:
    std::string label;
private:
    Color *labelColor, *labelHoveredColor, *labelActiveColor;
};

#endif /*BUTTON_H_*/