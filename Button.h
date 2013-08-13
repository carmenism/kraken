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

    //virtual void setActive(bool a);
    /*bool isActive() { return active; }
    void setActive(bool a);
    void activeOn() { active = true; }
    void activeOff();*/
private:
    std::string label;
    Color *labelColor, *labelHoveredColor, *labelActiveColor;
    //bool mouseIsHovering;
    //bool mouseIsPressing;
    //bool active;
};

#endif /*BUTTON_H_*/