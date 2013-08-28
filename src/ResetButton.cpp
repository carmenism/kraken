#include "ResetButton.h"
#include "Slider.h"

ResetButton::ResetButton(Slider *slider) : SliderButton("RESET", slider) {

}

ResetButton::~ResetButton() {

}

bool ResetButton::mouseReleased(float x, float y) {
    if (Button::mouseReleased(x, y)) {
        slider->reset();

        return true;
    }

    return false;
}