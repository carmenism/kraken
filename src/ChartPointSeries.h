#ifndef CHARTPOINTSERIES_H_
#define CHARTPOINTSERIES_H_

#include "Displayable.h"
#include <vector>

class LineChart;
class ChartPoint;
class Point;
class Color;
class Shape;

typedef std::vector<ChartPoint *> ChartPointList;
typedef std::vector<ChartPoint *>::const_iterator ChartPointIterator;

#define FOREACH_POINT(it, chartPointList) \
    for(ChartPointIterator it = chartPointList.begin(); it != chartPointList.end(); ++it)
#define FOREACH_POINTP(it, chartPointList) \
    for(ChartPointIterator it = chartPointList->begin(); it != chartPointList->end(); ++it)

class ChartPointSeries : public Displayable
{
public:
    ChartPointSeries(LineChart *chart, std::string label, std::vector<float> *x, std::vector<float> *y);
    ~ChartPointSeries();
    
    void setValues(std::vector<float> *x, std::vector<float> *y);

    void draw();
    void drawSelected();

    float drawInLegend(float x, float y, float lineLength, float spacing, float h);

    void drawToPick();

    float getMaximumValueX();
    float getMaximumValueY();  
    
    float getMinimumValueX();
    float getMinimumValueY();
    
    float getPreviousMinimumValueY();
    float getPreviousMaximumValueY(); 

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
    Color *getColor();

    ChartPointList *getPoints() { return points; }

    void capturePreviousValues();

    //void drawGhost();
    
    void drawGhostAsLine();
    void drawGhostAsBlend();

    int size();

    float getStartValue();

    float getPercentIncreaseOfFinalValue();
    float getFinalValue();
    float getPreviousFinalValue();
private:
    LineChart *chart;

    std::string label;
    ChartPointList *points;
    ChartPoint *legendPoint;
    ChartPoint *min, *max;
    Point *previousMin, *previousMax;
    float lineWidth;
    Color *lineColor;
    bool displayMarkers, displayAsArea;

    void calculatePointLocations();
    void drawAsLines();
    void drawAsArea();    
};

#endif /*CHARTPOINTSERIES_H_*/