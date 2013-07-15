#ifndef RESETBUTTON_H_
#define RESETBUTTON_H_

class Slider;

#include "SliderButton.h"

class ResetButton: public SliderButton
{
public:
    ResetButton(Slider *slider);
    virtual ~ResetButton();
    
    bool mouseReleased(float x, float y);
};

#endif /*RESETBUTTON_H_*/