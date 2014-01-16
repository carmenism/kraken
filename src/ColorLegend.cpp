#include "ColorLegend.h"
#include "ColorLegendItem.h"
#include "PrintText.h"
#include <GL/glut.h>

ColorLegend::ColorLegend() {
    items = new std::vector<ColorLegendItem *>();
    title = "ARC LEGEND";
}

ColorLegend::~ColorLegend() {
    while (!items->empty()) {
        ColorLegendItem *item = items->back();
        items->pop_back();
        delete item;
    }

    delete items;
}

void ColorLegend::addColorLegendItem(ColorLegendItem *item) {
    items->push_back(item);
}

void ColorLegend::drawAtOrigin() {
    float totalHeight = padding;
    float maxWidth = -1;

    for (unsigned int i = 0; i < items->size(); i++) {
        float h = std::max(fontHeight, items->at(i)->getBoxHeight());
    
        glPushMatrix();
            glTranslatef(padding, totalHeight, 0);
            float w = items->at(i)->draw(fontHeight, padding);
        glPopMatrix();

        maxWidth = std::max(maxWidth, w);
        totalHeight = totalHeight + h + padding;
    }

    float w = PrintText::strokeWidth(title, fontHeight);
    maxWidth = std::max(maxWidth, w);    
    PrintText::drawStrokeText(title, padding + maxWidth / 2.0, totalHeight, fontHeight, HORIZ_CENTER, VERT_BOTTOM);
    totalHeight = totalHeight + fontHeight + padding;

    width = maxWidth + 2 * padding;
    height = totalHeight;
}