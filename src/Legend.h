#ifndef LEGEND_H_
#define LEGEND_H_

#include "Displayable.h"
#include "Object2D.h"
#include <string>

class Legend : public Displayable, public Object2D {
public:
    Legend();
    ~Legend();

    void draw();

    float getFontHeight() { return fontHeight; }
    void setFontHeight(float h) { fontHeight = h; }
protected:
    float fontHeight;
    float padding;
    std::string title;

    virtual void drawAtOrigin() = 0;
};

#endif /* LEGEND_H_ */