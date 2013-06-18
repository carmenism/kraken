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
private:
    float width, height;
    LineGroupList lines;
    float round(float num);
    float roundUp(float num);
    float roundDown(float num);
};

#endif /*LINEGRAPH_H_*/