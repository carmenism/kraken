#ifndef COLORLEGEND_H_
#define COLORLEGEND_H_

#include "Legend.h"
#include <vector>

class ColorLegendItem;

class ColorLegend : public Legend {
public:
    ColorLegend();
    ~ColorLegend();

    void addColorLegendItem(ColorLegendItem *item);

protected:
    std::vector<ColorLegendItem *> *items;
    
    virtual void drawAtOrigin();
};

#endif /* LEGEND_H_ */