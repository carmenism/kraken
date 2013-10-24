#ifndef TOGGLEBUTTON_H_
#define TOGGLEBUTTON_H_

#include "Button.h"

class ToggleButton: public Button
{
public:
    ToggleButton(std::string label);
    ToggleButton(std::string label, bool value);
    ~ToggleButton();
    virtual void draw();
    
    virtual bool mouseReleased(float x, float y);

    bool getValue() { return value; }
    void setValue(bool v) { value = v; }
private:
    std::string baseLabel;
    bool value;
};

#endif /*TOGGLEBUTTON_H_*/