#ifndef CHANGESLIDER_H_
#define CHANGESLIDER_H_

#include "Slider.h"

class ChangeSlider : public Slider {
public:
    ChangeSlider(std::string label, float min, float max, float start);
    ~ChangeSlider();

    void draw();

    bool mouseMoved(float x, float y);

    void clearDisplay() { display = false; }
private:
    Color *decreaseColor;
    Color *increaseColor;
    Color *color;
    bool display;

    void determineColor(float value, float lastValue);
};

#endif /* CHANGESLIDER_H_ */