#ifndef LINEGROUP_H_
#define LINEGROUP_H_

#include <vector>

class GraphMarker;
class Color;

typedef std::vector<GraphMarker *> GraphMarkerList;
typedef std::vector<GraphMarker *>::const_iterator GraphMarkerIterator;

#define FOREACH_MARKER(it, graphMarkerList) \
    for(GraphMarkerIterator it = graphMarkerList.begin(); it != graphMarkerList.end(); ++it)
#define FOREACH_MARKERP(it, graphMarkerList) \
    for(GraphMarkerIterator it = graphMarkerList->begin(); it != graphMarkerList->end(); ++it)

class LineGroup
{
public:
    LineGroup(std::string label, std::vector<float> x, std::vector<float> y);
    virtual ~LineGroup();
    
    void redraw(float graphWidth, float graphHeight,
        float maxValueX,  float maxValueY);

    float getMaximumValueX();
    float getMaximumValueY();  
    
    float getMinimumValueX();
    float getMinimumValueY();

    void setDisplay(bool d) { display = d; }
    bool getDisplay() { return display; }
    void displayOn() { display = true; }
    void displayOff() { display = false; }

    void setDisplayMarkers(bool d) { displayMarkers = d; }
    bool getDisplayMarkers() { return displayMarkers; }
    void displayMarkersOn() { displayMarkers = true; }
    void displayMarkersOff() { displayMarkers = false; }

    void setShape(int shape);
    void setSize(float size);
    void setBorderColor(Color *color);
    void setFillColor(Color *color);
    void setBorderWidth(float width);

private:
    std::string label;
    GraphMarkerList markers;
    GraphMarker *min, *max;
    //float maxX, maxY;
    bool display, displayMarkers;
};

#endif /*LINEGROUP_H_*/