#ifndef UNDOBUTTON_H_
#define UNDOBUTTON_H_

class Slider;

#include "SliderButton.h"

class UndoButton: public SliderButton
{
public:
    UndoButton(Slider *slider);
    virtual ~UndoButton();
    
    bool mouseReleased(float x, float y);
};

#endif /*UNDOBUTTON_H_*/