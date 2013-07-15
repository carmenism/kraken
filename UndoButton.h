#ifndef UNDOBUTTON_H_
#define UNDOBUTTON_H_

class Slider;

#include "Button.h"

class UndoButton: public Button
{
public:
    UndoButton(Slider *slider);
    virtual ~UndoButton();
    
    bool mouseReleased(float x, float y);
private:
    Slider *slider;
};

#endif /*UNDOBUTTON_H_*/