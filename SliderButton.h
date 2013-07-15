#ifndef SLIDERBUTTON_H_
#define SLIDERBUTTON_H_

class Slider;

#include "Button.h"

class SliderButton: public Button
{
public:
    SliderButton(std::string label, Slider *s) : Button(label) { slider = s; }
    ~SliderButton() {}

    Slider *getSlider() { return slider; }
    void setSlider(Slider *s) { slider = s; }

    virtual bool mouseReleased(float x, float y) = 0;
protected:
    Slider *slider;
};

#endif /*SLIDERBUTTON_H_*/