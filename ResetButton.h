#ifndef RESETBUTTON_H_
#define RESETBUTTON_H_

class Slider;

#include "Button.h"

class ResetButton: public Button
{
public:
    ResetButton(Slider *slider);
    virtual ~ResetButton();
    
    bool mouseReleased(float x, float y);
private:
    Slider *slider;
};

#endif /*RESETBUTTON_H_*/