#ifndef CHANGESLIDER_H_
#define CHANGESLIDER_H_

#include "Slider.h"
#include <vector>

class ChangeSlider : public Slider {
public:
    ChangeSlider(std::string label, std::vector<float> *values, int startIndex);
    ChangeSlider(std::string label, float min, float max, float start);
    ~ChangeSlider();

    void draw();

    bool mouseMoved(float x, float y);

    void clearDisplay() { display = false; }
    void setBaseline();
private:
    Color *decreaseColor;
    Color *increaseColor;
    Color *color;
    bool display;
    int baselineIndex;

    void determineColor(float value, float previousValue);

    void initialize();
};

#endif /* CHANGESLIDER_H_ */