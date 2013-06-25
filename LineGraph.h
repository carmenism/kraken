#ifndef LINEGRAPH_H_
#define LINEGRAPH_H_

#include <vector>
#include <QList>
#include "LineGroup.h"

class GraphAxis;
class GraphLegend;
class QStringList;

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

    void setLineWidths(float w);

    void setDisplayMarkers(bool d);
    void displayMarkersOn();
    void displayMarkersOff();

    void setMarkersSize(float s);

    void displayAsAreas();
    void displayAsLines();

    GraphMarkerList *getMarkers();

    static LineGraph *createGraph(QList<QList<double>> matrix, QStringList labels);
private:
    GraphAxis *axisX;
    GraphAxis *axisY;
    GraphLegend *legend;

    float width, height;
    LineGroupList lines;
    
    float globalMinX, globalMaxX;
    float globalMinY, globalMaxY;

    void calculateGlobalBounds();
    void drawBoundary();
    void drawLines();
    void drawLabels();
    void drawAxes();

    float round(float num);
    float f(float num, float c);
    float roundDown(float num);
    float g(float num, float c);
    float roundUp(float num);

    float calculateIntervalSize(float min, float max);
};

#endif /*LINEGRAPH_H_*/