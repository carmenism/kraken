#include "UndoButton.h"
#include "Slider.h"

UndoButton::UndoButton(Slider *slider) : Button("UNDO") {
    this->slider = slider;
}

UndoButton::~UndoButton() {

}

bool UndoButton::mouseReleased(float x, float y) {
    if (Button::mouseReleased(x, y)) {
        slider->undo();

        return true;
    }

    return false;
}