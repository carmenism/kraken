#include "ChangeSlider.h"
#include "Color.h"
#include <GL/glut.h>

ChangeSlider::ChangeSlider(std::string label, std::vector<float> *values, int startIndex)
    : Slider(label, values, startIndex) {
    initialize();
}

ChangeSlider::ChangeSlider(std::string title, float min, float max, float start) 
    : Slider(title, min, max, start) {
    initialize();
}

ChangeSlider::~ChangeSlider() {
    delete increaseColor;
    delete decreaseColor;
}

void ChangeSlider::initialize() {
    decreaseColor = &Color::red;
    increaseColor = &Color::skyblue;

    color = increaseColor;
    display = false;
    baselineIndex = 0;
}

void ChangeSlider::draw() {
    if (display) {
        glPolygonMode(GL_FRONT, GL_FILL);
        glColor4f(color->r, color->g, color->b, 0.5);

        float value = getValue();
        float previousValue = getValue();

        if (baselineIndex >= 0 && baselineIndex < valueHistory.size()) {
            previousValue = valueHistory[baselineIndex]; //.back();
        }
        
        determineColor(value, previousValue);

        float valueX = valueToPosition(value);
        float lastValueX = valueToPosition(previousValue);

        glRectf(main->getInnerX() + valueX,     main->getY() - labelFontHeight - 2*main->getBorder(),
                main->getInnerX() + lastValueX, main->getY() + cursor->getHeight() - labelFontHeight - 2 *main->getBorder());
                
    }

    Slider::draw();
}

bool ChangeSlider::mouseMoved(float x, float y) {
    bool ret = Slider::mouseMoved(x, y);

    if (ret) {
        display = true;
    }

    return ret;
}

void ChangeSlider::determineColor(float value, float previousValue) {
    if (previousValue < value) {
        color = increaseColor;
    } else {
        color = decreaseColor;
    }
}

void ChangeSlider::setBaseline() {
    baselineIndex = valueHistory.size() - 1;
}