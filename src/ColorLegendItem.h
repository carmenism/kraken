#ifndef COLORLEGENDITEM_H_
#define COLORLEGENDITEM_H_

#include "Color.h"
#include "Displayable.h"
#include <string>

class ColorLegendItem : public Displayable {
public:
    ColorLegendItem(Color *color, std::string label);
    ~ColorLegendItem();

    float draw(float fontHeight, float padding);

    float getBoxWidth() { return boxWidth; }
    float getBoxHeight() { return boxHeight; }

    Color *getBoxColor() { return boxColor; }
    void setBoxColor(Color *c) { boxColor = c; }
protected:
    float boxWidth, boxHeight;
    Color *boxColor;
    std::string label;
};

#endif /* COLORLEGENDITEM_H_ */