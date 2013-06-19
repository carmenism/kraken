#ifndef LINEGRAPH_H_
#define LINEGRAPH_H_

class LineGroup;

#include <vector>
#include <QList>

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

    void redraw();

    float getWidth() { return width; }
    void setWidth(float w) { width = w; }

    float getHeight() { return height; }
    void setHeight(float h) { height = h; }
    
    void addLine(LineGroup *line);
    void clearLines() { lines.clear(); }
    LineGroupList getLines() { return lines; }
private:
    float width, height;
    LineGroupList lines;
    
    float globalMinX, globalMaxX;
    float globalMinY, globalMaxY;

    void recalculateGlobalBounds();
    void redrawBoundary();
    void redrawLines();

    float round(float num);
    float f(float num, float c);
    float roundDown(float num);
    float g(float num, float c);
    float roundUp(float num);
};

#endif /*LINEGRAPH_H_*/