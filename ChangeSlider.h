#ifndef CHANGESLIDER_H_
#define CHANGESLIDER_H_

#include "Slider.h"

class ChangeSlider : public Slider {
public:
    ChangeSlider(std::string label, float xcorner, float ycorner, float length, float start);
    
    void draw();

    bool mouseMoved(float x, float y);

    void clearDisplay() { display = false; }
private:
    Color *decreaseColor;
    Color *increaseColor;
    Color *color;
    bool display;
};

#endif /* CHANGESLIDER_H_ */