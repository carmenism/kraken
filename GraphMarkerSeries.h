#ifndef GRAPHMARKERSERIES_H_
#define GRAPHMARKERSERIES_H_

#include <vector>
//#include "Shape.h"

class ChartPoint;
class Color;
class Shape;

typedef std::vector<ChartPoint *> ChartPointList;
typedef std::vector<ChartPoint *>::const_iterator ChartPointIterator;

#define FOREACH_POINT(it, chartPointList) \
    for(ChartPointIterator it = chartPointList.begin(); it != chartPointList.end(); ++it)
#define FOREACH_POINTP(it, chartPointList) \
    for(ChartPointIterator it = chartPointList->begin(); it != chartPointList->end(); ++it)

class GraphMarkerSeries
{
public:
    GraphMarkerSeries(std::string label, std::vector<float> x, std::vector<float> y);
    virtual ~GraphMarkerSeries();
    
    void setValues(std::vector<float> x, std::vector<float> y);

    void draw(float chartWidth, float chartHeight,
              float maxValueX,  float maxValueY);
    void drawLabels();

    float drawInLegend(float x, float y, float lineLength, float spacing, void *font);

    void drawToPick(float chartWidth, float chartHeight,
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

    void setDisplayAsArea(bool d) { displayAsArea = d; }
    bool getDisplayAsArea() { return displayAsArea; }
    void displayAsAreaOn() { displayAsArea = true; }
    void displayAsAreaOff() { displayAsArea = false; }

    void setMarkerShape(int shape);
    void setMarkerSize(float size);
    void setMarkerBorderColor(Color *color);
    void setMarkerFillColor(Color *color);
    void setMarkerBorderWidth(float width);
    
    void setLineColor(Color *c) { lineColor = c; }

    float getLineWidth() { return lineWidth; }
    void setLineWidth(float w) { lineWidth = w; }

    void setColor(Color *c);

    ChartPointList *getPoints() { return &points; }// { return markers; }
private:
    std::string label;
    ChartPointList points;
    ChartPoint *min, *max;
    float lineWidth;
    Color *lineColor;
    bool display, displayMarkers, displayAsArea;

    void calculatePointLocations(float chartWidth, float chartHeight,
                                 float maxValueX,  float maxValueY);
    void drawAsLines();
    void drawAsArea();
};

#endif /*GRAPHMARKERSERIES_H_*/