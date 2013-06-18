#include "LineGraph.h"
#include "LineGroup.h"
#include <limits>

LineGraph::LineGraph() {

}

LineGraph::~LineGraph() {

}

void LineGraph::redraw() {
    LineGroupList it;
    
    float globalMaxX = -1 * std::numeric_limits<float>::max();

    float globalMinY = std::numeric_limits<float>::min();
    float globalMaxY = -1 * std::numeric_limits<float>::max();

    FOREACH_LINEGROUP(it, lines) {
        if ((*it)->getDisplay()) {
            float maxX = (*it)->getMaximumValueX();

            float minY = (*it)->getMinimumValueY();
            float maxY = (*it)->getMaximumValueY();

            if (globalMaxX < maxX) {
                globalMaxX = maxX;
            }

            if (globalMinY > minY) {
                globalMinY = minY;
            }

            if (globalMaxY < maxY) {
                globalMaxY = maxY;
            }
        }
    }

    FOREACH_LINEGROUP(it, lines) {
        if ((*it)->getDisplay()) {
            (*it)->redraw(width, height, globalMaxX, globalMaxY);
        }
    }
}