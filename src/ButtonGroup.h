#ifndef BUTTONGROUP_H_
#define BUTTONGROUP_H_

class Button;

#include "MouseEventItem.h"
#include "Object2D.h"
#include "Displayable.h"
#include <string>
#include <vector>

class ButtonGroup: public MouseEventItem, public Displayable, public Object2D {
public:
    ButtonGroup(std::string groupLabels, std::vector<std::string> buttonLabels, int inactiveIndex);
    ~ButtonGroup();
    virtual void draw();
    
    virtual bool mouseMoved(float x, float y);
    virtual bool mousePressed(float x, float y);
    virtual bool mouseReleased(float x, float y);

    int getReleasedIndex() { return releasedIndex; }

    bool getActive(int buttonIndex);
    void setActive(int buttonIndex, bool a);

    void activeOff();
protected:
    std::string label;
private:
    float spacing;
    int releasedIndex;
    std::vector<Button *> *buttons;
};

#endif 