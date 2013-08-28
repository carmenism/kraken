#include "ToggleButton.h"

ToggleButton::ToggleButton(std::string label) 
: Button("") {
    baseLabel = label;
    value = true;
}

ToggleButton::ToggleButton(std::string label, bool value)
: Button("") {
    baseLabel = label;
    this->value = value;
}

void ToggleButton::draw() {
    if (value) {
        label = baseLabel + " On";
    } else {
        label = baseLabel + " Off";
    }

    Button::draw();
}

bool ToggleButton::mouseReleased(float x, float y) {
    bool released = Button::mouseReleased(x, y);

    if (released) {
        value = !value;

        return true;
    }

    return false;
}