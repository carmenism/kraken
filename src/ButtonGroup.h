#ifndef BUTTONGROUP_H_
#define BUTTONGROUP_H_

class Button;

#include "MouseEventItem.h"
#include <string>
#include <vector>

class ButtonGroup: public MouseEventItem {
public:
    ButtonGroup(std::string groupLabels, std::vector<std::string> buttonLabels, int inactiveIndex);
    ~ButtonGroup();
    virtual void draw();
    
    virtual bool mouseMoved(float x, float y);
    virtual bool mousePressed(float x, float y);
    virtual bool mouseReleased(float x, float y);

    float getWidth() { return width; }

    float getX() { return x; }
    float getY() { return y; }

    void setLocation(float x, float y) { this->x = x; this->y = y; }
    void setX(float x) { this->x = x; }
    void setY(float y) { this->y = y; }

    int getReleasedIndex() { return releasedIndex; }

    void setActive(int buttonIndex, bool a);

    void activeOff();
protected:
    std::string label;
private:
    float x, y;
    float width, height;
    float spacing;
    int releasedIndex;
    std::vector<Button *> *buttons;
    
    void drawBox();
};

#endif 