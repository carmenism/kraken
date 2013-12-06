#include "ToggleButton.h"
#include "PrintText.h"

ToggleButton::ToggleButton(std::string label) 
: Button("") {
    baseLabel = label;
    value = true;

    float recWidth = PrintText::strokeWidth(label + " Off", 10);
    this->setWidth(recWidth + 8);
}

ToggleButton::ToggleButton(std::string label, bool value)
: Button("") {
    baseLabel = label;
    this->value = value;

    float recWidth = PrintText::strokeWidth(label + " Off", 10);
    this->setWidth(recWidth + 8);
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