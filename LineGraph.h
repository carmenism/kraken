#ifndef LINEGRAPH_H_
#define LINEGRAPH_H_

#include <vector>
#include <QList>
#include "LineGroup.h"

class GraphAxis;

typedef std::vector<LineGroup *> LineGroupList;
typedef std::vector<LineGroup *>::const_iterator LineGroupIterator;

#define FOREACH_LINEGROUP(it, lineGroupList) \
    for(LineGroupIterator it = lineGroupList.begin(); it != lineGroupList.end(); ++it)
#define FOREACH_LINEGROUPP(it, lineGroupList) \
    for(LineGroupIterator it = lineGroupList->begin(); it != lineGroupList->end(); ++it)

class LineGraph {
public:
    LineGraph();
    ~LineGraph();

    void draw();
    void drawToPick();
    void drawToPickLines();

    float getWidth() { return width; }
    void setWidth(float w) { width = w; }

    float getHeight() { return height; }
    void setHeight(float h) { height = h; }
    
    void addLine(LineGroup *line);
    void clearLines() { lines.clear(); }
    LineGroupList getLines() { return lines; }

    GraphMarkerList *getMarkers();
private:
    GraphAxis *axisX;
    GraphAxis *axisY;

    float width, height;
    LineGroupList lines;
    
    float globalMinX, globalMaxX;
    float globalMinY, globalMaxY;

    void calculateGlobalBounds();
    void drawBoundary();
    void drawLines();

    float round(float num);
    float f(float num, float c);
    float roundDown(float num);
    float g(float num, float c);
    float roundUp(float num);
};

#endif /*LINEGRAPH_H_*/