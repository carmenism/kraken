#include "ChangeSlider.h"
#include "Color.h"
#include <GL/glut.h>

ChangeSlider::ChangeSlider(std::string label, float min, float max, float start) 
    : Slider(label, min, max, start) {
    decreaseColor = &Color::red;
    increaseColor = &Color::blue;

    color = increaseColor;
    display = false;
}

void ChangeSlider::draw() {
    Slider::draw();

    if (display) {
        glColor4f(color->r, color->g, color->b, 0.5);
        glRectf(cornerX + startCurX,  cornerY + border,
                cornerX + curX,    cornerY + border + height);
    }
}

bool ChangeSlider::mouseMoved(float x, float y) {
    bool ret = Slider::mouseMoved(x, y);

    if (ret) {
        display = true;
    }

    if (startCurX < curX) {
        color = increaseColor;
    } else {
        color = decreaseColor;
    }

    return ret;
}