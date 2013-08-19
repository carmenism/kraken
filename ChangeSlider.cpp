#include "ChangeSlider.h"
#include "Color.h"
#include <GL/glut.h>

ChangeSlider::ChangeSlider(std::string title, float min, float max, float start) 
    : Slider(title, min, max, start) {
    decreaseColor = &Color::red;
    increaseColor = &Color::skyblue;

    color = increaseColor;
    display = false;
}

ChangeSlider::~ChangeSlider() {
    delete increaseColor;
    delete decreaseColor;
}

void ChangeSlider::draw() {
    if (display) {
        glPolygonMode(GL_FRONT, GL_FILL);
        glColor4f(color->r, color->g, color->b, 0.5);
        glRectf(main->getInnerX() + startCurX, main->getY() - labelFontHeight - 2*main->getBorder(),
                main->getInnerX() + curX,      main->getY() + cursor->getHeight() - labelFontHeight - 2 *main->getBorder());
    }

    Slider::draw();
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